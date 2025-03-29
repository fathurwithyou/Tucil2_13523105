#include "ImageCompressor.hpp"
#include <sys/resource.h>
#include <iostream>

void increaseStackSize() {
    const rlim_t kStackSize = 120 * 1024 * 1024; // 64 MB stack
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
        if (rl.rlim_cur < kStackSize) {
            rl.rlim_cur = kStackSize;
            if (setrlimit(RLIMIT_STACK, &rl) != 0) {
                std::cerr << "Error: Unable to increase stack size." << std::endl;
            }
        }
    } else {
        std::cerr << "Error: getrlimit() failed." << std::endl;
    }
}

int main() {
    increaseStackSize();
    ImageCompressor compressor;
    compressor.run();
    return 0;
}
