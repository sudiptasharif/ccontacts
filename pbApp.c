#include "masterHeader.h"

#define PBAPP_C_FILE "pbApp.c"

int main(int argc, char *argv[])
{
    if (privilegedUser())
    {
        char *errMsg = "";
        if (initPhoneBookDb(errMsg) == SQLITE_OK)
        {
            if (validTerminalArgCount(argc))
            {
                struct Command cmd = setCommand(argc, argv[TERMINAL_CMD_POS]);
                if (validTerminalArgs(argv, cmd))
                {
                    executeCmd(cmd, argv);
                }
            }
        }
        else
        {
            printf("Program stopped. We are working on this issue.\nIf you keep getting this message, try again later.\n");
            logMsg(PBAPP_C_FILE, "main", errMsg);
            if (errMsg != NULL)
                sqlite3_free(errMsg);
        }
    }
    else
    {
        printf("Program stopped. You don't have permission to access program resources.\n");
    }
    return 0;
}