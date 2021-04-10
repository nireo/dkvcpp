#include <iostream>
#include <string>
#include "config.hpp"
#include "./include/httplib.h"
#include <chrono>
#include <thread>

int main() {
    Config conf("conf.json");
    httplib::Server srv;

    auto heartbeat_check = [&]() {
        for (;;) {
            // for each shard check that it is alive
            for (auto& sh : conf.get_shards()) {
                if (!sh.alive)
                    continue;

                httplib::Client cli(sh.addr.c_str());
                auto res =cli.Get("/heartbeat");
                if (res->status != 200) {
                    std::cout << "problem with server at addr: " <<  sh.addr << ", name: " << sh.name << "\n";
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    };

    std::thread thread_object(heartbeat_check);

    srv.Get("/get", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto shard = conf.get_key_shard(key);
        auto addr = conf.get_shard_addr(shard);

        // redirect the request
        res.set_redirect(addr + "/get?key="+key);
    });

    // this is also get such that redirecting doesn't screw up and it really doesn't matter which one it is :)
    srv.Get("/set", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");

        auto shard = conf.get_key_shard(key);
        auto addr = conf.get_shard_addr(shard);
        res.set_redirect(addr+"/set?key="+key+"&val="+value);
    });

    srv.Get("/del", [&](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto shard = conf.get_key_shard(key);
        auto addr = conf.get_shard_addr(shard);
        res.set_redirect(addr+"/set?key="+key+"&val="+value);
    });

    srv.Post("/stop", [&](const httplib::Request& req, httplib::Response& res) {
        srv.stop();
    });

    srv.listen("localhost", 8080);
    return EXIT_SUCCESS;
}
