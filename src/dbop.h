
#ifndef _DBOP_H
#define _DBOP_H

// data base operations
/*doctest:
  RecSeq seq;
  initSeq(Rec, seq);

  CharSeq str1 = charpToSeq("here is str1\nnewline\n\nA new para is here");
  Rec r1 = {"f1", str1};

  CharSeq str2 = charpToSeq("Oh, here is str2");
  Rec r2 = {"f2", str2};

  addItem(seq, r1);
  addItem(seq, r2);

  char* word = "is";
  queryAll(seq, word);

  int cnt = countWordOf(seq, "f1", 2);

  printf("%d\n", cnt);


  int fre = countFrequency(seq, "is");

  printf("%d\n", fre);


*/
#include "types.h"
#include "streams.h"

#include "parseutils.h"
#include "strimpl.h"
#include "strutils.h"

enum Err{
  FileNotFoundErr = -1,
  OverRangeErr = -2
};

// query and print all `word` positions in `dr`
void queryAll(const RecSeq rs, const char* word);

/* count word numbers in paragragh `para` of file `fname`
* return -1 if no record with `fname` can be found
* return -2 if `para` is over range
*/ 
int countWordOf(const RecSeq rs, const char* fname, int para);

// count `word`'s frequency
int countFrequency(const RecSeq rs, const char* word);

/* list file content if `fnameOrNULL` is not NULL;
 list all file names if `fnameOrNULL` is NULL.
 returns the number of listed items (0 if fname not found)
*/
int listFile(const RecSeq rs, const char* fnameOrNULL);
#endif //#inndef _DBOP_H
