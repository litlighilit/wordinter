
#ifndef _STRUTILS_H
#define _STRUTILS_H

/**
 @file strutils.h
 @brief string utilities

 utilities of @ref CharSeq
*/ 

/*doctest: include("<assert.h>")
  CharSeq s = charpToSeq("The order is 3");

// # split2
  {
  PairS p = split2(s, ' ');
  printlnCharSeq(p.left); //-> The
  printlnCharSeq(p.right); //-> order is 3

  freePairS(p);
  }

// # splitQuo2
  {
    puts("test shellSplit2");
    CharSeq s = charpToSeq("The \"next sd\"");
    PairS p = splitQuo2(s, ' ');
    printlnCharSeq(p.left); //-> "The word"
    printlnCharSeq(p.right); //-> next
    deinitSeq(s);
    freePairS(p);
  }

// # subStrFrom
  CharSeq ss = subStrFrom(s, s.len-4);
  printlnCharSeq(ss); //-> is 3

// # parseInt
  CharSeq s3 = subStrFrom(s, s.len-2);
  int n;
  bool succ = parseInt(s3, &n);
  assert(succ);
  printf("%d\n", n); //-> 3

// # getLine
if(false){
  puts("input something:");
  CharSeq line = getLine();
  printlnCharSeq(line);

  deinitSeq(line);
}
// # seqIEqStr
  {
  CharSeq s=charpToSeq("asdHp");
  bool ieq = seqIEqStr(s, "asDhp");
  printf("%d\n", ieq);
  deinitSeq(s);
  }
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


/** get a line from `stdin`
 @retval `an empty CharSeq` if eof
 @retval a @ref CharSeq that includes newline char*/
CharSeq getLine();

/** like Python's `s.split(sep, 1)`
but will treat all successive @p sep as one
 @note assuming @p s does not start with @p sep, otherwise, result.left will be empty 
 @post @ref freePairS
*/
PairS split2(const CharSeq s, char sep);

/**
 like @ref split2 but regard quotation mark surrounded chars as one item
 @see split2
*/
PairS splitQuo2(const CharSeq s, char sep);

/// @returns a new string
CharSeq strAdd(const CharSeq s1, const CharSeq s2);

/**sub-string at [first,last]*/
CharSeq subStr(const CharSeq s, slen_t first, slen_t last);

/// string slice starting at @p start
/// @returns a new CharSeq
CharSeq subStrFrom(const CharSeq s, slen_t start);

/// parse @p s as an intergar
/// @param s 
/// @param[in] res a pointer to store result interger, untouched if not successful
/// @returns whether is successful
bool parseInt(const CharSeq s, int*res);

/// parse @p s as a `size_t` intergar (positive)
/// @param s 
/// @param[in] res a pointer to store result `size_t` interger, untouched if not successful
/// @returns whether is successful
bool parseSize(const CharSeq s, size_t*res);

/// parse a `slen_t`
bool parseSlenT(const CharSeq s, slen_t*res);

/** like seqEqStr() but ignore case*/
bool seqIEqStr(const CharSeq seq, const char* s);

#endif //#ifndef _STRUTILS_H
