#include <iostream>
#include <string>
#include "db.hpp"
#include "config.hpp"
#include "./include/httplib.h"

using namespace std::string_view_literals;
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "you need to provide the current shard name" << "\n";
        return EXIT_FAILURE;
    }

    Config conf;
    conf.verify_shards(std::string(argv[1]));

    DB db("./dkvdb");
    httplib::Server srv;

    srv.Get("/get", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        // calculate the shard hash for the given key
        uint32 shard = conf.get_key_shard(key);
        if (shard != conf.get_index()) {
            // if the shard doesn't match we need to move it to the matching shard
            std::string addr = conf.get_shard_addr(shard);
            res.set_redirect("http://"+addr + "/get?key="+key);
            return;
        }

        std::cout << key << "\n";
        std::string value;
        leveldb::Status st = db.get(key, &value);
        if (st.ok()) {
            res.status = 200;
            res.set_content(value, "text/plain");
        } else {
            // the key could not be found
            res.status = 404;
            res.set_content("value not found", "text/plain");
        }
    });

    // this is also get such that redirecting doesn't screw up and it really doesn't matter which one it is :)
    srv.Get("/set", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");

        uint32 shard = conf.get_key_shard(key);
        if (shard != conf.get_index()) {
            std::string addr = conf.get_shard_addr(shard);
            res.set_redirect("http://"+addr+"/set?key="+key+"&val="+value);
            return;
        }

        leveldb::Status st = db.put(key, value);
        if (st.ok()) {
            res.status = 201;
            res.set_content("successfully set value","text/plain");
        } else {
            res.status = 500;
            res.set_content("error setting value","text/plain");
        }
    });

    srv.Get("/list", [&](const httplib::Request& req, httplib::Response& res) {
        auto content = db.get_key_list();
        res.set_content(content, "text/plain");
    });

    srv.Get("/del", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        uint32 shard = conf.get_key_shard(key);
        if (shard != conf.get_index()) {
            std::string addr = conf.get_shard_addr(shard);
            res.set_redirect("http://"+addr+"/del?key="+key);
            return;
        }

        db.del(key);
    });

    srv.Post("/stop", [&](const httplib::Request& req, httplib::Response& res) {
        srv.stop();
    });

    srv.listen("localhost", 8080);
    return EXIT_SUCCESS;
}