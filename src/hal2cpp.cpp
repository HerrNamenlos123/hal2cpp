
#include "hal2cpp.h"

extern "C" {

    int hal2cpp_setup() {
        return 0;
    }

    void hal2cpp_loop(struct Data* data) {
        data->out.act_pos.x = data->in.cmd_pos.x;
        data->out.act_pos.y = data->in.cmd_pos.y;
        data->out.act_pos.z = data->in.cmd_pos.z;
        data->out.hardware_estop = data->in.virtual_estop;
    }

    void hal2cpp_exit() {
        
    }

}
