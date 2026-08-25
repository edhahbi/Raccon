#include "execute.h"

void buffer_push_arg(buffer *buff, token *arg)
{
    switch (arg->type)
    {
    case RESP_BOOLEAN:
        char c = arg->value->b ? '1' : '0';
        buffer_push(buff, sizeof(char), "%c", &c);
        break;

    case RESP_DOUBLE:
        buffer_push(buff, sizeof(double), "%f", &arg->value->d);
        break;

    case RESP_INT64:
        buffer_push(buff, sizeof(i64), "%lld", &arg->value->int64);
        break;

    case RESP_STRING:
        buffer_push(buff, arg->value->str.size, "%s", arg->value->str.ptr);
        break;

    case RESP_OBJECT:
        for (size_t i = 0; i < arg->value->object.size; i++)
        {
            property *prop = &arg->value->object.properties[i];
            buffer_push(buff, prop->field->size, "%s", prop->field->ptr);
            buffer_push(buff, sizeof(char), "%c", " ");
            buffer_push_arg(buff, prop->value);
            if (i != arg->value->object.size - 1)
                buffer_push(buff, sizeof(char), "%c", " ");
        }
        break;

    default:
        LOG_ERROR("unsupported type");
        exit(1);
    }
}

void exec_PING() { buffer_push(out, PONG_LEN, "%s", "+PONG\r\n"); }

dict_tv create_dict_tv(const size_t tv_idx)
{
    if (cmd->argc < 3)
    {
        return get_command_arg(tv_idx);
    }
    else
    {
        size_t prop_size = (cmd->argc - 2) / 2;
        dict_tv tv = create_arg_obj(malloc(sizeof(property) * prop_size), prop_size);
        size_t index = 0;
        for (size_t prop_idx = tv_idx; prop_idx < cmd->argc - 1; prop_idx += 2)
        {
            property p = (property){.field = get_command_arg_string(prop_idx), .value = get_command_arg(prop_idx + 1)};
            tv->value->object.properties[index] = p;
            index++;
        }
        return tv;
    }
}

void exec_SET()
{
    dict_key key = get_command_arg(1);
    dict_tv tv = create_dict_tv(2);
    dict_set(key, tv);
    buffer_push((buffer* const)out, OK_LEN, "%s", "+CMD_OK\r\n");
    LOG_DEBUG("kv pair has been set successfully");
}

void exec_GET()
{
    dict_key key = create_dict_tv(1);
    search_result result = dict_try_get(key);

    if (result.state == NOTFOUND)
    {
        buffer_push((buffer* const)out, NIL_LEN, "%s", "$-1\r\n");
        return;
    }

    buffer_push_arg((buffer* const)out, (dict_tv const)result.value);
}

void exec_DEL()
{
    dict_key key = get_command_arg(1);

    if (dict_try_del(key))
    {
        buffer_push((buffer* const)out, LEN_01, "%s", "+1\r\n");
        return;
    }

    buffer_push((buffer* const)out, LEN_01, "%s", "+0\r\n");
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
    }

    return CMD_ERROR;
}

cmd_state exec(const command* command, const buffer* outcoming)
{
    cmd = command;
    out = outcoming;
    return exec_cmd();
}
