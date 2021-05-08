#include "masterHeader.h"

/**
 * initPhoneBookDb: used to create the PhoneBook database and it's table PhoneBook
 * if it does not exist in memory yet.
 * 
 * Arguments
 * errMsg: stores error message incase of anything goes wrong during the sqlite3_exec() call.
 * 
 * Returns: SQLITE_OK if no errors, otherwise the corresponding 
 * SQLITE error code. 
*/
int initPhoneBookDb(char *errMsg)
{
    sqlite3 *ppDb =NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    if (resultCode == SQLITE_OK)
    {
        char *sql = "CREATE TABLE IF NOT EXISTS PhoneBook ( number TEXT PRIMARY KEY, numFormatted TEXT NOT NULL, name TEXT NOT NULL);";
        resultCode = sqlite3_exec(ppDb, sql, 0, 0, &errMsg);
    }
    sqlite3_close(ppDb);
    return resultCode;
}

/**
 * add: adds record to the PhoneBook table.
 * 
 * Arguments
 * num: unformatted phone number. I extract this from valid phone number input.
 * numFrmtd: valid formatted phone number passed as argument to the ADD command.
 * name: name of the person
 * 
 * Returns: SQLITE_DONE, otherwise corresponding SQLite error code.
 * 
*/
int add(const char *num, const char *numFrmtd, const char *name)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "INSERT INTO PhoneBook VALUES (?, ?, ?);";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if (resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, num, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, numFrmtd, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, name, -1, SQLITE_STATIC);

            resultCode = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt); 
    }
    sqlite3_close(ppDb);
    return resultCode;
}

/**
 * deleteByName: deletes record from the PhoneBook table by name.
 * 
 * Arguments
 * name: name of the person
 * 
 * Returns: SQLITE_DONE on success. Otherwise will return PHONE_BOOK_NO_RECORD if no record to delete.
 * Or will return corresponding SQLite error code.
*/
int deleteByName(const char *name)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    int recordsDeleted = 0;
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "DELETE FROM PhoneBook WHERE name=?;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            resultCode = sqlite3_step(stmt);
            recordsDeleted = sqlite3_changes(ppDb);
            if ((resultCode == SQLITE_DONE) && (recordsDeleted >= 1))
            {
                resultCode = SQLITE_DONE;
            }
            else if (recordsDeleted == 0)
            {
                resultCode = PHONE_BOOK_NO_RECORD;
            }        
            else
            {
                logErrorCode(PBSQLITE_C_FILE, "deleteByName", "SQLITE CODE: ", resultCode);
                logErrorCode(PBSQLITE_C_FILE, "deleteByName", "RECORDS DELETED: ", recordsDeleted);
            }    
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(ppDb);
    return resultCode;
}

/**
 * deleteByNumber: deletes record from the PhoneBook table by name.
 * 
 * Arguments
 * number: number of the person
 * 
 * Returns: SQLITE_DONE on success. Otherwise will return PHONE_BOOK_NO_RECORD if no record to delete.
 * Or will return corresponding SQLite error code.
*/
int deleteByNumber(const char *number)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    int recordsDeleted = 0;
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "DELETE FROM PhoneBook WHERE number=?;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, number, -1, SQLITE_STATIC);
            resultCode = sqlite3_step(stmt);
            recordsDeleted = sqlite3_changes(ppDb);
            if ((resultCode == SQLITE_DONE) && (recordsDeleted == 1))
            {
                resultCode = SQLITE_DONE;
            }
            else if ((resultCode == SQLITE_DONE) && (recordsDeleted == 0))
            {
                resultCode = PHONE_BOOK_NO_RECORD;
            }
        }
        sqlite3_finalize(stmt);  
    }
    sqlite3_close(ppDb);
    return resultCode;    
}

/**
 * list: returns all the records from the PhoneBook table by name.
 *  
 * Returns: SQLITE_DONE (101) if no errors, otherwise returns the corresponding 
 * SQLITE error code. 
*/
int list()
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *person = "Person";
        char *numFrmtd = "Number";
        char *sql = "SELECT name, numFormatted FROM PhoneBook;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if (resultCode == SQLITE_OK)
        {
            printf("%-25s %-25s\n", person, numFrmtd);
            resultCode = sqlite3_step(stmt);
            while (resultCode != SQLITE_DONE)
            {
                // column 1 corresponds to phone number
                // column 2 corresponds to person
                printf("%-25s %-25s\n", (char *)sqlite3_column_text(stmt, 0), (char *)sqlite3_column_text(stmt, 1));
                resultCode = sqlite3_step(stmt);
            }
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(ppDb);
    return resultCode;
}

/**
 * getNameByNumber: set the ppName variable to the name of the person corresponding to the phone number
 * 
 * Arguments
 * number: a phone number of a person
 * ppName: pointer to a name variable that will be set if a name is found
 * 
 * Returns: the name of the person, if found, otherwise return null. Must free the return value
 * where this function is called.
*/
char* getNameByNumber(char *number)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    char *name = NULL;
    int nameLen = 0;
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "SELECT name FROM PhoneBook WHERE number=?;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, number, -1, SQLITE_STATIC);
            resultCode = sqlite3_step(stmt); // only one row should be returned because number is the primary in my PhoneBook table
            if (resultCode == SQLITE_ROW) 
            {
                nameLen = strlen((const char*)sqlite3_column_text(stmt, 0));
                name = (char*)malloc((nameLen+1)*sizeof(char));
                //memset(name, NULL_TERMINATOR, strlen(name) * sizeof(name[0]));
                strncpy(name, (const char*)(const char*)sqlite3_column_text(stmt, 0), nameLen);
            }
        }
        sqlite3_finalize(stmt);  
    }
    sqlite3_close(ppDb);
    return name;      
}
