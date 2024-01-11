
#ifndef _STREAMS_H
#define _STREAMS_H
/// @file stream.h
/*doctest:

// ## using stream in a C-style
  CharSeq s = charpToSeq("Chapter 12. Good-bye");
  StrStream stream = toStrStream(s);
  
  while(1){
      CharSeq str = tokenToCharSeq(nextWord(&stream));
      if(str.len==0)break;
      printf(":" PRI_SLEN " ", str.len);printlnCharSeq(str);
      deinitSeq(str);
  }
  //-> 7: Chapter
  //-> 2: 12
  //-> 8: Good-bye

  deinitSeq(s);

// ## word-border-sep
  CharSeq s2 = charpToSeq("  This is a paragraph\n\n  And the second one,\nstill the 2nd\n\n\n  begin no.3\nalso 3~~\n\n  4");
  StrStream stream2 = toStrStream(s2);
  
  while(1){
      CharSeq str = tokenToCharSeq(nextPara(&stream2, true));
      if(str.len==0)break;
      printf("%" PRI_SLEN ": ", str.len);printlnCharSeq(str);
      deinitSeq(str);
  }
  //-> 21:   This is a paragraph
  //-> 35:   And the second one,
  //-> still the 2nd
  //-> 21:   begin no.3
  //-> also 3~~
  //-> 3:   4
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
    slen_t idx;
    char sep;
    CharSeq data;
} StrSepStream;

typedef struct{
    slen_t idx;
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

/// @retval `an empty token` if encounter sep in the begining
Token nextToken(StrSepStream* ps);

/// @note ignore ps->sep and treat sapces and punctuations except '-' as sep
/// @returns a token of a word (may empty if met end)
Token nextWord(StrStream* ps);

/// @brief get next paramgragh
/// @param ps pointer to @ref StrStream
/// @param multiLineSeq whether not to treat one line as a paragragh 
/// @returns a token of a paragragh (may empty if met end)
Token nextPara(StrStream* ps, bool multiLineSeq);

#define rewindStream(stream) do{\
  stream.idx=0;\
}while(0)

#endif //#ifndef _STREAMS_H