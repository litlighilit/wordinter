

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



bool pushRec(RecSeq*p, const char* dir, const char* fname){
    bool res=true;
    char* fpath = joinPath(dir, fname);
    FILE* f=fopen(fpath, "r");
    if(f==NULL){
        res = false;
        goto End;
    }
    CharSeq content = readAll(f);
    fclose(f);
    
    Rec rec;
    rec.fname = newCStr(fname);
    rec.data = content;
    
    pAddItem(p, rec);
    End:
    free(fpath);
    return res;
}

// skip . and ..
bool shallSkip(const char*filename){return strcmp(filename, ".")==0 || strcmp(filename, "..")==0;}
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
        if(shallSkip(fname)){
            goto Next;
        }
        pushRec(&res, dir, fname);
        
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

        if(shallSkip(fname)) continue;

        pushRec(&res, dir, fname);

    }
    closedir(dp);
    
    return res;
}


#endif


void freeListDir(RecSeq rs){
    forIndex(i, rs){
        Rec r = getItem(rs, i);
        free(r.fname);
        deinitSeq(r.data);
    }
    deinitSeq(rs);
}
