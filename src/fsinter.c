
#include "fsinter.h"

void warnCantOpenFile(const char*fname){
    warn("can't read '%s' ...", fname);
}

enum DirScanStat reprPushInDir(RecSeq*p, const char* dir){

    enum DirScanStat res = pushInDir(p, dir, warnCantOpenFile);
    if(res==dsCantOpen){
        warn("can't open directory `%s`", dir);
    }else if(res==dsItemSkipped){
        warn("some items in directory `%s` are skipped", dir);
    }
    return res;
}

bool reprPushPath(RecSeq*p, const char* path){
    bool res;
    if(dirExists(path)){
        enum DirScanStat ds = reprPushInDir(p, path);
        if(ds!=dsCantOpen){
            info("files in dir `%s` added", path);
            res = true;
        }
    }else{
         bool succ = pushFile(p, path);
         if(succ){
            info("file added");
            res = true;
         }else warn("file `%s` can't be added", path);
    }
    return res;
}