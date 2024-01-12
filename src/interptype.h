
#ifndef _INTERPTYPE_H
#define _INTERPTYPE_H
// interpreter type

#include "types.h"

typedef struct{
    bool multiLinePara; ///< if in multiLinePara mode
    RecSeq db;
} Interpreter;

/// @param[in] db [borrowed] a @ref RecSeq obj
#define initInterpreter(db,mode) ((Interpreter){mode,db})

#endif //#ifndef _INTERPTYPE_H
