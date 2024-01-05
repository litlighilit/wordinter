
#ifndef _FSUTIL_H
#define _FSUTIL_H

/**
 @file fsutil.h
 @brief filesystem interface utility, support both `gcc` and `MSVC`
*/

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

enum DirScanStat{
    CantOpen=-1, ///< can't open directory
    Succ,
    ItemSkipped ///< can't open some items
};

/** list data in files of @p dir
 @param[in] dir [copied] directory to list
 @returns @ref RecSeq i.e. @ref Seq of @ref Rec
 @retval an empty seq if no data read
 @post free @ref RecSeq result via @ref freeListDir
*/
RecSeq listDir(const char* dir);


/** push file content as record to a @ref RecSeq pointer
 @param[in,out] p a pointer
 @param[in] dir [copied] can be an empty string i.e. `""`
 @param[in] fname [copied] file name (also will be stored in @ref Rec)
 @returns whether read file content successfully
*/
bool pushRec(RecSeq*p, const char* dir, const char* fname);

/// @pre @ref listDir
/// @note latter @p rs is in broken status and shall not be passed to addItem...etc.
void freeListDir(RecSeq rs);

/**
  @attention if not define READ_KEEP_R (default), then discard '\r'
*/
CharSeq readAll(FILE* f);



#endif //#ifndef _FSUTIL_H
