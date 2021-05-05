final:
	gcc -Wall -g pbApp.c pbLog.c pbSecurity.c pbSQLite.c phonebook.c -lsqlite3 -lpcre -o pb