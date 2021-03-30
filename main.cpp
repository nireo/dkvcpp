#include <iostream>
#include <string>
#include <cassert>
#include "leveldb/db.h"
#include "server.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "you need to provide the current shard index" << "\n";
        return EXIT_FAILURE;
    }

    int curr_shard = std::stoi(argv[1]);
    Config config("sharding.toml");

    Server s;
    s.init_database_connection("/tmp/database");

    s.start("localhost", 8080);
    return EXIT_SUCCESS;
}