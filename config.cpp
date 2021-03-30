//
// Created by eemil on 3/30/21.
//

#include "config.hpp"
#include <fstream>
#include "./include/toml11/toml.hpp"

Config::Config(const std::string &conf_filepath) {
    // TODO: parse the toml shard array and store the values into a 'Shard' vector.
    auto data = toml::parse(conf_filepath);
}

uint32 Config::get_key_shard(const std::string& key) const {
    const uint32 fnv_prime = 0x811C9DC5;
    uint32 hash = 0;
    for (char i : key)
    {
        hash *= fnv_prime;
        hash ^= i;
    }

    return hash % m_shard_count;
}

bool Config::verify_shards(const std::vector<Shard>& shards, const std::string& cur_name) {
    int curr_index = -1;
    int shard_count = static_cast<int>(shards.size());

    for (auto& shard : shards) {
        m_addrs[shard.index] = shard.addr;
        if (cur_name == shard.name) {
            curr_index = shard.index;
        }
    }

    // verify that every shard has a address
    for (int i = 0; i < shard_count; ++i) {
        if (m_addrs.find(i) == m_addrs.end()) {
            // invalid shards
            return false;
        }
    }

    // the current name doesn't correspond to any shard.
    if (curr_index < 0) {
        return false;
    }

    m_current_index = curr_index;
    m_shard_count = shard_count;
    return true;
}
