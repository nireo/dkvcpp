//
// Created by eemil on 3/31/21.
//

#ifndef DKVCPP_DB_HPP
#define DKVCPP_DB_HPP

#include "leveldb/db.h"
#include <string>

class DB {
public:
    leveldb::Status put(const std::string& key, const std::string& val);
    leveldb::Status get(const std::string& key, std::string *val);
    leveldb::Status del(const std::string& key);
    [[nodiscard]] std::string get_key_list() const;

    explicit DB(const std::string& path);
    ~DB() {
        delete m_db;
    }
private:
    leveldb::DB *m_db{};
};


#endif //DKVCPP_DB_HPP
