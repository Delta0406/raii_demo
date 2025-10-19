//
// Created by wnc on 2025/10/19.
//

#include "../src/UniqueBuffer.h"
#include <iostream>

struct ControlInfo {
    int id;
    float value;
};

int main() {
    // 1. 图像数据
    UniqueBuffer<uint8_t> imgBuf(640*480*3);
    imgBuf.fill(128);
    std::cout << "imgBuf[0] = " << static_cast<int>(imgBuf[0]) << std::endl;

    // 2. 控制信息
    UniqueBuffer<ControlInfo> ctrlBuf(5);
    ctrlBuf[0] = {42, 3.14f};
    std::cout << "ctrlBuf[0].id=" << ctrlBuf[0].id
              << ", value=" << ctrlBuf[0].value << std::endl;

    // 3. 移动语义
    UniqueBuffer<uint8_t> movedBuf = std::move(imgBuf);
    std::cout << "movedBuf[0] = " << static_cast<int>(movedBuf[0]) << std::endl;

    return 0; // 离开作用域自动释放内存
}
