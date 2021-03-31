//
// Created by eemil on 3/31/21.
//

#include "db.hpp"

DB::DB(const std::string &path) {
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &m_db);
    assert(status.ok());
}

leveldb::Status DB::put(const std::string& key, const std::string& val) {
    return m_db->Put(leveldb::WriteOptions(), key, val);
}

leveldb::Status DB::get(const std::string& key, std::string* val) {
    return m_db->Get(leveldb::ReadOptions(), key, val);
}

leveldb::Status DB::del(const std::string& key) {
    return m_db->Delete(leveldb::WriteOptions(), key);
}
