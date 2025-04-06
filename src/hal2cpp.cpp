
#include "hal/hal2cpp.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory.h>
#include <ostream>

uint64_t micros()
{
  static auto start = std::chrono::high_resolution_clock::now();
  auto now = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
}

const size_t bufferlen = 16;
Vec5 buffer[bufferlen];

uint64_t startTime = 0;

extern "C" int hal2cpp_setup()
{
  // memset(buffer, 0, sizeof(buffer));
  startTime = micros();
  return 0;
}

extern "C" void hal2cpp_loop(struct Data* data)
{
  uint64_t now = micros();
  double elapsed = (now - startTime) / 1000000.0;

  data->out.act_pos.x = data->in.cmd_pos.x;
  data->out.act_pos.y = data->in.cmd_pos.y;
  data->out.act_pos.z = data->in.cmd_pos.z;
  data->out.hardware_estop = data->in.virtual_estop;

  auto start = micros();
  auto d = data;
  auto s = sizeof(Data);
  // auto elapsed = micros() - start;

  // Vec5 newPos = buffer[0];
  // for (int i = 0; i < bufferlen - 1; i++) {
  //   buffer[i] = buffer[i + 1];
  // }

  // data->out.act_pos.x = 20 * sin(elapsed);
}

extern "C" void hal2cpp_exit() { }
