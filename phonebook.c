#include "masterHeader.h"

struct Command setCommand(int argCount, char *cmdStr)
{
    struct Command command = {INVALID_CMD_CODE, INVALID_CMD_ARG_COUNT, INVALID_TOTAL_TERMINAL_ARG_COUNT};
    int cmdLen = 0;
    if (cmdStr != NULL)
    {
        cmdLen = strlen(cmdStr);
        command.totalTerminalArgCount = argCount;
        if (strncmp(cmdStr, ADD_CMD, cmdLen) == 0)
        {
            command.cmdCode = ADD_CMD_CODE;
            command.cmdArgCount = ADD_ARGS_COUNT;
        }
        else if (strncmp(cmdStr, DEL_CMD, cmdLen) == 0)
        {
            command.cmdCode = DEL_CMD_CODE;
            command.cmdArgCount = DEL_ARGS_COUNT;
        }
        else if (strncmp(cmdStr, LIST_CMD, cmdLen) == 0)
        {
            command.cmdCode = LIST_CMD_CODE;
            command.cmdArgCount = LIST_ARGS_COUNT;
        }
    }
    return command;
}

void helpPhoneBookCmds()
{
    printf("Usage:\n\npb <Command> [<Command Arg1>[, <Command Arg2>]]\n");
    printf("Add, Delete, or List phone numbers of members stored in local SQLite database.\n");
    printf("\n");
    printf("Valid command usage:\n\n");
    printf("ADD “<Person>” “<Telephone #>” - Add a new person to the database.\n");
    printf("DEL “<Person>” - Remove someone from the database by name.\n");
    printf("DEL “<Telephone #>” - Remove someone by telephone #\n");
    printf("LIST - Produce a list of the members of the database.\n\n");
}

int validTerminalArgCount(int argc)
{
    int result = 0;
    if (argc >= TERMINAL_MIN_ARG_COUNT)
    {
        result = 1;
    }
    else
    {
        printf("pb: invalid arguments to program\n\n");
        helpPhoneBookCmds();
    }
    return result;
}

int validTerminalArgs(char *argv[], struct Command cmd)
{
    int result = 0;
    if (cmd.cmdCode == INVALID_CMD_CODE)
    {
        printf("pb: invalid command -- '%s'\n\n", argv[TERMINAL_CMD_POS]);
        helpPhoneBookCmds();
    }
    else if ((cmd.totalTerminalArgCount - cmd.cmdArgCount) != 0)
    {
        printf("pb: invalid arguments to command '%s'\n\n", argv[TERMINAL_CMD_POS]);
        helpPhoneBookCmds();
    }
    else
    {
        result = 1;
    }
    return result;
}

void executeListCmd()
{
    int resultCode = list();
    if (resultCode != SQLITE_DONE)
    {
        printf("\nMember list not produced. Incident reported. Try again later.\n\n");
        logErrorCode(PHONEBOOK_C_FILE, "executeListCmd", "SQLITE CODE", resultCode);
    }
    else
    {
        logListCmd(getuid());
    }
}

void executeAddCmd(char *name, char *number)
{
    char *unformattedNumber = extractNumbers(number);
    int resultCode = add(unformattedNumber, number, name);
    if (resultCode == SQLITE_DONE)
    {
        printf("\nMember added.\n\n");
        logAddCmd(getuid(), name);
        
    }
    else if (resultCode == SQLITE_CONSTRAINT)
    {
        printf("\nMember not added. Duplicate phone number. %s exists in database.\n\n", number);
    }
    else
    {
        printf("\nMember not added. Incident reported. Try again later.\n\n");
        logErrorCode(PHONEBOOK_C_FILE, "executeAddCmd", "SQLITE CODE", resultCode);
    }
    free(unformattedNumber);
}

void executeDelByNumberCmd(char *number)
{
    char *name = NULL; 
    char *unformattedNumber = extractNumbers(number);
    name = getNameByNumber(unformattedNumber);
    int resultCode = deleteByNumber(unformattedNumber);
    if (resultCode == SQLITE_DONE)
    {
        printf("\nMember deleted.\n\n");
        logDeleteCmd(getuid(), name);
    }
    else if (resultCode == PHONE_BOOK_NO_RECORD)
    {
        printf("\nMember not deleted. No record in database.\n\n");
    }
    else
    {
        printf("\nMember not deleted. Incident reported. Try again later.\n\n");
        logErrorCode(PHONEBOOK_C_FILE, "executeDelByNumberCmd", "SQLITE CODE",resultCode);
    }
    free(unformattedNumber);
    free(name);
}

void executeDelByNameCmd(char *name)
{
    int resultCode = deleteByName(name);
    if (resultCode == SQLITE_DONE)
    {
        printf("\nMember deleted.\n\n");
        logDeleteCmd(getuid(), name);
    }
    else if (resultCode == PHONE_BOOK_NO_RECORD)
    {
        printf("\nMember not deleted. No record in database.\n\n");
    }
    else
    {
        printf("\nMember not deleted. Incident reported. Try again later.\n\n");
        logErrorCode(PHONEBOOK_C_FILE, "executeDelByNameCmd", "SQLITE CODE:", resultCode);
    }
}

void executeCmd(struct Command cmd, char *argv[])
{
    char *input1 = NULL;
    char *input2 = NULL;

    if (cmd.cmdCode == LIST_CMD_CODE)
    {
        executeListCmd();
    }
    else if (cmd.cmdCode == ADD_CMD_CODE)
    {
        input1 = trimStr(argv[TERMINAL_ADD_ARG1_POS]);
        input2 = trimStr(argv[TERMINAL_ADD_ARG2_POS]);

        if (validName(input1) == VALID_INPUT)
        {
            if (validNumber(input2) == VALID_INPUT)
            {
                executeAddCmd(input1, input2);
            }
            else
            {
                printf("Program stopped. Invalid number format.\n");
            }
        }
        else
        {
            printf("Program stopped. Invalid name format.\n");
        }
    }
    else if (cmd.cmdCode == DEL_CMD_CODE)
    {
        input1 = trimStr(argv[TERMINAL_DEL_ARG_POS]);

        if (isalpha(input1[0]))
        {
            if (validName(input1) == VALID_INPUT)
            {
                executeDelByNameCmd(input1);
            }
            else
            {
                printf("Program stopped. Invalid name format.\n");
            }
        }
        else
        {
            if (validNumber(input1) == VALID_INPUT)
            {
                executeDelByNumberCmd(input1);
            }
            else
            {
                printf("Program stopped. Invalid number format.\n");
            }
        } 
    }
    if (input1 != NULL)
        free(input1);
    if (input2 != NULL)
        free(input2);
}