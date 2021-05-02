#ifndef PBSQLITE_H_INCLUDED
#define PBSQLITE_H_INCLUDED
#include <sqlite3.h> 
#include <stdio.h>

#define DB_NAME "pbDB.db"
sqlite3* openDB(const char *dbName);
int createTablePhoneBook(char *errMsg);
int add(const char *num, const char *numFrmtd, const char *name);
int list();
int deleteByName(const char *name);
int deleteByNumber(const char *number);
#endif
