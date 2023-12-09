
#include <stdio.h> // getchar()
#include "strutils.h"

CharSeq getLine(){
    CharSeq res;
    initSeq(char, res);

    char c;
    while( (c=getchar()) != EOF ){
        if(c=='\n'){
            addItem(res, c);
            goto Ret;
        }
        addItem(res, c);
    }
Ret:
    return res;

}

CharSeq strAdd(const CharSeq s1, const CharSeq s2){
    CharSeq res;
    initSeq(char, res);

    forIndex(i, s1) addItem(res, getItem(s1, i));
    
    forIndex(i, s2) addItem(res, getItem(s2, i));

    res.len = s1.len + s2.len;

    return res;
}

CharSeq strSlice(const CharSeq s, int a){
    CharSeq res;
    initSeq(char, res);

    for(int i=a; i<s.len; i++){
        addItem(res, getItem(s,i));
    }

    return res;

}

PairS split2(const CharSeq s, char sep){
    PairS res;

    CharSeq lft = parseUntil(s, sep, 0);
     
    res.left = lft;

    int n = skip(s, sep, lft.len);

    res.right = strSlice(s, lft.len+n);

    return res;
}



bool parseInt(const CharSeq s, int* res){
    char*cs=cstr(s);
    int n = sscanf(cs, "%d", res);
    free(cs);
    return n==1;
}
