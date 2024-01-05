
#ifndef _TYPES_H
#define _TYPES_H

//[header-only]
/*doctest:
  Pos pos = {"fname", 3, 2};
  printPos(pos);
*/

#include "seq.h"
#include "strimpl.h"

/// Position of a word
typedef struct{
    char* fname; ///< file name
    int para; ///< paragraph index
    int idx; ///< word index of one paragragh
} Pos;

typedef Seq(Pos) PosSeq;

#define printPos(pos) printf("in file %s at paragraph %d in the word index %d\n"\
    , pos.fname, pos.para, pos.idx);


typedef struct{
    char*fname;
    CharSeq data;
} Rec; ///< record of file data


typedef Seq(Rec) RecSeq;

#endif //#ifndef _TYPES_H
