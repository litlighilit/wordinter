
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

CharSeq subStr(const CharSeq s, slen_t first, slen_t last){
    if(last>s.len-1) last = s.len-1;
    CharSeq res;
    initSeq(char, res);

    for(slen_t i=first; i<=last; i++){
        addItem(res, uncheckedGetItem(s,i));
    }

    return res;

}
CharSeq subStrFrom(const CharSeq s, slen_t start){
    return subStr(s, start, s.len-1);
}

PairS split2(const CharSeq s, char sep){
    PairS res;

    CharSeq lft = parseUntil(s, sep, 0);
     
    res.left = lft;

    slen_t n = skip(s, sep, lft.len);

    res.right = subStrFrom(s, lft.len+n);

    return res;
}

static char QuoM = '"'; // Quotation mark

/** strip a may-be quoted string.
 e.g. `"s"` -> `s`    asd -> asd    `  e` -> `  e`
 \param[out] p a \b not initialized pointer
 \returns number of parsed char
 \retval -1 if quotation mark is not matched (no ending mark is found)
 \retval 0 if @p s is empty
*/
slen_t stripStr(CharSeq*p, const CharSeq s, slen_t start, bool rightUntilEnd){
    if(s.len==0)
        return 0;

    slen_t n=0;
    CharSeq res;
    if(getItem(s,start)==QuoM){
        n++;

        res = parseUntil(s, QuoM, start+n);
        n += res.len;

        if(uncheckedGetItem(s, start+n)!=QuoM) return -1;

        n++;
    }else{
        if(rightUntilEnd) res = subStrFrom(s, start);
        else res = parseUntil(s, ' ', start);
        n += res.len;
    }
    *p = res;

    return n;
}

PairS splitQuo2(const CharSeq s, char sep){
    PairS res;

    slen_t n, pos; // avoid c++'s crosses initialization of ...
    if(s.len==0) goto RetEmpty;

    // then len>=1
    pos = 0; // parse position
    n = stripStr(&res.left, s, pos, false);
    if(n==-1) goto RetEmpty;
    
    pos += n;

    pos += skip(s, sep, pos);

    if(pos>=s.len){
        initSeq(char, res.right);
        return res;
    }

    n = stripStr(&res.right, s, pos, true);
    if(n==-1) goto RetEmpty;

    return res;

    RetEmpty:
        initSeq(char, res.left);
        initSeq(char, res.right);
        return res;
    
}


#define upper(c) (c & -33) // ~('a'-'A') -> -33
bool seqIEqStr(const CharSeq seq, const char* s){
    if(seq.len!=strlen(s)) return false;
    forIndex(i, seq){
        bool eq = upper(uncheckedGetItem(seq, i)) == upper(s[i]);
        if(!eq) return false;
    }
    return true;
}

bool parseInt(const CharSeq s, int* res){
    char*cs=cstr(s);
    int n = sscanf(cs, "%d", res);
    free(cs);
    return n==1;
}

bool parseSize(const CharSeq s, size_t*res){
    char*cs=cstr(s);
    if(s.len>1 && cs[0]=='-') return false;
    int n = sscanf(cs, "%zu", res);
    free(cs);
    return n==1;
}
bool parseSlenT(const CharSeq s, slen_t*res){
    char*cs=cstr(s);
    if(s.len>1 && cs[0]=='-') return false;
    int n = sscanf(cs, "%" PRI_SLEN , res);
    free(cs);
    return n==1;
}
