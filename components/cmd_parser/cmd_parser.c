// System includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Prj includes
#include "cmd_parser.h"

// Local defines

// Number of potential commands -> to be increased if more commands are meant to be added
#define NUM_OF_POSSIBLE_CMDS 4u

// Size of biggest command sent by user (SET_INTERVAL + 1 for '\0')
#define BIGGEST_USER_COMMAND 13u

// Optional data size determined by int32_t max number (2147483647) - 10 digits + 1 for sign + 1 for \0
#define BIGGEST_OPT_DATA_SIZE 12u

// Local typedefs

// Local structure used to map certein strings received from user with system enum
typedef struct 
{
    /* data */
    char*                 user_command;
    e_cmd_parser_commands system_command;
}t_mapped_cmds;

// Local structure used to map certein strings received from user with system enum
static const t_mapped_cmds predefined_commands[NUM_OF_POSSIBLE_CMDS] = {
                                                                        {"GET_TEMP", CMD_GET_TEMP}, 
                                                                        {"GET_HUM", CMD_GET_HUM},
                                                                        {"SET_INTERVAL", CMD_SET_INTERVAL},
                                                                        {"LOG_DUMP", CMD_LOG_DUMP}
                                                                       }; 

// helper function used to determine which system command will be used depending on a user string
static e_cmd_parser_commands e_parse_cmd(const char* user_cmd_string)
{
    for(size_t i = 0u; i < NUM_OF_POSSIBLE_CMDS; i++)
    {
        if (strcmp(user_cmd_string, predefined_commands[i].user_command) == 0)
        {
            return predefined_commands[i].system_command;
        }
    }

    return CMD_UNKNOWN;
} 

t_cmd_parser_operational_data cmd_parser_parse_user_cmd(const char* user_command)
{
    t_cmd_parser_operational_data parsed_data = {CMD_UNKNOWN, 0};
    char cmd_str[BIGGEST_USER_COMMAND] = {0};
    char opt_data_str[BIGGEST_OPT_DATA_SIZE] = {0};
    size_t j = 0;
    size_t i = 0;

    // If string is NULL, don't do any operation on the data, return CMD_UNKNOWN
    if(user_command == NULL) return parsed_data;

    // extract command in separate string
    for(i = 0u; user_command[i] != ' '&& user_command[i] != '\0'; i++)
    {
        // Get each character
        cmd_str[i] = user_command[i];
    }

    // Skip whitespace
    if(user_command[i] == ' ') i++;

    // extract optional data string
    for(; user_command[i]; i++)
    {
        opt_data_str[j] = user_command[i];
        j++;
    }
    
    // Extract system command
    parsed_data.parsed_cmd = e_parse_cmd(cmd_str);
    // Extract optional data 
    parsed_data.opt_param = atoi(opt_data_str);

    return parsed_data;
}