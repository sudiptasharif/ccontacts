#ifndef PHONEBOOK_H_INCLUDED
#define PHONEBOOK_H_INCLUDED

#define ADD_CMD "ADD"
#define DEL_CMD "DEL"
#define LIST_CMD "LIST"

#define LIST_ARGS_COUNT 2
#define DEL_ARGS_COUNT 3
#define ADD_ARGS_COUNT 4

#define ADD_CMD_CODE 0
#define DEL_CMD_CODE 1
#define LIST_CMD_CODE 2

#define INVALID_CMD_CODE -1
#define INVALID_CMD_ARG_COUNT -1
#define INVALID_TOTAL_TERMINAL_ARG_COUNT -1

#define TERMINAL_APP_NAME_POS 0
#define TERMINAL_CMD_POS 1
#define TERMINAL_ADD_ARG1_POS 2
#define TERMINAL_ADD_ARG2_POS 3
#define TERMINAL_DEL_ARG_POS 2
#define TERMINAL_MIN_ARG_COUNT 2

#include "masterHeader.h"

struct Command 
{
    int cmdCode;
    int cmdArgCount;
    int totalTerminalArgCount;
};

struct Command setCommand(int argCount, char *cmdStr);
void helpPhoneBookCmds();
int validTerminalArgs(char *argv[], struct Command cmd);
int validTerminalArgCount(int argc);
void executeAddCmd(char *name, char *number);
void executeListCmd();
void executeDelByNumberCmd(char *number);
void executeDelByNameCmd(char *name);
void executeCmd(struct Command cmd, char *argv[]);

#endif