#ifndef MASTERHEADER_H_INCLUDED
#define MASTERHEADER_H_INCLUDED

#define _GNU_SOURCE 
#define PHONEBOOK_C_FILE "phonebook.c"
#define PBLOG_C_FILE "pbLog.c"
#define PBSECURITY_C_FILE "pbSecurity.c"
#define PBSQLITE_C_FILE "pbSQLite.c"

#define AUDIT_FILE "res/audit.txt"
#define LOG_FILE "res/pbLog.txt"
#define DB_NAME "res/pbDb.db"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sqlite3.h> 

#include "pbSQLite.h"
#include "pbLog.h"
#include "pbSecurity.h"
#include "phonebook.h"

#endif