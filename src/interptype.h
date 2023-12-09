
#ifndef _INTERPTYPE_H
#define _INTERPTYPE_H
// interpreter type

#include "types.h"

typedef struct{
    bool multiLinePara;
    RecSeq db;
} Interpreter;

#define initInterpreter(db,mode) ((Interpreter){mode,db})

#endif //#ifndef _INTERPTYPE_H
