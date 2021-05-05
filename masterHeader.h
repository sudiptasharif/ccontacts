#ifndef MASTERHEADER_H_INCLUDED
#define MASTERHEADER_H_INCLUDED
#define _GNU_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sqlite3.h> 
#include <pcre.h>

#include "pbSQLite.h"
#include "pbLog.h"
#include "pbSecurity.h"
#include "phonebook.h"

#define PHONEBOOK_C_FILE "phonebook.c"
#define PBLOG_C_FILE "pbLog.c"
#define PBSECURITY_C_FILE "pbSecurity.c"
#define PBSQLITE_C_FILE "pbSQLite.c"

#define AUDIT_FILE "res/audit.txt"
#define LOG_FILE "res/pbLog.txt"
#define DB_NAME "res/pbDb.db"

#define NULL_TERMINATOR '\0'
#define SPACE_CHAR ' '
#define MAX_ERROR_MSG_LENGTH 150
#define PHONE_BOOK_NO_RECORD 0
#define REGEX_COMPILE_FAILED 0
#define REGEX_STUDY_FAILED 0
#define REGEX_MATCH_NOT_FOUND 0
#define REGEX_MATCH_FOUND 1

#define VALID_INPUT 0
#define INVALID_INPUT 1

#endif