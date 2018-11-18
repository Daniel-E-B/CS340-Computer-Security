#include <cstdint>
#include <vector>
#include <string>
#include "picosha2.h"

/*
 * TODO: set mining difficulty in Chain.mine, according to time differential between last several blks:
 *      for now, to keep it simple, if the time between the last 2 < 0.5 min, incr diff, if > 1.5, decr diff
*/

namespace BlockChain {

    class Block {
    public:
        uint64_t blockNo;
        uint32_t nonce;
        long long int timestamp; // µseconds since epoch
        std::string data;
        std::string prevHash;
        int difficulty; // number of b's at the beginning of hash.

        Block(uint64_t prevBlockNo, std::string prevHash_, std::string data_, int diff);
        bool valid();
        std::string hash();

    };

    class Chain {
    public:
        std::vector<Block> blocks;

        Chain(std::string genesisData);

        bool mine(std::string data); // Simple mining function. Returns true if valid block has been added.
    };
}