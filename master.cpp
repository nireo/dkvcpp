//
// Created by eemil on 4/5/21.
//

#include "master.hpp"
#include "./include/httplib.h"

void Master::start_heartbeat_checks() {
    std::vector<Shard> shards;
    shards = this->m_conf->get_shards();
    std::thread heartbeat_thread([&] {
        // every 15 minutes check that a server is alive
        for (auto& sh : shards) {
            httplib::Client cli(sh.addr.c_str());
            auto res =cli.Get("/heartbeat");
            if (res->status != 200) {
                std::cout << "problem with server: " <<  sh.addr << "\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::minutes(15));
    });
}
