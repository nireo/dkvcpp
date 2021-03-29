//
// Created by eemil on 3/30/21.
//

#ifndef DKVCPP_SERVER_HPP
#define DKVCPP_SERVER_HPP

#include "leveldb/db.h"
#include "./include/httplib.h"
#include <string>

typedef unsigned int uint32;

class Server {
public:
    void init_database_connection(const std::string& path);
    void start(const std::string& addr, int port);

    uint32 get_key_shard(const std::string& key) const;

    leveldb::Status put(const std::string& key, const std::string& val);
    leveldb::Status get(const std::string& key, std::string *val);
    leveldb::Status del(const std::string& key);

    ~Server() {
        delete m_db;
    }
private:
    leveldb::DB *m_db;
    httplib::Server m_srv;

    unsigned int m_shard_count;
};


#endif //DKVCPP_SERVER_HPP
