

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
        rec.fname = newCStr(fname);
        rec.data = content;

        addItem(res, rec);
    }
    closedir(dp);
    
    return res;
}

