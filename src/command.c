#include "command.h"

void command_init(command* command){
    command->argc = 0;
    command->argv = NULL;
}

void push_arg(command* command, arg arg){
    command->argv = realloc(command->argv,(command->argc+1) * sizeof(arg));
    command->argv[command->argc++] = arg;
}

arg create_arg(arg_type arg_type, void* arg_val, size_t arg_size){
    arg a = (arg){.type = arg_type};
    switch (arg_type)
    {
    case RESP_INTEGER:
        a.value.integer = *((size_t*)arg_val);
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

void command_cleanup(command* cmd){
    free(cmd->argv);
    cmd->argc = 0;
}
