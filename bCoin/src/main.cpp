#include "blockchain.h"

int main() {
    BlockChain::Block A(1, "", "block 1");
    BlockChain::Block B(A.blockNo, A.hash, "ree");
    return 0;
}