#include <string>
#include <chrono>
#include <iostream>
#include "blockchain.h"

using namespace BlockChain;


BlockChain::Block::Block(int prevBlockNo, std::string prevHash_, std::string data_) {
    blockNo = ++prevBlockNo;
    data = data_;
    prevHash = prevHash_;
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    difficulty = 5; // arbitrary for testing
    nonce = 0;

    while (valid() == false) {
        hash = computeHash();
        ++nonce;
        std::cout << hash << std::endl;
    }

}