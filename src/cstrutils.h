
#ifndef _CSTRUTILS_H
#define _CSTRUTILS_H

#define WIN_OS_SEP '\\'
#define UNIX_OS_SEP '/'

#define isOsSep(c) (c==WIN_OS_SEP || c==UNIX_OS_SEP)

#ifdef _WIN32 // Win32 or Win64
#define OS_SEP WIN_OS_SEP
#else
#define OS_SEP UNIX_OS_SEP
#endif // _WIN32

// join dir, fname and returns a new string
char* joinPath(const char* dir, const char*fname);

// returns a reference
const char* basenameRef(const char* path);

char* newCStr(const char* s);

char* newSubCStr(const char* s, int n);

char* catCStr(const char*a, const char*b);

// return -1 if not found
int findChar(const char* s, char c);

#endif //#ifndef _CSTRUTILS_H