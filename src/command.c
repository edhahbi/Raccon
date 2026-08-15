#include "command.h"

void command_init(command* command){
    command->argv = malloc(sizeof(arg));
    command->argc = 0;
}

inline void push_arg(command* command, arg arg){
    command->argc++;
    command->argv = realloc(command->argv,command->argc * sizeof(arg));
    command->argv[command->argc] = arg;
}

arg create_arg(arg_type arg_type, void* arg_val, size_t arg_size){
    arg a = (arg){.type = arg_type};
    switch (arg_type)
    {
    case RESP_INTEGER:
        memcpy(&a.value.integer, arg_val, sizeof(u32));
        break;
    
    case RESP_STRING:
        a.value.string = sdc_init(arg_val,arg_size);
        break;

    default:
        LOG_ERROR("Invalid arg type");
        exit(EXIT_FAILURE);
    }
    return a;
}
