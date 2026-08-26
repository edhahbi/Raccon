#include "execute.h"

void buffer_push_arg(buffer *buff, token *arg)
{
    switch (arg->type)
    {
    case RESP_BOOLEAN:
        const char *c =
            *((bool *)arg->value) ? "1" : "0";

        buffer_push(buff, sizeof(char) + 3, "#%c\r\n", c);
        break;

    case RESP_DOUBLE:
        buffer_push(buff, sizeof(double) + 3, ",%f\r\n", (double *)arg->value);
        break;

    case RESP_INT64:
        buffer_push(buff, sizeof(i64) + 3, ":%lld\r\n", (i64 *)arg->value);
        break;

    case RESP_STRING:
        string *sdc = (string *)arg->value;
        buffer_push(buff, sdc->size + sizeof(i64) + 5, "$%d\r\n%s\r\n", sdc->size, sdc->ptr);
        break;

    case RESP_OBJECT:
        object *obj = (object *)arg->value;
        for (size_t i = 0; i < obj->size; i++)
        {
            property *prop = &obj->properties[i];
            buffer_push_arg(buff, &(token){.type = RESP_STRING, .value = prop->field});
            buffer_push_arg(buff, prop->value);
        }
        break;

    default:
        LOG_ERROR("unsupported type");
        exit(1);
    }
}

void exec_PING() { buffer_push((buffer *const)out, PONG_LEN, "%s", "+PONG\r\n"); }

dict_tv create_dict_tv(const size_t tv_idx)
{
    if (cmd->argc <= 3)
    {
        return get_command_arg(tv_idx);
    }
    else
    {
        size_t prop_size = (cmd->argc - 2) / 2;
        property *props = malloc(sizeof(property) * prop_size);
        size_t index = 0;
        for (size_t prop_idx = tv_idx; prop_idx < cmd->argc - 1; prop_idx += 2)
        {
            property p = (property){
                .field = get_command_arg_string(prop_idx),
                .value = get_command_arg(prop_idx + 1)
            };
            
            props[index] = p;
            index++;
        }
        dict_tv tv = create_arg_obj(props, prop_size);
        props = NULL;
        return tv;
    }
}

void exec_SET()
{
    dict_key key = get_command_arg(1);
    dict_tv tv = create_dict_tv(2);
    dict_set(key, tv);
    buffer_push((buffer *const)out, OK_LEN, "%s", "+OK\r\n");
    LOG_DEBUG("kv pair has been set successfully");
}

void exec_GET()
{
    dict_key key = create_dict_tv(1);
    search_result result = dict_try_get(key);

    if (result.state == NOTFOUND)
    {
        buffer_push((buffer *const)out, NIL_LEN, "%s", "$-1\r\n");
        return;
    }

    buffer_push_arg((buffer *const)out, (dict_tv const)result.value);
}

void exec_DEL()
{
    dict_key key = get_command_arg(1);

    if (dict_try_del(key))
    {
        buffer_push((buffer *const)out, LEN_01, "%s", "+1\r\n");
        return;
    }

    buffer_push((buffer *const)out, LEN_01, "%s", "+0\r\n");
}

cmd_result valid_ping_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (memcmp(sdc->ptr, "PING", sdc->size))
        return (cmd_result){.state = CMD_NOT_CMD};

    if (cmd->argc > 1)
        return (cmd_result){.state = CMD_ERROR};

    return (cmd_result){.state = CMD_OK, .exec_fn = exec_PING};
}

// SET STR_K VAL
cmd_result valid_set_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (memcmp(sdc->ptr, "SET", sdc->size))
        return (cmd_result){.state = CMD_NOT_CMD};

    if (cmd->argc < 3)
        return (cmd_result){.state = CMD_INCOMPLETE, .exec_fn = exec_SET};

    token_type key_type = get_arg_type(1);
    if (key_type != RESP_STRING)
        return (cmd_result){.state = CMD_ERROR};

    if (cmd->argc == 3)
        return (cmd_result){.state = CMD_OK, .exec_fn = exec_SET};

    if (cmd->argc % 2 != 0)
        return (cmd_result){.state = CMD_ERROR};

    for (size_t arg_idx = 2; arg_idx < cmd->argc; arg_idx += 2)
        if (get_arg_type(arg_idx) != RESP_STRING)
            return (cmd_result){.state = CMD_ERROR};

    return (cmd_result){.state = CMD_OK, .exec_fn = exec_SET};
}

// GET KEY
cmd_result valid_get_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (memcmp(sdc->ptr, "GET", sdc->size))
        return (cmd_result){.state = CMD_NOT_CMD};

    if (cmd->argc < 2)
        return (cmd_result){.state = CMD_INCOMPLETE, .exec_fn = exec_GET};

    if (cmd->argc > 2)
        return (cmd_result){.state = CMD_ERROR};

    return (cmd_result){.state = CMD_OK, .exec_fn = exec_GET};
}

// DEL KEY
cmd_result valid_del_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (memcmp(sdc->ptr, "DEL", sdc->size))
        return (cmd_result){.state = CMD_NOT_CMD};

    if (cmd->argc < 2)
        return (cmd_result){.state = CMD_INCOMPLETE, .exec_fn = exec_DEL};

    if (cmd->argc > 2)
        return (cmd_result){.state = CMD_ERROR};

    return (cmd_result){.state = CMD_OK, .exec_fn = exec_DEL};
}

cmd_state exec_cmd()
{
    if (!valid_command_begining())
        return CMD_ERROR;

    for (size_t i = 0; i < CMD_NUM; i++)
    {
        cmd_result res = func_arr[i]();
        if (res.state == CMD_OK)
        {
            res.exec_fn();
            return CMD_OK;
        }
        else if (res.state == CMD_ERROR)
            return CMD_ERROR;
    }

    return CMD_ERROR;
}

cmd_state exec(const command *command, const buffer *outcoming)
{
    cmd = command;
    out = outcoming;
    return exec_cmd();
}
