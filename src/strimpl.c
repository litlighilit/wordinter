
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strimpl.h"

bool seqEqStr(const CharSeq seq, const char*s){
    if(seq.len!=strlen(s)) return false;
    return memcmp(seq.data, s, seq.len)==0;
}

#define _initStr(s) (s.itemSize = sizeof(char))
CharSeq charpToSeq(const char* p){
    CharSeq res;
    //initSeq(char, res);
    _initStr(res);
    res.len = strlen(p);
    res.cap = res.len+1;// avoid crash if addItem
    res.data = (char*)malloc(res.cap);
    memcpy(res.data, p, res.len);
    return res;
}

char* cstr(const CharSeq s){
    char* res = (char*)malloc(s.len+1);
    memcpy(res, s.data, s.len);
    res[s.len]='\0';
    return res;
}

CharSeq copyCharSeq(const CharSeq s){
    CharSeq res;
    _initStr(res);
    res.cap = s.cap;
    res.len = s.len;
    res.data = (char*)malloc(res.len);
    memcpy(res.data, s.data, s.len);
    return res;
}
