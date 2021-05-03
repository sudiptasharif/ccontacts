#ifndef PBLOG_H_INCLUDED
#define PBLOG_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define AUDIT_FILE "res/audit.txt"
#define LOG_FILE "res/pbLog.txt"
#define PBLOG_SRC_FILE "pbLog.c"

struct logTime
{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
};
int logListCmd(uid_t realUserID);
int logAddCmd(uid_t uid, const char *name);
int logDeleteCmd(uid_t uid, const char *name);
int logMsg(const char *file, const char *func, const char *msg);
struct logTime getLogTime();

#endif