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

    // this is changed by the heartbeat checks performed every 15 seconds
    bool alive;
};

typedef unsigned int uint32;
class Config {
public:
    explicit Config(const std::string& path);
    [[nodiscard]] uint32 get_key_shard(const std::string& key) const;

    bool verify_shards(const std::string& cur_name);
    std::vector<Shard> get_shards() const {
        return m_shards;
    }

    // make shard dead if response to heartbeat is invalid
    void set_dead(int32_t index);

    int get_index() const;
    std::string get_shard_addr(int index) const;
    ~Config() = default;
private:
    uint32 m_shard_count;
    uint32 m_current_index;

    std::vector<Shard> m_shards;
    std::unordered_map<int, std::string> m_addrs;
};


#endif //DKVCPP_CONFIG_HPP
