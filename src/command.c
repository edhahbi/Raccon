#include "command.h"

inline void push_arg(command* command, arg arg){
    command->argv[command->argc++] = arg;
}

arg create_arg(arg_type arg_type, void* arg_val, size_t arg_size){
    arg a = (arg){.type = arg_type};
    switch (arg_type)
    {
    case INTEGER:
        memcpy(&a.arg_value.integer, arg_val, sizeof(u32));
        break;
    
    case STRING:
        a.arg_value.string = sdc_init(arg_val,arg_size);
        break;

    default:
        LOG_ERROR("Invalid arg type");
        exit(EXIT_FAILURE);
        break;
    }
    return a;
}
