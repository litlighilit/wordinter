
#ifndef _STRIMPL_H
#define _STRIMPL_H

/*doctest: include("<assert.h>")
  CharSeq s = charpToSeq("This is a String");
  printlnCharSeq(s); //-> This is a String

  CharSeq fmtS = charpToSeq("A number '%d'");
  printfCharSeq(fmtS, 3); //->A number '3'

  deinitSeq(fmtS);

  addItem(s, ' ');
  addItem(s, 'a');


  assert( !seqEqStr(s, "This is A String a") );
  assert( seqEqStr(s, "This is a String a") );

  assert( !seqEqStr(s, "This") );
  assert( !seqEqStr(s, "This is a String abc") );

*/

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "seq.h"

typedef Seq(char) CharSeq;

// no newline
#define printCharSeq(s) forEach(s, putchar)

#define printlnCharSeq(s) do{\
    printCharSeq(s);\
    putchar('\n');\
}while(0)

// a CharSeq as the format string
#define printfCharSeq(s,...) do{\
    char*cs=cstr(s);\
    printf(cs,##__VA_ARGS__);\
    free(cs);\
}while(0)

CharSeq charpToSeq(const char* p);

// return a new c-string
char* cstr(const CharSeq s);

CharSeq copyCharSeq(const CharSeq s);

bool seqEqStr(const CharSeq seq, const char* s);

#endif //#ifndef _STRUTILS_H