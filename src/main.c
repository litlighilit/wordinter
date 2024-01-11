
#include "cnf.h"

#include <stdio.h>
#include "types.h" // RecSeq
#include "seq.h"
#include "cmsg.h" // err

#include "main.h" // HelpMsg, Version

int main(int argc, char* argv[]){

    cmsgWarnOnFailCfg(clOnIfEnvAllow);
    
    #define K(arr) arr[1] // get "-x"'s 'x'
    const char singLineP = K(singleLineParaFlag);
    const char filesK = K(filesKey);
    const char codesK = K(codesKey);
    #undef K

    ProjInfo INFO = (ProjInfo){
        .proj=Proj,
        .version=Version,
        .license=License,
        .auther=Author, 
        .desc=Desc,
        .usages=genUsage()
    };

    ArgParser parser = newArgParser(argc, argv, NULL);
    
    for(size_t i=0; i<sizeof(Notes)/sizeof(Notes[0]); i++)
        addNote(parser, Notes[i]);

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
            " but got %" PRI_SLEN "\n"
            " however, you can pass any number of files as input via `-f`", args.args.len);

    bool hasInput=false;

    RecSeq rs;
    initSeq(Rec, rs);



    if(args.args.len!=0){
        CharSeq dirS=getItem(args.args, 0);
        char* dir = cstr(dirS);
        enum DirScanStat ds = reprPushInDir(&rs, dir);
        if(ds!=dsCantOpen) hasInput=true;
        free(dir);
    }

    ListArg files = getListVal(args, filesK);
    forIndex(i, files){
        hasInput=true;
        CharSeq filepath=getItem(files, i);
        char*cfilepath=cstr(filepath);

        bool succ = pushFile(&rs, cfilepath);
        if(!succ) warnCantOpenFile(cfilepath);
        free(cfilepath);
        // do not deinitSeq(char, filepath), as they're owned by args.

    }

    if(!hasInput) warn("neither a directory nor some files is passed."\
         "  please pass some input data (pass `-h` for details)");
    while(!hasInput){
        msg("please input a directory or file path: ");
        CharSeq s = getLine();
        if(s.len==0){
            info("EOF got");
            sayBye();
            exit(0);
        }
        char* path = cstr(s);
        if(path[s.len-1]=='\n') path[s.len-1]='\0'; // rstrip one newline

        bool succ = reprPushPath(&rs, path);
        if(succ) hasInput = true;

        deinitSeq(s);
        free(path);
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

