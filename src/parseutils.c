

#include "parseutils.h"

#define RETskipBy(s, check, start) do{\
    int i=0;\
    for(; ; i++){\
        int idx=i+start;\
        if( idx>=s.len || !(check(getItem(s, idx)))  )break;\
    }\
    return i;\
}while(0)

#define RETparseUntilBy(s, check, start) do{\
    CharSeq res;\
    initSeq(char, res);\
    int i=0;\
    for(; ; i++){\
        int idx = i+start;\
        if( idx>=s.len || check(getItem(s, idx)) ) break;\
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
  #define charEq(c) (c==until)
    RETparseUntilBy(s, charEq, start);
  #undef charEq
}

CharSeq parseUntil2(const CharSeq s, char until, int start){
  #define charEq(c) (c==until&& *(&(c)+1)==until )
    RETparseUntilBy(s, charEq, start);
  #undef charEq
}

bool isPunc(char c){ return (19<c&&c<'A' || 'Z'<c && c<'a' || 'z'<c);}

int skipPunc(const CharSeq s, int start){
    RETskipBy(s, isPunc, start);
}

CharSeq parseUntilPunc(const CharSeq s, int start){
    RETparseUntilBy(s, isPunc, start);
}

