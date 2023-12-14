
#ifndef _STREAMS_H
#define _STREAMS_H

/*doctest:

// ## using stream in a C-style
  CharSeq s = charpToSeq("This is a String, and some others.");
  StrStream stream = toStrStream(s);
  
  while(1){
      CharSeq str = tokenToCharSeq(nextWord(&stream));
      if(str.len==0)break;
      printf("%d: ", str.len);printlnCharSeq(str);
      deinitSeq(str);
  }
  //-> 4: This
  //-> 2: is
  //-> 1: a
  //-> 6: String
  //-> 3: and
  //-> 4: some
  //-> 6: others

  deinitSeq(s);

// ## punctuation-sep
  CharSeq s2 = charpToSeq("  This is a paragraph\n\n  And the second one,\nstill the 2nd\n\n\n  begin no.3\nalso 3~~\n\n  4");
  StrStream stream2 = toStrStream(s2);
  
  while(1){
      CharSeq str = tokenToCharSeq(nextPara(&stream2), true);
      if(str.len==0)break;
      printf("%d: ", str.len);printlnCharSeq(str);
      deinitSeq(str);
  }
  //-> 4: This
  //-> 2: is
  //-> 1: a
  //-> 6: String
  //-> 3: and
  //-> 4: some
  //-> 6: others
  deinitSeq(s2);

// ## NOTE
// always firstly `skip` the sep if sep may exist in the begining

*/

#include <stdbool.h>
#include "strimpl.h"
#include "parseutils.h"

#define NL '\n' // XXX: data is treated to contain no '\r'
                //  ensured by "fsutil"'s readAll

typedef CharSeq Token;

#define toToken(char_seq) char_seq
#define tokenToCharSeq(tok) tok

typedef struct{
    int idx;
    char sep;
    CharSeq data;
} StrSepStream;

typedef struct{
    int idx;
    CharSeq data;
} StrStream;

#define toStrSepStream(s, _sep) ((StrSepStream){.idx=0, .sep=_sep, .data=s})

#define toStrStream(s) ((StrStream){.idx=0, .data=s})

#define forSepStream(seqVar, stream, body) do{\
    while(1){\
      CharSeq seqVar = tokenToCharSeq(nextToken(&stream));\
      if(seqVar.len==0)break;\
      do{body;}while(0);\
      deinitSeq(seqVar);\
    }\
}
#define forStream(seqVar, stream, next, body) do{\
    while(1){\
      CharSeq seqVar = tokenToCharSeq(next(&stream));\
      if(seqVar.len==0)break;\
      do{body;}while(0);\
      deinitSeq(seqVar);\
    }\
}while(0)

// return empty token if encounter sep in the begining
Token nextToken(StrSepStream* ps);

// ignore ps->sep and treat all punctuations as sep
Token nextWord(StrStream* ps);

// 
Token nextPara(StrStream* ps, bool multiLineSeq);

#define rewindStream(stream) do{\
  stream.idx=0;\
}while(0)

#endif //#ifndef _STREAMS_H