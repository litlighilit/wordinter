
#ifndef _DBOP_H
#define _DBOP_H

/**
 @file dbop.h
 @brief data base operations
 @details some functions to operate @ref Interpreter
*/

/*doctest:
  RecSeq seq;
  initSeq(Rec, seq);

  CharSeq str1 = charpToSeq("here is str1\nnewline\n\nA new para is here");
  Rec r1 = {"f1", str1};

  CharSeq str2 = charpToSeq("Oh, here is str2");
  Rec r2 = {"f2", str2};

  addItem(seq, r1);
  addItem(seq, r2);

  Interpreter interp = initInterpreter(seq, true);

  char* word = "is";
  queryAll(interp, word);

  int cnt = countWordOf(interp, "f1", 2);

  printf("%d\n", cnt);


  int fre = countFrequency(interp, word);

  printf("%d\n", fre);


*/
#include "types.h"
#include "streams.h"
#include "interptype.h"

#include "parseutils.h"
#include "strimpl.h"
#include "strutils.h"

enum Err{
  FileNotFoundErr = -1,
  OverRangeErr = -2
};

/// query and print all @p word positions
void queryAll(const Interpreter interp, const char* word);

/** count word numbers in paragragh @p para of file @p fname
* @retval -1 if no record with @p fname can be found
* @retval -2 if @p para is over range
*/ 
int countWordOf(const Interpreter interp, const char* fname, int para);

/// count the frequency of @p word
int countFrequency(const Interpreter interp, const char* word);

/**
 list:

 - file content if @p fnameOrNULL is not NULL;
 - all file names if @p fnameOrNULL is NULL.
 
 @returns the number of listed items
 @retval 0 if @p fname not found
*/
int listFile(const Interpreter interp, const char* fnameOrNULL);
#endif //#inndef _DBOP_H
