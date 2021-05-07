#ifndef PBSECURITY_H_INCLUDED
#define PBSECURITY_H_INCLUDED

int privilegedUser();
int validName(char *name);
int validNumber(char *number);
int evalutateRegex(char *input, char *aStrRegex);
char* trimStr(char *str);
char* extractNumbers(char *str);
#endif