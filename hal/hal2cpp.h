
#ifndef HAL2CPP_H
#define HAL2CPP_H

// This file must compile in C++ and plain C, 
// hence no C++ features in this file.

// For performance reasons, we do not do proper serialization here.
// The entire class is serialized via a memory dump.
// This will only work if both the LinuxCNC PC and the receiver share their endianness.
// All Arduinos and Intel-based x86 CPUs are little endian.

#ifdef __cplusplus
extern "C" {
#endif

    struct Vec5 {
        float x;
        float y;
        float z;
        float a;
        float b;
    } __attribute__((packed));

    struct DataIn {
        bool virtual_estop;
        bool enable;
        struct Vec5 cmd_pos; 
        struct Vec5 cmd_vel;
    } __attribute__((packed));

    struct DataOut {
        bool hardware_estop;
        struct Vec5 act_pos; 
        struct Vec5 act_vel; 
    } __attribute__((packed));

    struct Data {
        struct DataIn in;
        struct DataOut out;
    } __attribute__((packed));

#ifdef __cplusplus
}
#endif

#endif // HAL2CPP_H
