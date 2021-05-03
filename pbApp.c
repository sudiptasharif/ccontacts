#include "pbSQLite.h"
#include "pbLog.h"
#include "pbSecurity.h"

#define PB_APP_SRC_FILE "pbApp.c"

int main(int argc, char *argv[])
{
    if (privilegedUser())
    {
        char *errMsg = "";
        if (initPhoneBookDb(errMsg) == SQLITE_OK)
        {
            printf("TODO\nGreat work so far.\n");
        }
        else
        {
            // Generic error message to user is shown to user, while the real error message is logged for debugging.
            printf("Program stopped. We are working on issue.\nIf you keep getting this message, try again later.\n");
            logMsg(PB_APP_SRC_FILE, "main", errMsg);
            sqlite3_free(errMsg);
        }
    }
    else
    {
        // If the process's effective user id does not have permission to read/write to this app's database, audit or any log files
        // program is stopped. A generic error message is given. 
        printf("Program stopped. You don't have permission to access program resources.\nContact system administrator.\n");
    }
    return 0;
}