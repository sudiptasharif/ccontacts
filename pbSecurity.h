#ifndef PBSECURITY_H_INCLUDED
#define PBSECURITY_H_INCLUDED

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pbLog.h"
#include "pbSQLite.h"

#define PBSECURITY_SRC_FILE "pbSecurity.c"

int privilegedUser();

#endif