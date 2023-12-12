

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


// can't easly deined as a func
// a tricky macro with a `continue` stmt
#define PushResWithName(res, fname) do{\
        char* fpath = joinPath(dir, fname);\
        FILE* f=fopen(fpath, "r");\
        free(fpath);\
        if(f==NULL){\
            continue;\
        }\
        CharSeq content = readAll(f);\
        fclose(f);\
        \
        Rec rec;\
        rec.fname = newCStr(fname);\
        rec.data = content;\
        \
        addItem(res, rec);\
}while(0);


#ifdef _MSC_VER
#include <io.h>

RecSeq listDir(const char* dir){
    RecSeq res;
    initSeq(Rec, res);

    const char* pattern = joinPath(dir, "*");
    struct _finddata_t fileInfo;
    intptr_t handle = _findfirst(pattern, &fileInfo);

    if (handle == -1L) {
        return res;
    }

    int k;
    do {
        char*fname = fileInfo.name;
        if(strcmp(fname, ".")==0 || strcmp(fname, "..")==0){
            goto Next;
        }
        PushResWithName(res, fname)
        
        Next:
        k = _findnext(handle, &fileInfo);
    } while (k==0);
     
    _findclose(handle);
    free((char*)pattern);
    return res;
}

//#elif defined(_WIN32) && !defined(__MINGW64_VERSION_MAJOR)
#else

#include <dirent.h>
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

        PushResWithName(res, fname)

    }
    closedir(dp);
    
    return res;
}


#endif