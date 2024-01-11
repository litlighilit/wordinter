
#ifndef _FSUTILS_H
#define _FSUTILS_H

/**
 @file fsutils.h
 @brief filesystem interface utilities, support both POSIX c compiler like `gcc` and `MSVC`
*/

/*doctest: include("<assert.h>")
  char*cdir = ".";
  assert(dirExists(cdir));
  RecSeq seq = listDir(cdir);

  printf("len: %" PRI_SLEN "\n", seq.len);

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
    dsCantOpen=-1, ///< can't open directory
    dsSucc,
    dsItemSkipped, ///< can't open some items
};

/** list data in files of @p dir
 @param[in] dir [copied] directory to list
 @returns @ref RecSeq i.e. @ref Seq of @ref Rec
 @retval an empty seq if no data read
 @post free @ref RecSeq result via freeListDir()
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


/** push file content as record to a @ref RecSeq pointer
 @param[in,out] p a pointer
 @param[in] fpath [copied] file path
 @returns whether read file content successfully
*/
bool pushFile(RecSeq*p, const char* fpath);

/// @pre listDir() or pushFile() or pushInDir()
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

 > translated from [nim-lang](
  https://github.com/nim-lang/Nim/tree/version-2-0/lib/std/private/oscommon.nim#L139)
*/
bool dirExists(const char* dir);

#endif //#ifndef _FSUTILS_H
