#include "pbSQLite.h"
#include "pbLog.h"
#include "pbSecurity.h"

int main(int argc, char *argv[]) 
{
    int resultCode = 0;
    char *errMsg = "";
    sqlite3 *db = openDB(DB_NAME); // need this to create the db if one does not exist

    if (db != NULL)
    {
        if (createTablePhoneBook(errMsg) == SQLITE_OK)
        {
            resultCode = add("2604455219", "(260) 445-5219", "Sudipta Sharif");
            resultCode = add("2604455219", "(260) 445-5219", "Sudipta Sharif");
            resultCode = add("2602062387", "(260) 206-2387", "Sudipta Sharif");
            resultCode = add("6463182686", "(646) 318-2686", "Taz N. Silvia");
            list();
            deleteByNumber("2602062387");
            deleteByName("Taz N. Silvia");
            list();
        }
        else
        {
            printf("Error: %s\n", errMsg);
        }
    }
    else
    {
        printf("Error: Failed to open %s\n", DB_NAME);
        resultCode = 1;
    }
    sqlite3_close(db);
    return resultCode;
}
