#include "execute.h"

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
        return false;
    

    arg_type key_type = get_arg_type(1);
    if (key_type != RESP_STRING)
        return false;
    

    if (exec_ctx.cmd->argc == 3)
        return true;
    

    if (exec_ctx.cmd->argc % 2 != 0)
        return false;
    

    for (size_t arg_idx = 2; arg_idx < exec_ctx.cmd->argc; arg_idx += 2)
        if (get_arg_type(arg_idx) != RESP_STRING)
            return false;

    return true;
}

// GET KEY
bool valid_get_cmd(){
    const string *sdc = get_command_arg_string(0);

    if(exec_ctx.cmd->argc != 2 || memcmp(sdc->ptr,"GET",sdc->size))
        return false;

    return true;
}

// DEL KEY
bool valid_del_cmd(){
    const string* sdc = get_command_arg_string(0);

    if(exec_ctx.cmd->argc != 2 || memcmp(sdc->ptr, "DEL", sdc->size))
        return false;
    
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

    if(valid_get_cmd()){
        exec_ctx.cmd_type = GET;
        return;
    }

    if(valid_del_cmd()){
        exec_ctx.cmd_type = DEL;
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
        return (dict_key){.len = sizeof(size_t), .value = get_command_arg_integer(key_idx)};

    case RESP_STRING:
        string str = *get_command_arg_string(key_idx);
        return (dict_key){.len = str.size, .value = str.ptr};

    default:
        LOG_ERROR("Unhandeled Type");
        exit(1);
    }
}

dict_tv create_simple_tv_cmd(size_t tv_idx){
    switch (get_arg_type(tv_idx)){

    case RESP_INTEGER:
        return (dict_tv){.type = UINT, .value = (dict_value*) get_command_arg_integer(tv_idx)};

    case RESP_STRING:
        return (dict_tv){.type = STRING, .value = (dict_value*) get_command_arg_string(tv_idx)};
    
    default:
        LOG_ERROR("Unhandeled Type");
        exit(1);
    }
}

dict_tv create_tv_cmd(size_t tv_idx)
{
    if(exec_ctx.cmd->argc == 3){
        return create_simple_tv_cmd(tv_idx);
    }else{
        dict_tv tv;
        size_t size = (exec_ctx.cmd->argc - 2) / 2;
        property *properties = malloc(sizeof(property) * size);
        for (size_t prop = 0; prop < size; prop++){
            properties[prop] = (property){
                .field = get_command_arg_string(2*prop+2),
                .value = create_simple_tv_cmd(2*prop+3)
            };
        }
        tv.type = OBJECT;
        tv.value = (dict_value*) create_object(size,properties);
        return tv;
    }
}

void exec_PING(){ buffer_push(out, "+PONG\r\n", PONG_LEN);}

void exec_SET()
{
    dict_key key = create_key_cmd(1);
    dict_tv tv = create_tv_cmd(2); 
    dict_set(key, tv);
    buffer_push(out, "+OK\r\n",OK_LEN);
    LOG_DEBUG("kv pair has been set successfully");
}


string from_dict_value_to_string(const dict_tv* tv){
    switch (tv->type)
    {
    case UINT:
        return from_int_to_srt(tv->value->uint64);
    
    case STRING:
        return tv->value->str;

    case OBJECT:
        const object* obj = &tv->value->object;
        string result = sdc_init(NULL,0);
        string sdc;
        for (size_t prop_idx = 0; prop_idx < obj->size; prop_idx++){
            sdc_merge(&result, obj->properties[prop_idx].field);
            sdc_push(&result,' ');
            sdc = from_dict_value_to_string(&obj->properties[prop_idx].value);
            sdc_merge(&result, &sdc);
            sdc_free(&sdc);
            if(prop_idx != obj->size-1){
                sdc_push(&result,' ');
            }
        }
        return result;
    
    default:
        LOG_ERROR("unhandeled type");
        exit(1);
    }
}

void exec_GET(){
    dict_key key = create_key_cmd(1);
    search_result result = dict_try_get(key);
    
    if(result.state == NOTFOUND){
        buffer_push(out,"$-1\r\n",NIL_LEN);
        return;
    }
    
    string value = from_dict_value_to_string((dict_tv*)result.value);
    
    buffer_push(out,value.ptr,value.size);

    sdc_free(&value);
}

void exec_DEL(){
    dict_key key = create_key_cmd(1);

    if(dict_try_del(key)){
        buffer_push(out,"+1\r\n",INTEGER_LEN);
        return;
    }
    
    buffer_push(out,"+0\r\n",INTEGER_LEN);
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

    case GET:
        exec_GET();
        break;
    
    case DEL:
        exec_DEL();
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
        return INVALID;
    
    exec_cmd();

    return exec_ctx.result;
}
