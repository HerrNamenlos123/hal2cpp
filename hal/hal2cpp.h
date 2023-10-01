
#ifndef HAL2CPP_H
#define HAL2CPP_H

// This file must compile in C++ and plain C, 
// hence no C++ features in this file.

#ifdef __cplusplus
extern "C" {
#endif

    struct Vec5 {
        float x;
        float y;
        float z;
        float a;
        float b;
    };

    struct DataIn {
        bool virtual_estop;
        bool enable;
        struct Vec5 cmd_pos; 
        struct Vec5 cmd_vel; 
    };

    struct DataOut {
        bool hardware_estop;
        struct Vec5 act_pos; 
        struct Vec5 act_vel; 
    };

    struct Data {
        struct DataIn in;
        struct DataOut out;
    };

#ifdef __cplusplus
}
#endif

#endif // HAL2CPP_H