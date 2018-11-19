#include <string>
#include <mutex>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include "blockchain.h"

bool BlockChain::Block::valid() {
    for (int i = 0; i < difficulty; ++i) {
        if (hash(nonce)[i] != 'b') {
            return false;
        }
    }
    return true;
}

bool BlockChain::Block::valid(unsigned int nonce_) {
    for (int i = 0; i < difficulty; ++i) {
        if (hash(nonce_)[i] != 'b') {
            return false;
        }
    }
    return true;
}

std::string BlockChain::Block::hash(unsigned int nonce_) {
    std::string compoundData =
            std::to_string(nonce_) + std::to_string(timestamp) + data + prevHash + std::to_string(blockNo);
    std::vector<unsigned char> hasher(picosha2::k_digest_size);
    picosha2::hash256(compoundData.begin(), compoundData.end(), hasher.begin(), hasher.end());

    return picosha2::bytes_to_hex_string(hasher.begin(), hasher.end());
}

std::string BlockChain::Block::hash() {
    return hash(nonce);
}

void BlockChain::Block::mine(int numThreads, unsigned int startingNonce, std::mutex &mut, bool &stop) {
    bool localStop = false;
    while (!valid(startingNonce) && startingNonce < 4294967295 && !localStop) {
        startingNonce += numThreads;
        mut.lock();
        localStop = stop;
        mut.unlock();
    }
    if (valid(startingNonce)) {
        mut.lock();
        stop = true;
        nonce = startingNonce;
        mut.unlock();
    }
}

BlockChain::Block::Block(uint64_t prevBlockNo, std::string prevHash_, std::string data_, int diff, int numThreads) {
    blockNo = prevBlockNo + 1;
    data = data_;
    prevHash = prevHash_;
    nonce = 0;
    difficulty = diff;
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    std::mutex m;
    bool stop = false;
    std::vector<std::thread> ths;
    for (unsigned int i = 0; i < numThreads; ++i) {
        ths.push_back(std::thread(&Block::mine, this, numThreads, i, std::ref(m), std::ref(stop)));
    }
    while (!stop) { // hopefully i dont need to mutex this part?

    }
    for (auto &th : ths) {
        th.join();
    }
}

BlockChain::Chain::Chain(std::string genesisData) {
    blocks.push_back(Block(0, "", genesisData, 0, 1));
}

bool BlockChain::Chain::mine(std::string data) {
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 1;
    }
    std::cout << "mining on " << numThreads << " threads" << std::endl;
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
    BlockChain::Block B(prevBlock.blockNo, prevBlock.hash(), data, diff, numThreads);

    if (B.valid()) {
        blocks.push_back(B);
        return true;
    }
    return false;
}