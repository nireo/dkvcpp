//
// Created by eemil on 4/6/21.
//

#include <iostream>
#include "./include/httplib.h"
#include "leveldb/db.h"
#include "db.hpp"

// this contains the shard server which the master connects.
int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "usage: ./shard [address] [port] [db_addr]" << "\n";
        return EXIT_FAILURE;
    }

    // make sure that the port parameter is a number
    int32_t port;
    try {
        port = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
        std::cout << "port number is not a number: " << e.what() << "\n";
        return EXIT_FAILURE;
    } catch(...) {
        std::cout << "something went wrong parsing port number" << "\n";
    }

    DB db(argv[3]);
    httplib::Server srv;

    srv.Get("/get", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");

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
        // we don't need to check if the key hash is correct, since the master server takes care of that
        db.del(key);
    });

    srv.Get("/heartbeat", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
    });

    srv.Post("/stop", [&](const httplib::Request& req, httplib::Response& res) {
        srv.stop();
    });

    std::cout << "starting shard server..." << "\n";
    srv.listen(argc[0], port);

    return EXIT_SUCCESS;
}