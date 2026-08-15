#include "execution_ctx.h"

void excution_ctx_init(execution_ctx* execution_ctx, command* cmd){
    execution_ctx->cmd = cmd;
}
