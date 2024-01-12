
#ifndef _FSINTER_H
#define _FSINTER_H

/**
 @file fsinter.h
 @brief file system interface
 
 plus message to alert compared with @ref fsutils.h

 @note do not forget firstly @ref cmsgCfg
*/
#include "cmsg.h"
#include "fsutils.h"

void warnCantOpenFile(const char*fname);

/** will warn for "can't open file" and "...dir"
  @see pushInDir()
*/
enum DirScanStat reprPushInDir(RecSeq*p, const char* dir);


/**
  will show message about the pushing status
  @param path either a file or directory path
  @returns whether successful
*/
bool reprPushPath(RecSeq*p, const char* path);


#endif // #ifndef _FSINTER_H