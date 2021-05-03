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
            printf("resultCode = %d\n", add("2604455219", "(260) 445-5219", "Sudipta Sharif"));

            printf("resultCode = %d\n", add("2604455219", "(260) 445-5219", "Sudipta Sharif"));

            printf("resultCode = %d\n", add("2602062387", "(260) 206-2387", "Sudipta Sharif"));

            printf("resultCode = %d\n", add("6463182686", "(646) 318-2686", "Taz N. Silvia"));

            printf("resultCode = %d\n", list());

            // printf("resultCode = %d\n", deleteByNumber("2602062387"));

            // printf("resultCode = %d\n", deleteByName("Taz N. Silvia"));

            // printf("resultCode = %d\n", list());

            // printf("resultCode = %d\n", deleteByNumber("12356"));

            // printf("resultCode = %d\n", deleteByNumber("sumu"));

            // char *name = getNameByNumber("2604455219");

            // if(name != NULL)
            //     printf("person name = %s\n",  name);
            // free(name);

            // char *anotherName = getNameByNumber("123413");
            // if(anotherName != NULL)
            //     printf("person name = %s\n",  anotherName);
            // else
            //     printf("no person found\n");

            // free(anotherName);

            char *str = "%02d/%02d/%04d %02d:%02d:%02d\n";
            struct logTime lt = getLogTime();
            printf(str, lt.month, lt.day, lt.year, lt.hour, lt.min, lt.sec);

            logMsg(PB_APP_SRC_FILE, "main", "this is a test!");

            uid_t caller_uid = getuid();
            logListCmd(caller_uid);
            logAddCmd(caller_uid, getNameByNumber("2604455219"));
            logDeleteCmd(caller_uid, "Sudipta Sharif");
            logDeleteCmd(caller_uid, getNameByNumber("6463182686"));
        }
        else
        {
            printf("Error: %s\n", errMsg);
            sqlite3_free(errMsg);
        }
    }
    else
    {
        printf("Program Stopped. You don't have permission to access program resources.\n");
    }

    return 0;
}