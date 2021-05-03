#include "pbSQLite.h"
#include "pbLog.h"
#include "pbSecurity.h"

int main(int argc, char *argv[])
{
    char *errMsg = "";

    if (initPhoneBookDb(errMsg) == SQLITE_OK)
    {
        printf("resultCode = %d\n", add("2604455219", "(260) 445-5219", "Sudipta Sharif"));

        printf("resultCode = %d\n", add("2604455219", "(260) 445-5219", "Sudipta Sharif"));
    
        printf("resultCode = %d\n", add("2602062387", "(260) 206-2387", "Sudipta Sharif"));

        printf("resultCode = %d\n", add("6463182686", "(646) 318-2686", "Taz N. Silvia"));

        printf("resultCode = %d\n", list());

        printf("resultCode = %d\n", deleteByNumber("2602062387"));

        printf("resultCode = %d\n", deleteByName("Taz N. Silvia"));

        printf("resultCode = %d\n", list());

        printf("resultCode = %d\n", deleteByNumber("12356"));

        printf("resultCode = %d\n", deleteByNumber("sumu"));
    }
    else
    {
        printf("Error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return 0;
}
