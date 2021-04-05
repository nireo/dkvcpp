//
// Created by eemil on 4/5/21.
//

#ifndef DKVCPP_MASTER_HPP
#define DKVCPP_MASTER_HPP

#include "config.hpp"
#include <thread>
#include <chrono>

// Master is the class that administrates the shard servers
class Master {
public:
    void start_heartbeat_checks();

    ~Master() {
        delete m_conf;
    }

private:
    Config* m_conf;
};


#endif //DKVCPP_MASTER_HPP
