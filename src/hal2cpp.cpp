
#include "hal/hal2cpp.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory.h>
#include <ostream>

#include "TinyStd.hpp"

#include <sockpp/socket.h>
#include <sockpp/udp_socket.h>

uint64_t micros()
{
  static auto start = std::chrono::high_resolution_clock::now();
  auto now = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
}

const size_t bufferlen = 16;
Vec5 buffer[bufferlen];
sockpp::udp_socket sock;

uint64_t startTime = 0;
uint64_t prevTime = 0;

ts::Arena arena;

extern "C" int hal2cpp_setup()
{
  // memset(buffer, 0, sizeof(buffer));
  startTime = micros();

  if (!sock.connect(sockpp::inet_address("localhost", 12345))) {
    std::cerr << "Error creating socket\n";
    return 1;
  }

  arena = ts::Arena::create();

  return 0;
}

double xvel = 0;
double xpos = 0;

extern "C" void hal2cpp_loop(struct Data* data)
{
  arena.clearAndReinit();

  uint64_t now = micros();
  double elapsed = (now - startTime) / 1000000.0;
  double dt = (now - prevTime) / 1000000.0;
  prevTime = now;

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

  if (data->in_x_pwmgen_enable) {
    xvel += data->in_x_pwmgen_value * dt;
  }
  xvel *= 0.9;
  xpos += xvel * dt;

  data->out_x_encoder_raw = xpos;
  data->out_x_encoder_pos = xpos;
  // data->out_x_encoder_vel = xpos;

  auto msg = ts::format(arena, "PWM enable={}, PWM value={}", data->in_x_pwmgen_enable, (float)data->in_x_pwmgen_value);
  sock.send(msg.c_str(arena), msg.length);
}

extern "C" void hal2cpp_exit() { arena.free(); }
