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
    a.value = malloc(sizeof(arg_value));
    switch (arg_type)
    {
    
    case RESP_INT64:
        a.value->int64 = *(i64*)arg_val;
        break;

    case RESP_BOOLEAN:
        a.value->b = *(bool*)arg_val;
        break;
    
    case RESP_STRING:
        a.value->str = sdc_init(arg_val,arg_size);
        break;

    case RESP_OBJECT:
        a.value->object.size = arg_size;
        a.value->object.properties = arg_val;
        break;

    default:
        LOG_ERROR("Invalid arg type");
        exit(EXIT_FAILURE);
    }
    return a;
}

arg* create_arg_obj(const property* const val, size_t size){
    arg* a = malloc(sizeof(arg));
    a->value = malloc(sizeof(arg_value));

    a->type = RESP_OBJECT;
    a->value->object.size = size;
    a->value->object.properties = val;

    return a;
}

void command_cleanup(command* cmd){

    for (size_t i = 01; i < cmd->argc; i++){
        switch (cmd->argv[i].type)
        {
        case RESP_INT64 || RESP_BOOLEAN:
            free(cmd->argv[i].value);            
            break;
        
        case RESP_STRING:
            sdc_free(&cmd->argv[i].value->str);
            break;

        default:
            LOG_ERROR("Unhandeled type");
            exit(1);
        }
    }
    free(cmd->argv);
    cmd->argc = 0;
}

// dispose of the first command 
void command_dispose(command* cmd){
    const arg* temp = cmd->argv;
    cmd->argv++;
    cmd->argc--;
    sdc_free(&temp->value->str);
}
