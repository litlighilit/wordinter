
#ifndef _PARSEUTIL_H
#define _PARSEUTIL_H
/// @file parseutils.h
/*doctest:

// # for one certain char
  CharSeq s = charpToSeq("a   String");

  CharSeq ss = parseUntil(s, ' ', 0);
  printlnCharSeq(ss); //-> a

  deinitSeq(ss); // ss.data destoryed

  int n = skip(s, ' ', ss.len);
  printf("%d\n", n); //-> 3

  ss = parseUntil(s, ' ', ss.len+n);
  printlnCharSeq(ss); //-> String

  deinitSeq(ss);
  deinitSeq(s);

// # for punctuations(not '-') and spaces
  CharSeq s2 = charpToSeq("a,String");

  CharSeq ss2 = parseUntilW(s2, 0);
  printlnCharSeq(ss2);
  
  int n2 = skipW(s2, ss2.len);
  printf("%d\n", n2);


  deinitSeq(ss2);
  deinitSeq(s2);

// # for 2 cahrs as sep
  CharSeq s3 = charpToSeq("a String  another");

  CharSeq ss3 = parseUntil2(s2, ' ', 0);

  printf("%d: ", ss3.len);printlnCharSeq(ss3);
  
  int n3 = skip(s3, ' ',ss3.len);

  printf("%d\n", n3);

  deinitSeq(ss3);
  deinitSeq(s3);


*/
#include <stdbool.h>
#include "strimpl.h"

/// @returns the number of chars skipped
int skip(const CharSeq s, char skipped, int start);

typedef CharSeq (*ParseFunc)(const CharSeq, char, int);

/** Parses a token and return it.
*/
CharSeq parseUntil(const CharSeq s, char until, int start);

/// skip space and punctuations except '-'
int skipW(const CharSeq, int start);

/** parse until word border
  @note e.g. good-bye is considered as one word
*/
CharSeq parseUntilW(const CharSeq s, int start);

/// parse until two @p until characters are met
CharSeq parseUntil2(const CharSeq s, char until, int start);

#endif //#ifndef _PARSEUTIL_H