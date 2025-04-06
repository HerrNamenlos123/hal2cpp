
#include "hal2cpp.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <ostream>

uint64_t micros() {
    static auto start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
}

extern "C" int hal2cpp_setup() {
    return 0;
}

extern "C" void hal2cpp_loop(struct Data* data) {
    data->out.act_pos.x = data->in.cmd_pos.x;
    data->out.act_pos.y = data->in.cmd_pos.y;
    data->out.act_pos.z = data->in.cmd_pos.z;
    data->out.hardware_estop = data->in.virtual_estop;

    auto start = micros();
    auto d = data;
    auto s = sizeof(Data);
    auto elapsed = micros() - start;

    //data->out.act_pos.x = 187;
}

extern "C" void hal2cpp_exit() {
        
}
