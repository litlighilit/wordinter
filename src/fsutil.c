

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "fsutil.h"

CharSeq readAll(FILE* f){
    CharSeq res;
    initSeq(char, res);
    char c;
    while( (c=getc(f))!=EOF ){
      #ifndef READ_KEEP_R
      if(c!='\r')
      #endif
        addItem(res, c);
    }
    return res;
}


char* _newStr(const char* s){
    char*res=malloc(strlen(s)+1);
    strcpy(res,s);
    return res;
}

char* joinPath(const char* dir, const char*fname){
    size_t dLen = strlen(dir);
    char last = dir[dLen-1];
    size_t resLen = dLen+strlen(fname)+1;

    char*res;
    if(!isOsSep(last)){
        res=malloc(++resLen);
        strcpy(res, dir);
        res[dLen]=OS_SEP;
    }else{
        res=malloc(resLen);
        strcpy(res, dir);
    }
    strcat(res, fname);
    return res;
}


RecSeq listDir(const char* dir){
    RecSeq res;
    initSeq(Rec, res);

    DIR* dp=opendir(dir);
    if(dp==NULL){
        return res;
    }
    struct dirent* dire;
    while((dire=readdir(dp)) != NULL){
        //if(dire->d_type == DT_DIR) continue;
        char* fname=dire->d_name;

        if(strcmp(fname, ".")==0 || strcmp(fname, "..")==0) continue;

        char* fpath = joinPath(dir, fname);
        FILE* f=fopen(fpath, "r");
        free(fpath);
        if(f==NULL){
            //fprintf(stderr, "can't open %s as file, skip it\n");
            continue;
        }
        
        CharSeq content = readAll(f);
        fclose(f);

        Rec rec;
        rec.fname = _newStr(fname);
        rec.data = content;

        addItem(res, rec);
    }
    closedir(dp);
    
    return res;
}

