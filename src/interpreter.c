
#include "interpreter.h"

#define QUE "query"
#define CNT "count"
#define FCY "frequency"
#define LST "list"
#define MOD "mode"
#define HLP "help"
#define ALI "alias"
#define QUI "quit"

#define N_CMD 8
#define N_ALIAS 3
const char* CMD[N_CMD][N_ALIAS] = {
     QUE, "que", "qu"
    ,CNT, "cnt", "c"
    ,FCY, "fcy", "f"
    ,LST, "lst", "l"
    ,MOD, "mod", "m"
    ,HLP, "hel", "h"
    ,ALI, "ali", "a"
    ,QUI, "exit", "q"
};

#define SmultiLinePara "multiLinePara"
#define smultiLinePara "L"
#define SsingleLinePara "singleLinePara"
#define ssingleLinePara "l"

#define ReprModes SmultiLinePara"("smultiLinePara") or "\
        SsingleLinePara"("ssingleLinePara")"

#define SEP "\t:"
const char* HELP[N_CMD] = {
     QUE" WORD         "  SEP "query and print all positions of the word WORD"
    ,CNT" FNAME N_PARA "  SEP "count the number of words in N_PARA paragraph of file FNAME"
    ,FCY" WORD         "  SEP "print the frequency of the word WORD"
    ,LST" [FNAME]      "  SEP "print content of FNAME or print all filenames"
    ,MOD" [MODE]       "  SEP "switch between or switch mode to " ReprModes
    ,HLP" [CMD]        "  SEP "print all help or for a certain CMD"
    ,ALI" [CMD]        "  SEP "print all alias or for a certain CMD"
    ,QUI"|exit         "  SEP "quit"
};

bool priHelp(int n){
    if(!(-1<n && n<N_CMD)) return false;
    
    msgl(HELP[n]);
    return true;
}

#define N_COL_LEN 10 // shall bigger than the max len of any of CMD
bool priAlias(int n){
    if(!(-1<n && n<N_CMD)) return false;
    
    for(int j=0; j<N_ALIAS; j++)
        msg("%-*s ", N_COL_LEN, CMD[n][j]);
    msg("\n");
    return true;
}

int cmdOrd(const CharSeq cmd){
    int res = -1;
    for(int i=0; i<N_CMD; i++){
        for(int j=0; j<N_ALIAS; j++){
            if(seqEqStr(cmd, CMD[i][j])){
                res = i;
                goto Ret;
            }
        }
    }

Ret:
    return res;
}

bool priHelpOf(const CharSeq dest){
    int ord = cmdOrd(dest);
    char*help;
    bool found = ord==-1;
    if(found){
        help=(char*)HELP[ord];
    }else{
        help="unknow cmd";
    }
    msgl(HELP[ord]);
    return found;
}

void printCnt(int cnt){msgl("all word count: %d", cnt);}
void printFcy(int fre){msgl("word frequency: %d", fre);}

void printPs(){msg(">> ");}

char* getModeS(const Interpreter* pinterp) {return pinterp->multiLinePara?SmultiLinePara:SsingleLinePara;}

// return -1 if unknown
int isMultiStr(const CharSeq s){
    if(seqEqStr(s, smultiLinePara)
     ||seqEqStr(s, SmultiLinePara)){
        return 1;
    }
    if(seqEqStr(s, ssingleLinePara)
     ||seqEqStr(s, SsingleLinePara)){
        return 0;
    }
    return -1;

}

void enterRepl(Interpreter* pinterp){
    char* mode_s = getModeS(pinterp);
    msgl("entering repl in %s mode...", mode_s);
    CharSeq line;
    while( printPs(), (line=getLine()), line.len!=0 ){
        int len = line.len;
        line.len = len-1;
        enum Flag f = evalCmd(pinterp, line);
        if(f==FQuit){
            msgl("bye.");
            break;
        }
        line.len = len;
        deinitSeq(line);
    }
}

enum Flag evalCmd(Interpreter* pinterp, const CharSeq cmd){
    enum Flag ret = FSucc;
    if(cmd.len==0) return FEmptyCmd;
    PairS pair = splitQuo2(cmd, ' ');
    CharSeq key = pair.left;
    CharSeq args = pair.right;
    int ord = cmdOrd(key);
    switch (ord)
    {
    case 0:
        if(args.len==0){
            warn("missing arg! Here is help:");
            priHelp(0);
            ret=FMissArg;
            goto Clean;
        }
        char* c_word4qry = cstr(args);
        queryAll(*pinterp, c_word4qry);
        free(c_word4qry);
        break;
    case 1:
        ;PairS si = splitQuo2(pair.right, ' ');
        int nPara;

        char*fname = cstr(si.left);

        bool succ = parseInt(si.right, &nPara);
        if(!succ){
            warn("bad int input! You shall input:");
            priHelp(1);
            free(fname);
            ret = FTypeErr;
            goto Clean;
        }

        int cnt = countWordOf(*pinterp, fname, nPara);
        if(cnt==FileNotFoundErr) warn("no file named '%s' found", fname);
        else if(cnt==OverRangeErr) warn("The number %d is out of range", nPara);
        else printCnt(cnt);

        free(fname);

        break;
    case 2:
        if(args.len==0){
            priHelp(2);
            ret=false;
            goto Clean;
        }
        char* c_word4fcy = cstr(args);
        int fcy = countFrequency(*pinterp, c_word4fcy);
        printFcy(fcy);
        free(c_word4fcy);

        break;
    case 3:
        ;char*fnameOrNULL;
        if(args.len==0) fnameOrNULL=NULL;
        else fnameOrNULL=cstr(args);

        int nFiles = listFile(*pinterp, fnameOrNULL);

        if(nFiles==0) warn("no file named '%s' found", fnameOrNULL);
        else info("%d files listed", nFiles);

        free(fnameOrNULL);
        break;
    case 4:
        if(args.len==0){
            pinterp->multiLinePara = !pinterp->multiLinePara;
            goto PriLineMode;
        }
        int multiMode = isMultiStr(args);
        if(multiMode==-1){
            warn("got unknown mode. please run 'help mode' for help");
            break;
        }
        pinterp->multiLinePara=multiMode;
        
      PriLineMode:
        info("switch to mode '%s'", getModeS(pinterp));
        break;
    case 5:
    #define PRI(dest) do{\
        if(args.len==0){\
            msgl("all %s:", #dest);\
            for(int i=0; i<N_CMD; i++) pri##dest(i);\
        }else{\
            int _destOrd = cmdOrd(args);\
            if(_destOrd==-1) warn("unknow %s", #dest);\
            else pri##dest(_destOrd);\
        }\
    }while(0);

        PRI(Help)
        break;
    case 6:
        PRI(Alias)
        break;
    case 7:
        return FQuit;
        break;

    default:
        ;char* c_key=cstr(key);
        warn("unknow command %s, type \"%s\" for help", c_key, HLP);
        free(c_key);
        ret = FUnknowCmd;
    }

Clean:
    freePairS(pair);
    return ret;
}
