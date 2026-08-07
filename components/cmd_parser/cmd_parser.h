#pragma once 
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Enum used for parsing commands
typedef enum
{
    CMD_GET_TEMP,
    CMD_GET_HUM,
    CMD_SET_INTERVAL,
    CMD_LOG_DUMP, 
    CMD_UNKNOWN,
}e_cmd_parser_commands;

// Struct type used to store parsed commands + optional parameters (e.g. used for setter commands)
// e.g.: SET_INTERVAL 5000
typedef struct 
{
    /* data */
    e_cmd_parser_commands parsed_cmd;
    int32_t               opt_param;
}t_cmd_parser_operational_data;

// Parses a command string and returns the decoded command and optional parameter.
t_cmd_parser_operational_data cmd_parser_parse_user_cmd(const char* user_command);

#ifdef __cplusplus
}
#endif