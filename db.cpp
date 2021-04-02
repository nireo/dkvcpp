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

// get_key_list returns all the keys in a given shard database
std::string DB::get_key_list() const {
    std::string res;
    leveldb::Iterator* it = m_db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid; it->Next()) {
        res += it->key().ToString() + "\n";
    }

    return res;
}