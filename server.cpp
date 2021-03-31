//
// Created by eemil on 3/30/21.
//

#include "server.hpp"
#include <memory>
#include <string>
#include <stdexcept>

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
        std::cout << key << "\n";
        std::string value;
        leveldb::Status st = this->m_db->get(key, &value);
        if (st.ok()) {
            res.status = 200;
            res.set_content(value, "text/plain");
        } else {
            // the key could not be found
            res.status = 404;
            res.set_content("value not found", "text/plain");
        }
    });

    m_srv.Put("/key", [&, this](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");

        leveldb::Status st = this->m_db->put(key, value);
        if (st.ok()) {
            res.status = 201;
            res.set_content("successfully set value","text/plain");
        } else {
            res.status = 500;
            res.set_content("error setting value","text/plain");
        }
    });

    m_srv.listen(addr.c_str(), port);
}

Server::Server(DB *db) {
    m_db = db;
}
