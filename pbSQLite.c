#include "pbSQLite.h"

// reuse SQLite codes for better error handling
// here is the url: https://www.sqlite.org/rescode.html
// ex: SQLITE_OK, SQLITE_DONE, SQLITE_CONSTRAINT

sqlite3 *openDB(const char *dbName)
{
    sqlite3 *db = NULL;
    sqlite3_open("pbDB.db", &db);
    return db; // Make sure to close it wherever this is returned.
}

int createTablePhoneBook(char *errMsg)
{
    int resultCode = 0;
    sqlite3 *db = openDB(DB_NAME);
    if (db != NULL)
    {
        char *sql = "CREATE TABLE IF NOT EXISTS PhoneBook ( number TEXT PRIMARY KEY, numFormatted TEXT NOT NULL, name TEXT NOT NULL);";
        resultCode = sqlite3_exec(db, sql, 0, 0, &errMsg);
        sqlite3_close(db);
    }
    return resultCode;
}

int add(const char *num, const char *numFrmtd, const char *name)
{
    int resultCode = 0;
    sqlite3_stmt *stmt;
    sqlite3 *db = openDB(DB_NAME);
    if (db != NULL)
    {
        char *sql = "INSERT INTO PhoneBook VALUES (?, ?, ?);";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, num, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, numFrmtd, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, name, -1, SQLITE_STATIC);

            resultCode = sqlite3_step(stmt);
            
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    return resultCode;
}

int deleteByName(const char *name)
{
    int resultCode = 0;
    sqlite3_stmt *stmt;
    sqlite3 *db = openDB(DB_NAME);
    if (db != NULL)
    {
        char *sql = "DELETE FROM PhoneBook WHERE name=?;";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            resultCode = sqlite3_step(stmt); // SQLITE_DONE

        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    return resultCode;
}

int deleteByNumber(const char *number)
{
    int resultCode = 0;
    sqlite3_stmt *stmt;
    sqlite3 *db = openDB(DB_NAME);
    if (db != NULL)
    {
        char *sql = "DELETE FROM PhoneBook WHERE number=?;";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if(resultCode == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, number, -1, SQLITE_STATIC);
            resultCode = sqlite3_step(stmt); // SQLITE_DONE

        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    return resultCode;    
}

int list()
{
    int resultCode = 0;
    sqlite3_stmt *stmt;
    char *person = "Person";
    char *numFrmtd = "Number";
    sqlite3 *db = openDB(DB_NAME);
    if (db != NULL)
    {
        char *sql = "SELECT name, numFormatted FROM PhoneBook;";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
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
        sqlite3_close(db);
    }
    return resultCode;
}
