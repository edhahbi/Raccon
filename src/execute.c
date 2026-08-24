#include "execute.h"

void buffer_push_arg(buffer *buff, arg *arg)
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
        property *props = malloc(sizeof(property) * prop_size);
        size_t index = 0;
        for (size_t prop_idx = tv_idx; prop_idx < cmd->argc - 1; prop_idx += 2)
        {
            property p = (property){.field = get_command_arg_string(prop_idx), .value = get_command_arg(prop_idx + 1)};
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
    buffer_push(out, OK_LEN, "%s", "+OK\r\n");
    LOG_DEBUG("kv pair has been set successfully");
}

void exec_GET()
{
    dict_key key = create_dict_tv(1);
    search_result result = dict_try_get(key);

    if (result.state == NOTFOUND)
    {
        buffer_push(out, NIL_LEN, "%s", "$-1\r\n");
        return;
    }

    buffer_push_arg(out, (dict_tv const)result.value);
}

void exec_DEL()
{
    dict_key key = get_command_arg(1);

    if (dict_try_del(key))
    {
        buffer_push(out, LEN_01, "%s", "+1\r\n");
        return;
    }

    buffer_push(out, LEN_01, "%s", "+0\r\n");
}

bool valid_ping_cmd()
{
    const string *sdc = get_command_arg_string(0);
    return cmd->argc == 1 && memcmp(sdc->ptr, "PING", sdc->size);
}

// SET STR_K VAL
// SET STR_K VAL
bool valid_set_cmd()
{
    const string *sdc = get_command_arg_string(0);
    if (cmd->argc < 3 || memcmp(sdc->ptr, "SET", sdc->size) != 0)
        return false;

    arg_type key_type = get_arg_type(1);
    if (key_type != RESP_STRING)
        return false;

    if (cmd->argc == 3)
        return true;

    if (cmd->argc % 2 != 0)
        return false;

    for (size_t arg_idx = 2; arg_idx < cmd->argc; arg_idx += 2)
        if (get_arg_type(arg_idx) != RESP_STRING)
            return false;

    return true;
}

// GET KEY
bool valid_get_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (cmd->argc != 2 || memcmp(sdc->ptr, "GET", sdc->size))
        return false;

    return true;
}

// DEL KEY
bool valid_del_cmd()
{
    const string *sdc = get_command_arg_string(0);

    if (cmd->argc != 2 || memcmp(sdc->ptr, "DEL", sdc->size))
        return false;

    return true;
}

static inline bool valid_command_begining() { return get_arg_type(0) == RESP_STRING; }

state exec_cmd()
{
    if (!valid_command_begining())
        return ERROR;

    if (valid_ping_cmd())
    {
        exec_PING();
        return OK;
    }

    if (valid_set_cmd())
    {
        exec_SET();
        return OK;
    }

    if (valid_get_cmd())
    {
        exec_GET();
        return OK;
    }

    if (valid_del_cmd())
    {
        exec_DEL();
        return OK;
    }

    return ERROR;
}

state exec(const command *command, const buffer *outcoming)
{
    cmd = command;
    out = outcoming;
    return exec_cmd();
}
