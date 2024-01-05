
#include "cnf.h"

#include <stdio.h>
#include "types.h" // RecSeq
#include "seq.h"
#include "msg.h" // err

#include "main.h" // HelpMsg, Version


void warnCantOpenFile(const char*fname){
    warn("can't read '%s' ...", fname);
}

void reprPushInDir(RecSeq*p, const char* dir){

    enum DirScanStat st = pushInDir(p, dir, warnCantOpenFile);
    if(st==CantOpen){
        warn("can't open directory `%s`", dir);
        return;
    }else if(st==ItemSkipped){
        warn("some items in directory `%s` are skipped", dir);
    }
}

int main(int argc, char* argv[]){
    
    #define K(arr) arr[1] // get "-x"'s 'x'
    const char singLineP = K(singleLineParaFlag);
    const char filesK = K(filesKey);
    const char codesK = K(codesKey);
    #undef K

    ProjInfo INFO = (ProjInfo){
        .auther=Author, 
        .license=License,

        .proj=Proj,
        .version=Version,
        .desc=Desc,
        //.usages=... // will (and have to be) set within `main` function
    };

    ArgParser parser = newArgParser(argc, argv, detailsHelp);

    INFO.usages = genUsage(); 
    enInfo(parser, INFO);

    addBoolOpt(parser, singLineP, lHelp);
    addArgHelp(parser, "dir", "dir to process");

    addListKey(parser, codesK, "CODE", cHelp);
    addArgMap(parser, codesK, "--code");

    addListKey(parser, filesK, "FILE", fHelp);
    addArgMap(parser, filesK, "--file");

    ParseArgRes args = parseArgs(parser);
    if(args.err!=NULL) err("error: %s\n", args.err);
    if(args.args.len>1)
        err("only one positional argument as a directory to read from is allowed,\n"
            " but got %d\n"
            " however, you can pass any number of files as input via `-f`", args.args.len);

    bool hasInput=false;

    RecSeq rs;
    initSeq(Rec, rs);



    if(args.args.len!=0){
        CharSeq dirS=getItem(args.args, 0);
        char* dir = cstr(dirS);
        reprPushInDir(&rs, dir);
        hasInput=true;
        free(dir);
    }

    ListArg files = getListVal(args, filesK);
    forIndex(i, files){
        hasInput=true;
        CharSeq filepath=getItem(files, i);
        char*cfilepath=cstr(filepath);

        bool succ = pushRec(&rs, "", cfilepath);
        if(!succ) 
        free(cfilepath);
        // do not deinitSeq(char, filepath), as they're owned by args.

    }

    if(!hasInput) warn("neither a directory nor some files is passed."\
         "  please pass some input data (pass `-h` for details)");
    while(!hasInput){
        printf("please input a directory: ");
        CharSeq s = getLine();
        if(s.len==0) continue;
        char* dir = cstr(s);
        if(dir[s.len-1]=='\n') dir[s.len-1]='\0'; // rstrip one newline

        reprPushInDir(&rs, dir);

        hasInput = true;

        deinitSeq(s);
        free(dir);
    }


    bool b=wantOpt(args, singLineP); // here, cause as we consider -L as default, this wantOpt can make what we want
                                     //  otherwise(if we want -l as default), it can't be equalified
    bool multiLinePara = !b;

    if(rs.len==0) {
        err("can't read any data from dir");
    }

    Interpreter interp = initInterpreter(rs, multiLinePara);


    ListArg cmds = getListVal(args, codesK);

    if( cmds.len!=0 ){
        forIndex(i, cmds){
            CharSeq cmd = getItem(cmds, i);
            evalCmd(&interp, cmd);
        }
    }else{
        enterRepl(&interp);
    }
    deInfo(INFO);
    freeParseArgRes(args);
    freeArgParser(parser);
    freeListDir(rs);
    return 0;

}

