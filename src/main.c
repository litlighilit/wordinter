
#include "cnf.h"

#include <stdio.h>
#include "types.h" // RecSeq
#include "seq.h"
#include "fsutil.h" // listDir
#include "interpreter.h" // enterRepl
#include "cli.h"
#include "msg.h" // err

#include "main.h" // HelpMsg, Version

int main(int argc, char* argv[]){
    if(argc==1) err("please pass a directory path as a argument");

    int passed = 1; // pass argv[0]
    const char** restArgs = (const char**)(argv+passed);
    Args args = parseArg(restArgs, argc-passed);

    if(args.help) goto PHelp;
    else if(args.version) goto PVersion;

    RecSeq rs = listDir(args.arg);
    if(rs.len==0) {
        err("can't read any data from dir");
    }

    Interpreter interp = initInterpreter(rs, args.multiLinePara);
    if( args.cmd.len!=0 ){
        evalCmd(interp, args.cmd);
        deinitSeq(args.cmd);
    }else{
        enterRepl(interp);
    }
    return 0;

PHelp:
    PriInfo(ProjName);
    PriInfo(Version);
    PriInfo(License);
    msgl(HelpMsg);
    return 0;

PVersion:
    msgl(Version);
}

