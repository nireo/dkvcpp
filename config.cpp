//
// Created by eemil on 3/30/21.
//

#include "config.hpp"
#include "./include/json.hpp"
#include <fstream>

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

bool Config::verify_shards(const std::string& cur_name) {
    int curr_index = -1;
    int shard_count = static_cast<int>(m_shards.size());

    for (auto& shard : m_shards) {
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

Config::Config(const std::string& path) {
     // read a JSON file
    std::ifstream i(path);
    nlohmann::json j;
    i >> j;

    std::vector<Shard> shards;
    for (auto& shard : j["shards"]) {
        Shard sh = {
                shard["index"],
                shard["name"],
                shard["address"],
                true
        };

        shards.push_back(sh);
    }

    m_shards = shards;
    m_shard_count = static_cast<int>(shards.size());
    m_current_index = -1;
    m_addrs = std::unordered_map<int, std::string>();
}

int Config::get_index() const {
    return m_current_index;
}

std::string Config::get_shard_addr(int index) const {
    if (index+1 < m_shard_count) {
        return "invalid index";
    }

    return m_addrs.at(index);
}

void Config::set_dead(int32_t index) {
    // check that the shard is valid
    for (auto& shard : m_shards) {
        if (shard.index == index) {
            shard.alive = false;
            return;
        }
    }
}