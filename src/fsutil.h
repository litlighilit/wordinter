
#ifndef _FSUTIL_H
#define _FSUTIL_H

/**
 @file fsutil.h
 @brief filesystem interface utility, support both `gcc` and `MSVC`
*/

/*doctest: include("<assert.h>")
  char*cdir = ".";
  assert(dirExists(cdir));
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

/** append files in @p dir to @p p
 @param[out] p pointer to append file record to
 @param[in] dir [borrowed] directory to scan
 @param[in] fallback can be `NULL`, if not `NULL`, will be passed the file's name that can't be open
    @note any subdirectory will be skipped
    @returns whether to append
*/
enum DirScanStat pushInDir(RecSeq*p, const char*dir, void fallback(const char* filename));


void warnCantOpenFile(const char*fname);

/**will warn for "can't open file" and "...dir"
 * @see @ref pushInDir
*/
enum DirScanStat reprPushInDir(RecSeq*p, const char* dir);

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


/**
 @retval true if the directory `dir` exists.
 @retval false if either @p dir not exists or `dir` is a file
 Follows symlinks.
 @see 
*/
bool dirExists(const char* dir);

#endif //#ifndef _FSUTIL_H
