
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
  IndexErr = -2,
  
};

/** query and print all @p word positions
 * @returns whether print any, i.e. if any one found
*/
bool queryAll(const Interpreter interp, const char* word, bool ignoreCase);

/** count word numbers in paragragh @p para of file at @p fileOrd
 @param fileOrd file order counting from 1 (Not 0)
 @retval @ref FileNotFoundErr if no record with @p fileOrd can be found(i.e. out of range)
 @retval @ref IndexErr if @p para is over range
*/ 
int countWordOf(const Interpreter interp, int fileOrd, int para);

/// count the frequency of @p word
int countFrequency(const Interpreter interp, const char* word, bool ignoreCase);

/**
 list:

 - file content if @p fileOrd is a Positive;
 - all file names if @p fileOrd is 0.

 @param fileOrd file order counting from 1 (0 means listing all files)
 @returns the number of listed items
 @retval 0 if @p fileOrd out of range
*/
int listFile(const Interpreter interp, int fileOrd);
#endif //#inndef _DBOP_H
