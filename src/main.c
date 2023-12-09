
#include "cnf.h"

#include <stdio.h>
#include "types.h" // RecSeq
#include "fsutil.h" // listDir
#include "interpreter.h" // enterRepl
#include "msg.h" // err


int main(int argc, char* argv[]){
    char* arg = argv[1];
    if(arg==NULL) err("please pass a directory path as a argument");

    RecSeq rs = listDir(arg);
    if(rs.len==0) {
        err("can't read any data from dir");
    }

    int icmd = argc-1;
    if( strcmp(
        argv[icmd-1],
        "-c")==0
    ){
        CharSeq cmd = charpToSeq(argv[icmd]);
        evalCmd(rs, cmd);
    }else{
        enterRepl(rs);
    }

}

