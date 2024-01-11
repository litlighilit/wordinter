
#ifndef _PARSEUTIL_H
#define _PARSEUTIL_H
/// @file parseutils.h
/*doctest:

// # for one certain char
  CharSeq s = charpToSeq("a   String");

  CharSeq ss = parseUntil(s, ' ', 0);
  printlnCharSeq(ss); //-> a

  deinitSeq(ss); // ss.data destoryed

  slen_t n = skip(s, ' ', ss.len);
  printf("%" PRI_SLEN "\n", n); //-> 3

  ss = parseUntil(s, ' ', ss.len+n);
  printlnCharSeq(ss); //-> String

  deinitSeq(ss);
  deinitSeq(s);

// # for punctuations(not '-') and spaces
  CharSeq s2 = charpToSeq("a,String");

  CharSeq ss2 = parseUntilW(s2, 0);
  printlnCharSeq(ss2);
  
  slen_t n2 = skipW(s2, ss2.len);
  printf("%" PRI_SLEN "\n", n2);


  deinitSeq(ss2);
  deinitSeq(s2);

// # for 2 cahrs as sep
  CharSeq s3 = charpToSeq("a String  another");

  CharSeq ss3 = parseUntil2(s2, ' ', 0);

  printf("%" PRI_SLEN ": ", ss3.len);printlnCharSeq(ss3);
  
  slen_t n3 = skip(s3, ' ',ss3.len);

  printf("%" PRI_SLEN "\n", n3);

  deinitSeq(ss3);
  deinitSeq(s3);


*/
#include <stdbool.h>
#include "strimpl.h"

/// @returns the number of chars skipped
slen_t skip(const CharSeq s, char skipped, slen_t start);

typedef CharSeq (*ParseFunc)(const CharSeq, char, slen_t);

/** Parses a token and return it.
*/
CharSeq parseUntil(const CharSeq s, char until, slen_t start);

/// skip space and punctuations except '-'
slen_t skipW(const CharSeq, slen_t start);

/** parse until word border
  @note e.g. good-bye is considered as one word
*/
CharSeq parseUntilW(const CharSeq s, slen_t start);

/// parse until two @p until characters are met
CharSeq parseUntil2(const CharSeq s, char until, slen_t start);

#endif //#ifndef _PARSEUTIL_H