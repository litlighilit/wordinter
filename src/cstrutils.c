
#include <string.h>
#include <stdlib.h>

#include "cstrutils.h"

char* newCStr(const char* s){
    char*res=(char*)malloc(strlen(s)+1);
    strcpy(res,s);
    return res;
}

char* newSubCStr(const char* s, int n){
    size_t len = n;
    size_t slen = strlen(s);
    if(slen<n) len=slen;
    char*res = (char*)malloc(len);
    memcpy(res, s, len);
    res[len]='\0';
    return res;
}

char* catCStr(const char*a, const char*b){
    char*res = (char*)malloc(strlen(a)+strlen(b)+1);
    strcpy(res,a);
    strcat(res, b);

    return res;
}

int rfindChar(const char* s, char c){
    for(int i=strlen(s)-1; i>-1; i--)
        if(c==s[i]) return i;
    return -1;

}

char* joinPath(const char* dir, const char*fname){
    size_t dLen = strlen(dir);
    if(dLen==0) return newCStr(fname);
    char last = dir[dLen-1];
    size_t resLen = dLen+strlen(fname)+1;

    char*res;
    if(!isOsSep(last)){
        res=(char*)malloc(++resLen);
        strcpy(res, dir);
        res[dLen]=OS_SEP;
        res[dLen+1]='\0';
    }else{
        res=(char*)malloc(resLen);
        strcpy(res, dir);
    }
    strcat(res, fname);
    return res;
}

const char* basenameRef( const char* path){
    for(int i=strlen(path)-1; i>-1; i--){
        if(path[i]==UNIX_OS_SEP || path[i]==WIN_OS_SEP) return path+i+1;
    }
    return path;
}