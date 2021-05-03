#include "pbSQLite.h"

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
 * Returns: the number of rows affected by the INSERT statement. 
 * Otherwise if there is an error returns that SQLITE error code. 
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

            sqlite3_step(stmt);
            resultCode = sqlite3_changes(ppDb);
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
 * Returns: the number of rows affected by the DELETE statement. 
 * Otherwise if there is an error returns that SQLITE error code.
*/
int deleteByName(const char *name)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "DELETE FROM PhoneBook WHERE name=?;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            resultCode = sqlite3_changes(ppDb);

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
 * Returns: the number of rows affected by the DELETE statement. 
 * Otherwise if there is an error returns that SQLITE error code.
*/
int deleteByNumber(const char *number)
{
    sqlite3 *ppDb = NULL;
    int resultCode = sqlite3_open(DB_NAME, &ppDb);
    if (resultCode == SQLITE_OK)
    {
        sqlite3_stmt *stmt = NULL;
        char *sql = "DELETE FROM PhoneBook WHERE number=?;";
        resultCode = sqlite3_prepare_v2(ppDb, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, number, -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            resultCode = sqlite3_changes(ppDb);

        }
        sqlite3_finalize(stmt);  
    }
    sqlite3_close(ppDb);
    return resultCode;    
}

/**
 * list: returns all the records from the PhoneBook table by name.
 *  
 * Returns: SQLITE_OK (0) if no errors, otherwise the corresponding 
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
            while (sqlite3_step(stmt) != SQLITE_DONE)
            {
                // column 1 corresponds to phone number
                // column 2 corresponds to person
                printf("%-25s %-25s\n", (char *)sqlite3_column_text(stmt, 0), (char *)sqlite3_column_text(stmt, 1));
            }
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(ppDb);
    return resultCode;
}
