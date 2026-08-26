#include "dispatch.h"

void handle_parsing_error(conn_t *connection)
{
    buffer_push(&connection->_conn_ctx.outcoming, PARSER_ERR_MSG_LEN, "%s", "-Err Parsing Error\r\n", RESP_STRING);
}

void handle_execution_error(conn_t *connection)
{
    buffer_push(&connection->_conn_ctx.outcoming, EXCUTOR_ERR_MSG_LEN, "%s", "-Err Execution Error\r\n", RESP_STRING);
}

void dispatch(conn_t *connection)
{
    parser_result parser_result = parse(&connection->_conn_ctx.incoming);

    if (parser_result.state == PARSER_ERROR)
    {
        command_cleanup(&parser_result.cmd);
        handle_parsing_error(connection);
        return;
    }

    if (parser_result.state == PARSER_INCOMPLETE)
    {
        command_cleanup(&parser_result.cmd);
        return;
    }

    cmd_state exec_result = exec(&parser_result.cmd, &connection->_conn_ctx.outcoming);

    if (exec_result == CMD_ERROR)
    {
        command_cleanup(&parser_result.cmd);
        handle_execution_error(connection);
    }
    else if (exec_result == CMD_OK)
        command_dispose(&parser_result.cmd);
}
