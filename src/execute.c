#include "execute.h"

static buffer *out;
static execution_ctx exec_ctx;

static inline string *get_command_arg_string(const size_t idx)
{
    return &exec_ctx.cmd->argv[idx].value.string;
}

static inline size_t *get_command_arg_integer(const size_t idx)
{
    return &exec_ctx.cmd->argv[idx].value.integer;
}

static inline arg_type get_arg_type(const size_t idx)
{
    return exec_ctx.cmd->argv[idx].type;
}

bool valid_ping_cmd()
{
    const string *sdc = get_command_arg_string(0);
    return exec_ctx.cmd->argc == 1 && memcmp(sdc->ptr, "PING", sdc->size);
}

// SET STR_K VAL
// SET STR_K STR VAL STR VAL ...
bool valid_set_cmd()
{
    const string *sdc = get_command_arg_string(0);
    if (exec_ctx.cmd->argc < 3 || memcmp(sdc->ptr, "SET", sdc->size) != 0)
    {
        return false;
    }

    arg_type key_type = get_arg_type(1);
    if (key_type != RESP_STRING)
    {
        return false;
    }

    if (exec_ctx.cmd->argc == 3)
    {
        return true;
    }

    if (exec_ctx.cmd->argc % 2 != 0)
    {
        return false;
    }

    for (size_t arg_idx = 3; arg_idx < exec_ctx.cmd->argc; arg_idx += 2)
    {
        if (get_arg_type(arg_idx - 1) != RESP_STRING)
            return false;
    }

    return true;
}

static inline bool valid_command_begining() { return get_arg_type(0) == RESP_INTEGER ? false : true; }
void detect_cmd_type()
{
    if (!valid_command_begining())
    {
        exec_ctx.result = INVALID;
        return;
    }

    if (valid_ping_cmd())
    {
        exec_ctx.cmd_type = PING;
        return;
    }

    if (valid_set_cmd())
    {
        exec_ctx.cmd_type = SET;
        return;
    }

    exec_ctx.result = INVALID;
}

dict_key create_key_cmd(size_t key_idx)
{
    arg_type type = get_arg_type(key_idx);
    switch (type)
    {
    case RESP_INTEGER:
        return (dict_key){.len = sizeof(size_t), .value = get_command_arg_integer(1)};

    case RESP_STRING:
        string str = *get_command_arg_string(key_idx);
        return (dict_key){.len = str.size, .value = str.ptr};

    default:
        LOG_ERROR("Unhandeled Type");
        exit(1);
    }
}

dict_tv create_tv_cmd(size_t tv_idx)
{
    dict_tv tv;
    if(exec_ctx.cmd->argc == 3){
        switch (get_arg_type(tv_idx)){

        case RESP_INTEGER:
            tv.type = UINT;
            tv.value.uint64 = *get_command_arg_integer(tv_idx);
            break;

        case RESP_STRING:
            tv.type = STRING;
            tv.value.str = *get_command_arg_string(tv_idx);
            break;
        
        default:
            LOG_ERROR("Unhandeled Type");
            exit(1);
        }
    }else{
        size_t size = (exec_ctx.cmd->argc - 2) / 2;
        property *properties = malloc(sizeof(property) * size);
        for (size_t i = tv_idx+1; i < size; i+=2){
            properties[i] = (property){
                .field = *get_command_arg_string(i),
                .value = create_tv_cmd(i-1)
            };
        }
        tv.type = OBJECT;
        tv.value.object = create_object(size,properties);
    }
    return tv;
}

void exec_PING()
{
    buffer_push(out, "+PONG\r\n", PONG_LEN);
}

void exec_SET()
{
    dict_key key = create_key_cmd(1);
    dict_tv tv = create_tv_cmd(2); 
    dict_set(key, tv);
    buffer_push(out, "+OK\r\n",OK_LEN);
    LOG_DEBUG("kv pair has been set successfully");
}

void exec_cmd()
{
    switch (exec_ctx.cmd_type)
    {
    case PING:
        exec_PING();
        break;

    case SET:
        exec_SET();
        break;
    default:
        exec_ctx.result = INVALID;
        break;
    }
}

exec_result exec(command *cmd, buffer *outcoming)
{

    excution_ctx_init(&exec_ctx, cmd);
    out = outcoming;

    detect_cmd_type();

    if (exec_ctx.result == INVALID)
    {
        return INVALID;
    }

    exec_cmd();

    return exec_ctx.result;
}
