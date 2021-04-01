//
// Created by eemil on 3/30/21.
//

#ifndef DKVCPP_CONFIG_HPP
#define DKVCPP_CONFIG_HPP

#include <unordered_map>
#include <string>
#include <vector>

struct Shard {
    int index;
    std::string name;
    std::string addr;
};

typedef unsigned int uint32;
class Config {
public:
    explicit Config(const std::string& conf_filepath);
    [[nodiscard]] uint32 get_key_shard(const std::string& key) const;

    bool verify_shards(const std::vector<Shard>& shards, const std::string& cur_name);
    ~Config() = default;
private:
    uint32 m_shard_count;
    uint32 m_current_index;
    std::unordered_map<int, std::string> m_addrs;
};


#endif //DKVCPP_CONFIG_HPP
