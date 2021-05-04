#ifndef PBLOG_H_INCLUDED
#define PBLOG_H_INCLUDED

#include "masterHeader.h"

struct LogTime
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
int logIntMsg(const char *file, const char *func, int msgInt);
struct LogTime getLogTime();

#endif