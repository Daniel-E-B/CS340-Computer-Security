#include <cstdint>
#include <vector>
#include <string>
#include "picosha2.h"

namespace BlockChain {

    class Block {
    public:
        int blockNo;
        uint32_t nonce;
        long long int timestamp; // µseconds since epoch
        std::string data;
        std::string prevHash;
        std::string hash;
        int difficulty; // number of b's at the beginning of hash. Should be set according to time between last and current block

        Block(int prevBlockNo, std::string prevHash_, std::string data_);

    private:
        bool valid() {
            for (int i = 0; i < difficulty; ++i) {
                if (hash[i] != 'b') {
                    return false;
                }
            }
            return true;
        }

        std::string computeHash() {
            std::string compoundData =
                    std::to_string(nonce) + std::to_string(timestamp) + data + prevHash + std::to_string(blockNo);
            std::vector<unsigned char> hasher(picosha2::k_digest_size);
            picosha2::hash256(compoundData.begin(), compoundData.end(), hasher.begin(), hasher.end());

            return picosha2::bytes_to_hex_string(hasher.begin(), hasher.end());
        }
    };

    class Chain {
    public:
        std::vector<Block> blocks;
    };
}