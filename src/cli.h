
#ifndef _CLI_H
#define _CLI_H

#include <stdbool.h>
#include "strimpl.h"


#define STR_EQ(a,b) (strcmp(a,b)==0) 

#define C_FLAG "-c"
#define SGL_LNE_FLAG "-l"
#define MUL_LNE_FLAG "-L"
#define WANT_HELP(s) (STR_EQ(s, "-h") || STR_EQ(s, "--help"))
#define WANT_VER(s) (STR_EQ(s, "-v") || STR_EQ(s, "--version"))


typedef struct{
    bool multiLinePara;
    bool help;
    bool version;
    char*arg;
    CharSeq cmd;
    CharSeq firstUnknowCmd;
} Args;

Args parseArg(const char* args[], int len);

#endif // #ifndef _CLI_H