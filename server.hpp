//
// Created by eemil on 3/30/21.
//

#ifndef DKVCPP_SERVER_HPP
#define DKVCPP_SERVER_HPP

#include "leveldb/db.h"
#include "db.hpp"
#include "./include/httplib.h"
#include "config.hpp"
#include <string>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

typedef unsigned int uint32;

class Server {
public:
    void start(const std::string& addr, int port);

    void get_key_handler(const httplib::Request& req, httplib::Response& res);
    void put_key_handler(const httplib::Request& req, httplib::Response& res);
    void del_key_handler(const httplib::Request& req, httplib::Response& res);

    explicit Server(DB* db);
    ~Server() = default;
private:
    DB* m_db{};
    httplib::Server m_srv{};
};


#endif //DKVCPP_SERVER_HPP
