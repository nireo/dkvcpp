//
// Created by eemil on 3/30/21.
//

#include "server.hpp"

void Server::init_database_connection(const std::string& path) {
    leveldb::Options options;
    options.create_if_missing = true;

    // open
    leveldb::Status status = leveldb::DB::Open(options, path, &m_db);
    assert(status.ok());
}

void Server::start(const std::string& addr, int port) {
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

uint32 Server::get_key_shard(const std::string& key) const {
    const uint32 fnv_prime = 0x811C9DC5;
	uint32 hash = 0;
	for (uint32 i = 0; i < key.length(); i++)
	{
		hash *= fnv_prime;
		hash ^= (key[i]);
	}

	return hash % m_shard_count;
}
