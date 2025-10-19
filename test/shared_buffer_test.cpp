#include "../src/SharedBuffer.h"
#include <iostream>

int main() {
    SharedBuffer<uint8_t> buf(1024);
    buf.fill(255);
    std::cout << "buf[0] = " << static_cast<int>(buf.data()[0]) << std::endl;
    return 0;
}
