

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fsutils.h"

CharSeq readAll(FILE* f){
    CharSeq res;
    initSeq(char, res);
    int c;
    while( (c=getc(f))!=EOF ){
      #ifndef READ_KEEP_R
      if(c!='\r')
      #endif
        addItem(res, (char)c);
    }
    return res;
}



bool pushFile(RecSeq*p, const char* fpath){
    bool res=true;
    FILE* f=fopen(fpath, "rb");
    if(f==NULL) return false;
    CharSeq content = readAll(f);
    fclose(f);
    
    Rec rec;
    rec.fname = newCStr(fpath);
    rec.data = content;
    
    pAddItem(p, rec);
    return res;
}


void _NoopWithCharp(const char*_nouse){}

// skip . and ..
bool shallSkip(const char*filename){return strcmp(filename, ".")==0 || strcmp(filename, "..")==0;}
#if defined _MSC_VER
#include <windows.h>
#include <io.h>

enum DirScanStat pushInDir(RecSeq*p, const char*dir, void fallback(const char* filename)){

    const char* pattern = joinPath(dir, "*");
    struct _finddata_t fileInfo;
    intptr_t handle = _findfirst(pattern, &fileInfo);

    if (handle == -1L) return dsCantOpen;
    
    if(fallback==NULL) fallback=_NoopWithCharp;

    enum DirScanStat res = dsSucc;
    for(int k=0; k==0; k=_findnext(handle, &fileInfo)){
        char*fname = fileInfo.name;
        if(shallSkip(fname)) continue;

        char* path = joinPath(dir, fname);
        if(dirExists(path)); // if is dir, do nothing
        else{
            bool succ = pushFile(p, path);

            if(!succ){
                res=dsItemSkipped;
                fallback(fname);
            }
        }
        
        free(path);
     }
     
    _findclose(handle);
    free((char*)pattern); // discard const
    return res;
}

//#elif defined(_WIN32) && !defined(__MINGW64_VERSION_MAJOR)
#else

#include <dirent.h>
enum DirScanStat pushInDir(RecSeq*p, const char* dir, void fallback(const char* filename)){

    DIR* dp=opendir(dir);
    if(dp==NULL) return dsCantOpen;

    if(fallback==NULL) fallback=_NoopWithCharp;

    enum DirScanStat res = dsSucc;
    struct dirent* dire;
    while((dire=readdir(dp)) != NULL){
        //if(dire->d_type == DT_DIR) continue;
        char* fname=dire->d_name;

        if(shallSkip(fname)) continue;

        char* path = joinPath(dir, fname);
        if(dirExists(path)); // if is dir, do nothing
        else{
            bool succ = pushFile(p, path);

            if(!succ){
                res=dsItemSkipped;
                fallback(fname);
            }
        }

        free(path);
    }
    closedir(dp);
    
    return res;
}


#endif



RecSeq listDir(const char* dir){
    RecSeq res;
    initSeq(Rec, res);

    pushInDir(&res, dir, NULL);

    return res;
}

void freeListDir(RecSeq rs){
    forIndex(i, rs){
        Rec r = getItem(rs, i);
        free(r.fname);
        deinitSeq(r.data);
    }
    deinitSeq(rs);
}

#if defined _MSC_VER
#include <fileapi.h>
bool dirExists(const char* dir){
	bool res = false;
    DWORD a = GetFileAttributes(dir);
    if(a != -1)
      res = ((a & FILE_ATTRIBUTE_DIRECTORY) != 0);
    return res;
}
#else
#include <sys/stat.h>
bool dirExists(const char* dir){
    struct stat st;
    bool res = stat(dir, &st) >= 0 && S_ISDIR(st.st_mode);
	return res;
}
#endif // #ifdef _MSC_VER
	
