#include "command.h"

void command_init(command* command){
    command->argc = 0;
    command->argv = NULL;
}

void push_arg(command* command, token arg){
    command->argv = realloc(command->argv,(command->argc+1) * sizeof(arg));
    command->argv[command->argc++] = arg;
}

token create_token(token_type type, void* val, size_t size){
    token a = (token){.type = type};
    a.value = malloc(size);
    switch (type)
    {
    
    case RESP_INT64 || RESP_BOOLEAN || RESP_DOUBLE:
        a.value = val;
        break;


    case RESP_STRING:
        string* sdc = a.value;
        *sdc = sdc_init(val,size);
        break;

    case RESP_OBJECT:
        object* obj = a.value;
        obj->size = size;
        obj->properties = val;
        break;

    default:
        LOG_ERROR("Invalid arg type");
        exit(EXIT_FAILURE);
    }
    return a;
}

token* create_arg_obj(const property* const val, size_t size){
    token* a = malloc(sizeof(token));
    a->value = malloc(sizeof(object));

    a->type = RESP_OBJECT;

    object* obj = a->value;
    obj->size = size;
    obj->properties = val;

    return a;
}

void command_cleanup(command* cmd){

    for (size_t i = 0; i < cmd->argc; i++){
        switch (cmd->argv[i].type)
        {
        case RESP_INT64 || RESP_BOOLEAN || RESP_DOUBLE:
            free(cmd->argv[i].value);            
            break;
        
        case RESP_STRING:
            sdc_free(cmd->argv[i].value);
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
    const token* temp = cmd->argv;
    cmd->argv++;
    cmd->argc--;
    sdc_free(temp->value);
}
