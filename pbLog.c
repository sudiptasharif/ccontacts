#include "masterHeader.h"

struct LogTime getLogTime()
{
    struct LogTime lt = {0, 0, 0, 0, 0, 0};
    time_t now = time(NULL);
    if (now != -1)
    {
        struct tm *ptm = localtime(&now);
        if (ptm != NULL)
        {
            lt.year = ptm->tm_year + 1900;
            lt.month = ptm->tm_mon + 1;
            lt.day = ptm->tm_mday;
            lt.hour = ptm->tm_hour;
            lt.min = ptm->tm_min;
            lt.sec = ptm->tm_sec;
        }
    }
    return lt;
}

int logMsg(const char *file, const char *func, const char *msg)
{
    int result = 0;
    FILE *pfile = fopen(LOG_FILE, "a");
    char *logStr = "%02d/%02d/%04d %02d:%02d:%02d:- %s | %s | %s\n";
    struct LogTime lt = getLogTime();
    if (pfile != NULL)
    {
        fprintf(pfile, logStr, lt.month, lt.day, lt.year, lt.hour, lt.min, lt.sec, file, func, msg);
        result = 1;
    }
    fclose(pfile);
    return result;
}

int logErrorCode(const char *file, const char *func, const char *errorMsg, int errorCode)
{
    char msg[MAX_ERROR_MSG_LENGTH];
    memset(msg, NULL_TERMINATOR, MAX_ERROR_MSG_LENGTH * sizeof(msg[0]));
    snprintf(msg, MAX_ERROR_MSG_LENGTH, "%s: %d", errorMsg, errorCode);
    return logMsg(file, func, msg);
}

int logListCmd(uid_t uid)
{
    int result = 0;
    char *cmd = "LIST";
    char *logStr = "%02d/%02d/%04d %02d:%02d:%02d:- uid: %d | %s\n";
    struct LogTime lt = getLogTime();
    FILE *pfile = fopen(AUDIT_FILE, "a");

    if (pfile != NULL)
    {
        fprintf(pfile, logStr, lt.month, lt.day, lt.year, lt.hour, lt.min, lt.sec, uid, cmd);
        result = 1;
    }
    fclose(pfile);
    return result;
}

int logAddCmd(uid_t uid, const char *name)
{
    int result = 0;
    char *cmd = "ADD";
    char *logStr = "%02d/%02d/%04d %02d:%02d:%02d:- uid: %d | %s | %s\n";
    struct LogTime lt = getLogTime();
    FILE *pfile = fopen(AUDIT_FILE, "a");

    if (pfile != NULL)
    {
        fprintf(pfile, logStr, lt.month, lt.day, lt.year, lt.hour, lt.min, lt.sec, uid, cmd, name);
        result = 1;
    }
    fclose(pfile);
    return result;
}

int logDeleteCmd(uid_t uid, const char *name)
{
    int result = 0;
    char *cmd = "DEL";
    char *logStr = "%02d/%02d/%04d %02d:%02d:%02d:- uid: %d | %s | %s\n";
    struct LogTime lt = getLogTime();
    FILE *pfile = fopen(AUDIT_FILE, "a");

    if (pfile != NULL)
    {
        fprintf(pfile, logStr, lt.month, lt.day, lt.year, lt.hour, lt.min, lt.sec, uid, cmd, name);
        result = 1;
    }
    fclose(pfile);
    return result;
}