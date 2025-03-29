#include <sys/resource.h>

#include <iostream>

#include "ImageCompressor.hpp"

/* Increase stack size to avoid depth recursion limit exceeded
 */
void increaseStackSize() {
  const rlim_t kStackSize = 128 * 1024 * 1024;  // 128 MB Stack
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
