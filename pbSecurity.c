#include "pbSecurity.h"

int privilegedUser()
{
    int result = 0;
    uid_t caller_uid = getuid();
    uid_t owner_uid = geteuid();

    setresuid(owner_uid, -1, caller_uid);
    if ((access(DB_NAME, R_OK) == 0) && (access(DB_NAME, W_OK) == 0) && (access(AUDIT_FILE, R_OK) == 0) && (access(AUDIT_FILE, W_OK) == 0) && (access(LOG_FILE, R_OK) == 0) && (access(LOG_FILE, W_OK) == 0))
    {
        result = 1;
    }
    setresuid(caller_uid, -1, owner_uid);
    return result;
}

int validName(const char *name)
{
    //TODO
    return 1;
}
int validNumber(const char *number)
{
    //TODO
    return 1;
}