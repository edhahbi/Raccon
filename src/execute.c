#include "execute.h"

static command* cmd;
static buffer* out;
static command_type cmd_type;
static exec_result result;

void detect_cmd_type(){
    arg_type type = cmd->argv[0].type;

    if(type == INTEGER){
        result = INVALID;
        return;
    }

    string* sdc = &cmd->argv[0].arg_value.string;
    
    if(cmd->argc == 1 && memcmp(sdc->ptr,"PING",sdc->size)){
        cmd_type = PING;
    }

}

void exec_cmd(){
    if(cmd_type == PING){
        buffer_push(out,"+PONG\r\n",7);
    }
}

void exec(command* command, buffer* outcoming){   
    cmd = command;
    out = outcoming;

    detect_cmd_type();

    if(result == INVALID){
        //TODO handle command errors here 
    }

    exec_cmd();
}
