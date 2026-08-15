#include "execute.h"

static buffer* out;
static execution_ctx exec_ctx;

static inline string* get_command_arg_string(const size_t idx){
    return &exec_ctx.cmd->argv[idx].value.string;
}

static inline size_t* get_command_arg_integer(const size_t idx){
    return &exec_ctx.cmd->argv[idx].value.integer;
}

static inline arg_type get_arg_type(const size_t idx){
    return exec_ctx.cmd->argv[idx].type;
}

void detect_cmd_type(){
    arg_type type = get_arg_type(0);
    const string* sdc = get_command_arg_string(0);

    if(type == RESP_INTEGER){
        exec_ctx.result = INVALID;
        return;
    }
    
    if(exec_ctx.cmd->argc == 1 && memcmp(sdc->ptr,"PING",sdc->size)){
        exec_ctx.cmd_type = PING;
    }

    // TODO complete SET GET DEL commands
}

void exec_cmd(){
    if(exec_ctx.cmd_type == PING){
        buffer_push(out,"+PONG\r\n",7);
    }
}

exec_result exec(command* cmd, buffer* outcoming){

    excution_ctx_init(&exec_ctx, cmd);
    out = outcoming;

    detect_cmd_type();

    if(exec_ctx.result == INVALID){
        return INVALID;
    }

    exec_cmd();

    return exec_ctx.result;
}
