
#include "cli.h"

Args initArgs(){
    Args res={};
    res.multiLinePara = true;
    CharSeq s, s2;
    initSeq(char, s);
    initSeq(char, s2);
    res.cmd = s;
    res.firstUnknowCmd = s2;

    return res;
}

Args parseArg(const char* args[], int len){
    Args res = initArgs();

    char*c_cmd = NULL;
    for(int i=0; i<len; i++){
        const char* arg = args[i];
        if(arg[0]!='-'){
            res.arg=(char*)arg;
            continue;
        }
        if(STR_EQ(arg, C_FLAG)){
            i++;
            c_cmd = (char*)args[i];
        }else if(strstr(arg, C_FLAG)){
            c_cmd = (char*)(arg+strlen(C_FLAG));

        }else if(strstr(arg, SGL_LNE_FLAG)){
            res.multiLinePara=false;
        }else if(strstr(arg, MUL_LNE_FLAG)){
            res.multiLinePara=true;
        }else if(WANT_HELP(arg)){
            res.help = true;
            goto Ret;
        }else if(WANT_VER(arg)){
            res.version=true;
            goto Ret;
        }else{
            CharSeq unknown = charpToSeq(arg);
            deinitSeq(res.firstUnknowCmd);
            res.firstUnknowCmd = unknown;

            goto Ret;
        }
    }
    if(c_cmd!=NULL){
        CharSeq cmd;
        deinitSeq(res.cmd);
        cmd = charpToSeq(c_cmd);
        res.cmd = cmd;
    }


Ret:
    return res;
    
    
}