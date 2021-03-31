#include <iostream>
#include <string>
#include "server.hpp"
#include "db.hpp"


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "you need to provide the current shard index" << "\n";
        return EXIT_FAILURE;
    }

    //int curr_shard = std::stoi(argv[1]);
    //Config config("sharding.toml");

    DB db("./dkvdb");
    httplib::Server srv;


    srv.Get("/key", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
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

    srv.Put("/key", [&](const httplib::Request& req, httplib::Response& res) {
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

    srv.listen("localhost", 8080);
    return EXIT_SUCCESS;
}