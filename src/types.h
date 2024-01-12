
#ifndef _TYPES_H
#define _TYPES_H

//[header-only]
/**
 @file types.h
 @brief types for storing data
*/
/*doctest:
  Pos pos = {"fname", 3, 2};
  printPos(pos);
*/

#include "seq.h"
#include "strimpl.h"

/// Position of a word
typedef struct{
    char* fname; ///< file name
    size_t para; ///< paragraph index
    size_t idx; ///< word index of one paragragh
} Pos;

typedef Seq(Pos) PosSeq;

#define printPos(pos) printf("in file %s at paragraph %zu in the word index %zu\n"\
    , pos.fname, pos.para, pos.idx);


typedef struct{
    char*fname;
    CharSeq data;
} Rec; ///< record of file data


typedef Seq(Rec) RecSeq;

#endif //#ifndef _TYPES_H
