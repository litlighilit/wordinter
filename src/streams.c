
#include "streams.h"

#define RETnextWith(ps, untilProc, skipProc) do{\
    CharSeq res;\
    int n=skipProc;\
    ps->idx += n;\
    res = untilProc;\
    \
    if(res.len==0)return toToken(res);\
    ps->idx += res.len;\
    return toToken(res);\
}while(0)

Token nextToken(StrSepStream* ps){
    RETnextWith(ps,
        parseUntil(ps->data, ps->sep, ps->idx),
        skip(ps->data, ps->sep,ps->idx)
    );
}

Token nextWord(StrStream* ps){
    RETnextWith(ps, 
        parseUntilPunc(ps->data, ps->idx),
        skipPunc(ps->data, ps->idx)
    );
}

Token nextPara(StrStream* ps){
    RETnextWith(ps,
        parseUntil2(ps->data, NL, ps->idx),
        skip(ps->data, NL,ps->idx)
    );
}