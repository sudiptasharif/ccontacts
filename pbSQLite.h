#ifndef PBSQLITE_H_INCLUDED
#define PBSQLITE_H_INCLUDED

#include "masterHeader.h"

int initPhoneBookDb(char *errMsg);
int add(const char *num, const char *numFrmtd, const char *name);
int list();
int deleteByName(const char *name);
int deleteByNumber(const char *number);
char* getNameByNumber(char *number);

#endif
