
#ifndef _PARSEUTIL_H
#define _PARSEUTIL_H

/*doctest:

// # for one certain char
  CharSeq s = charpToSeq("a   String");

  CharSeq ss = parseUntil(s, ' ', 0);
  printCharSeq(ss); //-> a

  deinitSeq(ss); // ss.data destoryed

  int n = skip(s, ' ', ss.len);
  printf("%d\n", n); //-> 3

  ss = parseUntil(s, ' ', ss.len+n);
  printCharSeq(ss); //-> String

  deinitSeq(ss);
  deinitSeq(s);

// # for punctuations and spaces
  CharSeq s2 = charpToSeq("a,String");

  CharSeq ss2 = parseUntilPunc(s2, 0);
  printCharSeq(ss2);
  
  int n2 = skipPunc(s2, ss2.len);
  printf("%d\n", n2);


  deinitSeq(ss2);
  deinitSeq(s2);

// # for 2 cahrs as sep
  CharSeq s3 = charpToSeq("a String  another");

  CharSeq ss3 = parseUntil2(s2, ' ', 0);

  printf("%d: ", ss3.len);printCharSeq(ss3);
  
  int n3 = skip(s3, ' ',ss3.len);

  printf("%d\n", n3);

  deinitSeq(ss3);
  deinitSeq(s3);


*/
#include <stdbool.h>
#include "strimpl.h"
// returns the number of chars skipped
int skip(const CharSeq s, char skipped, int start);


/* Parses a token and return it.
*/
CharSeq parseUntil(const CharSeq s, char until, int start);

// XXX: space is treated as a punctuation too
int skipPunc(const CharSeq, int start);

CharSeq parseUntilPunc(const CharSeq s, int start);

CharSeq parseUntil2(const CharSeq s, char until, int start);

#endif //#ifndef _PARSEUTIL_H