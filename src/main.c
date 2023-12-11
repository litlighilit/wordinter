
#include "cnf.h"

#include <stdio.h>
#include "types.h" // RecSeq
#include "seq.h"
#include "fsutil.h" // listDir
#include "interpreter.h" // enterRepl
#include "aparsearg.h"
#include "msg.h" // err

#include "main.h" // HelpMsg, Version

int main(int argc, char* argv[]){
    
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
    if(args.args.len!=0){
        CharSeq dirS=getItem(args.args, 0);
        char* dir = cstr(dirS);
        rs = listDir(dir);
        hasInput=true;
        free(dir);
    }else{
        initSeq(Rec, rs);
    }

    ListArg files = getListVal(args, filesK);
    forIndex(i, files){
        hasInput=true;
        CharSeq filepath=getItem(files, i);
        char*cfilepath=cstr(filepath);
        FILE* fp = fopen(cfilepath, "r");
        if(fp==NULL){
            warn("can't read '%s' ...", cfilepath);
        }
        CharSeq content = readAll(fp);
        Rec rec={.fname=cfilepath, .data=content};
        addItem(rs, rec);
        // do not free(cfilepath) and deinitSeq(char, content), as they're owned by rec now.

    }
    if(!hasInput) err("neither a dir nor some files is passed. please pass some input data");


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
    return 0;

}

