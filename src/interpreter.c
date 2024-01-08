
#include "msg.h"
#include "interpreter.h"

#define QUE "query"
#define CNT "count"
#define FCY "frequency"
#define LST "list"
#define SRC "source"
#define MOD "mode"
#define HLP "help"
#define ALI "alias"
#define QUI "quit"

#define N_CMD 9
#define N_ALIAS 3
const char* CMD[N_CMD][N_ALIAS] = {
     QUE, "que", "qu"
    ,CNT, "cnt", "c"
    ,FCY, "fcy", "f"
    ,LST, "lst", "l"
    ,SRC, "src", "s"
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
     QUE" WORD                 "SEP "query and print all positions of the word WORD"
    ,CNT" FNAME|ORD N_PARA  "   SEP "count the number of words in N_PARA paragraph of file FNAME or file indexed in ORD"
    ,FCY" WORD                 "SEP "print the frequency of the word WORD"
    ,LST" [FNAME|ORD]          "SEP "print content of FNAME or indexed in ORD or print all filenames"
    ,SRC" DIR|FPATH            "SEP "load all files in DIR or only file"
    ,MOD" [MODE]               "SEP "switch between or switch mode to " ReprModes
    ,HLP" [CMD]                "SEP "print all help or for a certain CMD"
    ,ALI" [CMD]                "SEP "print all alias or for a certain CMD"
    ,QUI"|exit                 "SEP "quit"
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
    msgl(help);
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
            sayBye();
            break;
        }
        line.len = len;
        deinitSeq(line);
    }
}

/// 
/// @param[out] ordp will be set -1 if filename is not found
/// @param[in] rs
/// @param[out] filenamep will be set `NULL` if @p arg is alreadly a order
/// @returns whether @p arg is a file order alreadly
bool _ordFileArg(int*ordp, const CharSeq arg, RecSeq rs, char** const filenamep){
    int fileOrd=-1;
    bool isOrd = parseInt(arg, &fileOrd);

    char*fname=NULL;
    if(!isOrd){
        fname = cstr(arg);
        #define checkFname(rec, _fname) (strcmp(rec.fname, _fname)==0)
        int fileIdx = -1;
        findIndex(fileIdx, rs, fname, checkFname);
        if(fileIdx==-1) fileOrd=-1;
        else fileOrd = fileIdx + 1;
        #undef checkFname
    }
    *filenamep = fname;
    *ordp = fileOrd;
    return isOrd;
}
enum Flag evalCmd(Interpreter* pinterp, const CharSeq cmd){
    enum Flag ret = FSucc;
    if(cmd.len==0) return FEmptyCmd;
    PairS pair = splitQuo2(cmd, ' ');
    CharSeq key = pair.left;
    CharSeq args = pair.right;
    int ord = cmdOrd(key);

    char* fnameArg=NULL;
    int fileArgOrd;
    bool isArgOrd;
    #define check_arg(nCmd) do{\
        if(args.len==0){\
            warn("missing arg! Here is help:");\
            priHelp(nCmd);\
            ret=FMissArg;\
            goto Clean;\
        }\
    }while(0)

    switch (ord)
    {
    case 0:
        check_arg(0);
        char* c_word4qry = cstr(args);
        queryAll(*pinterp, c_word4qry);
        free(c_word4qry);
        break;
    case 1:
        ;PairS si = splitQuo2(args, ' ');

        isArgOrd = _ordFileArg(&fileArgOrd, si.left, pinterp->db, &fnameArg);

        if(fileArgOrd==-1) goto FileNotFound;

        int nPara;
        bool succ = parseInt(si.right, &nPara);
        if(!succ){
            warn("bad int input! You shall input:"); priHelp(1);
            
            ret = FTypeErr;
            goto Clean;
        }

        int cnt = countWordOf(*pinterp, fileArgOrd, nPara);
        if(cnt==FileNotFoundErr){
            FileNotFound:
            if(isArgOrd) warn("no file indexed in %d found", fileArgOrd);
            else warn("no file named '%s' found", fnameArg);
        }else if(cnt==IndexErr) warn("The number %d is out of range", nPara);
        else printCnt(cnt);

        

        break;
    case 2:
        check_arg(2);
        char* c_word4fcy = cstr(args);
        int fcy = countFrequency(*pinterp, c_word4fcy);
        printFcy(fcy);
        free(c_word4fcy);

        break;
    case 3:
        if(args.len==0) fileArgOrd = 0;
        else{
            isArgOrd = _ordFileArg(&fileArgOrd, args, pinterp->db, &fnameArg);
            if(fileArgOrd==-1) goto FileNotFound;
        }

        int nFiles = listFile(*pinterp, fileArgOrd);

        if(nFiles==0) goto FileNotFound;
        else info("%d files listed", nFiles);

        break;
    case 4:
        check_arg(4);

        { // block
        char* path = cstr(args);
        reprPushPath(&pinterp->db, path);
        free(path);
        }

        break;
    case 5:
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
    case 6:
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
    case 7:
        PRI(Alias)
        break;
    case 8:
        ret = FQuit;
        goto Clean;

        break;

    default:
        ;char* c_key=cstr(key);
        warn("unknow command %s, type \"%s\" for help", c_key, HLP);
        free(c_key);
        ret = FUnknowCmd;
    }

Clean:
    free(fnameArg);
    freePairS(pair);
    return ret;
}
