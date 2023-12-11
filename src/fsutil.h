
#ifndef _FSUTIL_H
#define _FSUTIL_H

// filesystem interface util

/*doctest:
  char*cdir = ".";
  RecSeq seq = listDir(cdir);

  printf("len: %d\n", seq.len);

  forIndex(idx, seq){
    Rec rec = getItem(seq, idx);
    CharSeq content = rec.data;
    printf("%s len: %d\n", rec.fname, content.len);
  }
*/

#include <stdio.h>
#include "types.h"
#include "cstrutils.h"


/* list data in files of `dir` and returns
returns an empty seq if no data read
*/
RecSeq listDir(const char* dir);

// ifndef READ_KEEP_R, then discard '\r'
CharSeq readAll(FILE* f);



#endif //#ifndef _FSUTIL_H
