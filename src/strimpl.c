
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strimpl.h"

bool seqEqStr(const CharSeq seq, const char*s){
    if(seq.len!=strlen(s)) return false;
    return memcmp(getData(seq), s, seq.len)==0;
}

CharSeq charpToSeq(const char* p){
    CharSeq res;
    //initSeq(char, res);
    res.len = strlen(p);
    res.data = malloc(res.len);
    res.cap = res.len;
    memcpy(res.data, p, res.len);
    return res;
}

char* cstr(const CharSeq s){
    char* res=malloc(s.len+1);
    memcpy(res, getData(s), s.len);
    res[s.len]='\0';
    return res;
}

CharSeq copyCharSeq(const CharSeq s){
    CharSeq res;
    res.cap = s.cap;
    res.len = s.len;
    res.data = malloc(res.len);
    forIndex(idx, s){
        setItem(res, idx, getItem(s, idx));
    }
    return res;
}
