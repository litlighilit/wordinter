
#ifndef _CSTRUTILS_H
#define _CSTRUTILS_H

/**
 @file cstrutils.h
 @brief c-string utilities
 
 also contains some path utilities
*/

#define WIN_OS_SEP '\\'
#define UNIX_OS_SEP '/'

#define isOsSep(c) (c==WIN_OS_SEP || c==UNIX_OS_SEP)

#ifdef _WIN32 // Win32 or Win64
#define OS_SEP WIN_OS_SEP
#else
#define OS_SEP UNIX_OS_SEP
#endif // _WIN32

/// join path of a directory and a path
/// @returns a new c-string
char* joinPath(const char* dir, const char*path);

/// @returns a reference (a borrowed c-string)
const char* basenameRef(const char* path);

char* newCStr(const char* s);

/// @returns a new c-string
char* newSubCStr(const char* s, int n);

/// cancatenate two string.
/// @returns a new c-string
char* catCStr(const char*a, const char*b);

/// @retval -1 if not found
int rfindChar(const char* s, char c);

#endif //#ifndef _CSTRUTILS_H