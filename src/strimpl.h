
#ifndef _STRIMPL_H
#define _STRIMPL_H

/*doctest: include("<assert.h>")
  CharSeq s = charpToSeq("This is a String");
  printCharSeq(s); //-> This is a String
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

#define printCharSeq(s) do{\
    forEach(s, putchar);\
    putchar('\n');\
}while(0)

CharSeq charpToSeq(const char* p);

// return a new c-string
char* cstr(const CharSeq s);

CharSeq copyCharSeq(const CharSeq s);

bool seqEqStr(const CharSeq seq, const char* s);

#endif //#ifndef _STRUTILS_H