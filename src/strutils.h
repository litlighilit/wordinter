
#ifndef _STRUTILS_H
#define _STRUTILS_H

/*doctest: include("<assert.h>")
  CharSeq s = charpToSeq("The order is 3");

// # split2
  PairS p = split2(s, ' ');
  printlnCharSeq(p.left); //-> The
  printlnCharSeq(p.right); //-> order is 3

  freePairS(p);

// # strSlice
  CharSeq ss = strSlice(s, s.len-4);
  printlnCharSeq(ss); //-> is 3

// # parseInt
  CharSeq s3 = strSlice(s, s.len-2);
  int n;
  bool succ = parseInt(s3, &n);
  assert(succ);
  printf("%d\n", n); //-> 3

// # getLine
  puts("input something:");
  CharSeq line = getLine();
  printlnCharSeq(line);

  deinitSeq(line);
*/

#include <stdbool.h>
#include "strimpl.h"
#include "parseutils.h"

typedef struct {
    CharSeq left, right;
} PairS;

#define freePairS(p) do{\
  deinitSeq(p.left);\
  deinitSeq(p.right);\
}while(0)


/*if eof, return an empty CharSeq (len==0)
otherwise, result includes newline char*/
CharSeq getLine();

/* like Python's s.split(sep, 1)
but will treat all successive `seq` as one*/
PairS split2(const CharSeq s, char sep);

CharSeq strAdd(const CharSeq s1, const CharSeq s2);

CharSeq strSlice(const CharSeq s, int a);

bool parseInt(const CharSeq s, int*res);

#endif //#ifndef _STRUTILS_H
