#include <string>
#include <chrono>
#include <cmath>
#include <iostream>
#include "blockchain.h"

bool BlockChain::Block::valid() {
    for (int i = 0; i < difficulty; ++i) {
        if (hash()[i] != 'b') {
            return false;
        }
    }
    return true;
}

BlockChain::Block::Block(uint64_t prevBlockNo, std::string prevHash_, std::string data_, int diff) {
    blockNo = prevBlockNo + 1;
    data = data_;
    prevHash = prevHash_;
    nonce = 0;
    difficulty = diff;
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    while (!valid() && nonce < 4294967295) {
        ++nonce;
    }
}

std::string BlockChain::Block::hash() {
    std::string compoundData =
            std::to_string(nonce) + std::to_string(timestamp) + data + prevHash + std::to_string(blockNo);
    std::vector<unsigned char> hasher(picosha2::k_digest_size);
    picosha2::hash256(compoundData.begin(), compoundData.end(), hasher.begin(), hasher.end());

    return picosha2::bytes_to_hex_string(hasher.begin(), hasher.end());
}

BlockChain::Chain::Chain(std::string genesisData) {
    blocks.push_back(Block(0, "", genesisData, 0));
}

bool BlockChain::Chain::mine(std::string data) {
    BlockChain::Block prevBlock = blocks[blocks.size() - 1];
    // simple calculation of mining difficulty for ~30 sec / blk (2880/day):
    int diff = prevBlock.difficulty;
    if (blocks.size() >= 2) {
        long long int timeDiff = prevBlock.timestamp - blocks[blocks.size() - 2].timestamp;
        std::cout << "tdiff = " << timeDiff << std::endl;
        // for some reason the time values are 1 order of magnitude off?
        if (timeDiff < 25000000) {
            // increase by 1 if last blk took < 25 secs
            ++diff;
        } else if (timeDiff > 35000000) {
            // decrease by 1 if last blk took > 35 secs
            --diff;
        }
    }
    std::cout << "diff = " << diff << std::endl;
    BlockChain::Block B(prevBlock.blockNo, prevBlock.hash(), data, diff);

    if (B.valid()) {
        blocks.push_back(B);
        return true;
    }
    return false;
}