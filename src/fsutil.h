
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


#define WIN_OS_SEP '\\'
#define UNIX_OS_SEP '/'

#define isOsSep(c) (c==WIN_OS_SEP || c==UNIX_OS_SEP)

#ifdef _WIN32 // Win32 or Win64
#define OS_SEP WIN_OS_SEP
#else
#define OS_SEP UNIX_OS_SEP
#endif // _WIN32


/* list data in files of `dir` and returns
returns an empty seq if no data read
*/
RecSeq listDir(const char* dir);

// ifndef READ_KEEP_R, then discard '\r'
CharSeq readAll(FILE* f);


// join dir, fname and returns a new string
char* joinPath(const char* dir, const char*fname);

#endif //#ifndef _FSUTIL_H
