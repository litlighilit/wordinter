

#include "parseutils.h"

#define RETskipBy(s, check, start) do{\
    int i=0;\
    for(; ; i++){\
        int idx=i+start;\
        if( idx>=s.len || !(check(getItem(s, idx)))  )break;\
    }\
    return i;\
}while(0)

#define RETparseUntilBy(s, checkCharSeqAt, start) do{\
    CharSeq res;\
    initSeq(char, res);\
    int i=0;\
    for(; ; i++){\
        int idx = i+start;\
        if( idx>=s.len || checkCharSeqAt(s, idx) ) break;\
        addItem(res, getItem(s,idx));\
    }\
    return res;\
}while(0)


int skip(const CharSeq s, char skipped, int start){
  #define charEq(c) (c==skipped)
    RETskipBy(s, charEq, start);
  #undef charEq
}

CharSeq parseUntil(const CharSeq s, char until, int start){
  #define charEq(s, idx) (getItem(s,idx)==until)
    RETparseUntilBy(s, charEq, start);
  #undef charEq
}

CharSeq parseUntil2(const CharSeq s, char until, int start){
  #define charEq(s, idx) (getItem(s, idx)==until && idx+1 < s.len && getItem(s,idx+1)==until)
    RETparseUntilBy(s, charEq, start);
  #undef charEq
}

bool isWordBorder(char c){
  /* ord of char
    ' ' 32
    - 45
    0:48 9:57
    A:65 Z:90 ... */
  return (
  #ifndef DASH_IN_WORD
     c!='-'&&
  #endif
     31<c&&c<'A' ||
    'Z'<c&&c<'a' ||
    'z'<c);
}

int skipW(const CharSeq s, int start){
    RETskipBy(s, isWordBorder, start);
}

#define isWordBorderAt(s, idx) isWordBorder(getItem(s,idx))
CharSeq parseUntilW(const CharSeq s, int start){
    RETparseUntilBy(s, isWordBorderAt, start);
}

