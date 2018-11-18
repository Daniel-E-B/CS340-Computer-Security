#include <iostream>
#include "blockchain.h"

int main() {
    BlockChain::Chain blockChain("Genesis");
    // mine blocks
    for (int i = 0; i < 8; ++i) {
        if (blockChain.mine(std::to_string(i))) {
            std::cout << "block mined" << std::endl;
        } else {
            std::cout << "failed" << std::endl;
        }
    }
    for (BlockChain::Block B : blockChain.blocks) {
        std::cout << "Block #" << B.blockNo << std::endl;
        std::cout << "Previous hash: " << B.prevHash << std::endl;
        std::cout << "Data: " << B.data << std::endl << "nonce: " << B.nonce << std::endl;
        std::cout << "Timestamp: " << B.timestamp << std::endl << "Mining difficulty: " << B.difficulty << std::endl;
        std::cout << "Hash: " << B.hash() << std::endl;
    }
    return 0;
}