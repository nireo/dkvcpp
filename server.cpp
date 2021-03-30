//
// Created by eemil on 3/30/21.
//

#include "server.hpp"
#include <memory>
#include <string>
#include <stdexcept>

void Server::init_database_connection(const std::string& path) {
    leveldb::Options options;
    options.create_if_missing = true;

    // open
    leveldb::Status status = leveldb::DB::Open(options, path, &m_db);
    assert(status.ok());
}

template<typename ... Args>
std::string ssprintf( const std::string& format, Args ... args )
{
    int size_s = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

void Server::start(const std::string& addr, int port) {
    m_srv.Get("/key", [this](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        std::string value;
        leveldb::Status st = this->get(key, &value);
        if (st.ok()) {
            res.status = 200;
            res.set_content(value, "text/plain");
        } else {
            // the key could not be found
            res.status = 404;
            res.set_content("value not found", "text/plain");
        }
    });

    m_srv.Put("/key", [this](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");

        leveldb::Status st = this->put(key, value);
        if (st.ok()) {
            res.status = 201;
            res.set_content(ssprintf("successfully set %s to %s", key, value),
                            "text/plain");
        } else {
            res.status = 500;
        }
    });

    m_srv.Delete("/key", [this](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        leveldb::Status st = this->del(key);
        if (st.ok()) {
            res.status = 204;
        } else {
            res.status = 404;
        }
    });


    m_srv.listen(addr.c_str(), port);
}

leveldb::Status Server::put(const std::string& key, const std::string& val) {
    return m_db->Put(leveldb::WriteOptions(), key, val);
}

leveldb::Status Server::get(const std::string& key, std::string* val) {
    return m_db->Get(leveldb::ReadOptions(), key, val);
}

leveldb::Status Server::del(const std::string& key) {
    return m_db->Delete(leveldb::WriteOptions(), key);
}

void Server::set_config(Config *conf) {
    m_conf = conf;
}


