
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

#define ReprModes SmultiLinePara "(" smultiLinePara ") or "\
        SsingleLinePara "(" ssingleLinePara ")"

#define ICASE_FLAG "-i"
#define ICASE_HELP " if " ICASE_FLAG " is given, ignore case"

#define SEP "\t:"
const char* HELP[N_CMD] = {
     QUE" [" ICASE_FLAG "] WORD            " SEP "query and print all positions of the word WORD." ICASE_HELP
    ,CNT" FNAME|ORD N_PARA  "    SEP "count the number of words in N_PARA paragraph of file FNAME or file indexed in ORD"
    ,FCY" [" ICASE_FLAG "] WORD            " SEP "print the frequency of the word WORD." ICASE_HELP
    ,LST" [FNAME|ORD] [PARA]   " SEP "print all (or Para.PARA) content of FNAME or indexed in ORD or print all filenames"
    ,SRC" DIR|FPATH            " SEP "load all files in DIR or only file"
    ,MOD" [MODE]               " SEP "switch between or switch mode to " ReprModes
    ,HLP" [CMD]                " SEP "print all help or for a certain CMD"
    ,ALI" [CMD]                " SEP "print all alias or for a certain CMD"
    ,QUI"|exit                 " SEP "quit"
};

bool priHelp(size_t n){
    if(!(n<N_CMD)) return false;
    
    msgl(HELP[n]);
    return true;
}

#define N_COL_LEN 10 // shall bigger than the max len of any of CMD
bool priAlias(size_t n){
    if(!(n<N_CMD)) return false;
    
    for(size_t j=0; j<N_ALIAS; j++)
        msg("%-*s ", N_COL_LEN, CMD[n][j]);
    msg("\n");
    return true;
}

size_t cmdOrd(const CharSeq cmd){
    size_t res = -1;
    for(size_t i=0; i<N_CMD; i++){
        for(size_t j=0; j<N_ALIAS; j++){
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
    size_t ord = cmdOrd(dest);
    const char*help;
    bool found = ord==-1;
    if(found){
        help=(char*)HELP[ord];
    }else{
        help="unknow cmd";
    }
    msgl(help);
    return found;
}

void printCnt(size_t cnt){msgl("all word count: %zu", cnt);}
void printFcy(size_t fre){msgl("word frequency: %zu", fre);}

void printPs(){msg(">> ");}

const char* getModeS(const Interpreter* pinterp) {return pinterp->multiLinePara?SmultiLinePara:SsingleLinePara;}

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
    const char* mode_s = getModeS(pinterp);
    msgl("entering repl in %s mode...", mode_s);
    CharSeq line;
    while( printPs(), (line=getLine()), line.len!=0 ){
        slen_t len = line.len;
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
/// @param[out] ordp will be set ((slen_t)-1) if filename is not found
/// @param[in] rs
/// @param[out] filenamep will be set `NULL` if @p arg is alreadly a order
/// @returns whether @p arg is a file order alreadly
static bool _ordFileArg(slen_t*ordp, const CharSeq arg, RecSeq rs, char** const filenamep){
    slen_t fileOrd=-1;
    bool isOrd = parseSlenT(arg, &fileOrd);

    char*fname=NULL;
    if(!isOrd){
        fname = cstr(arg);
        #define checkFname(rec, _fname) (strcmp(rec.fname, _fname)==0)
        slen_t fileIdx = -1;
        findIndex(fileIdx, rs, fname, checkFname);
        if(fileIdx==-1) fileOrd=-1;
        else fileOrd = fileIdx + 1;
        #undef checkFname
    }
    *filenamep = fname;
    *ordp = fileOrd;
    return isOrd;
}

static void priFileNotFound(bool isArgOrd, slen_t fileArgOrd, const char* fnameArg){
    if(isArgOrd) warn("no file indexed in %" PRI_SLEN " found", fileArgOrd);
    else warn("no file named '%s' found", fnameArg);
}

enum Flag evalCmd(Interpreter* pinterp, const CharSeq cmd){
    enum Flag ret = FSucc;
    if(cmd.len==0) return FEmptyCmd;
    PairS pair = splitQuo2(cmd, ' ');
    CharSeq key = pair.left;
    CharSeq args = pair.right;
    size_t ord = cmdOrd(key);

    char* fnameArg=NULL;
    slen_t fileArgOrd;
    bool isArgOrd;
    size_t cnt;
    bool succParseInt;
    PairS si;
    enum lfpIndexErr lfpErr;
    #define check_arg(nCmd) do{\
        if(args.len==0){\
            warn("missing arg! Here is help:");\
            priHelp(nCmd);\
            ret=FMissArg;\
            goto Clean;\
        }\
    }while(0)

    /**
     @param[out] argIn char*
     @param[in] flag bool if get @p flagStr, assign as true
    */
    #define parse_flag(argIn, flag, flagStr) do{\
        si = splitQuo2(args, ' ');\
        CharSeq word;\
        if(si.right.len==0) word=si.left;\
        else{\
            if(!seqEqStr(si.left, flagStr)){\
                char* barg = cstr(si.left);\
                warn("bad arg given, only %s is expected, but got %s", flagStr, barg);\
                free(barg);\
                goto Clean;\
            }\
            flag = true;\
            word = si.right;\
        }\
        argIn = cstr(word);\
    }while(0)

    #define _PriNoFileFound() priFileNotFound(isArgOrd, fileArgOrd, fnameArg)
    switch (ord)
    {
    case 0:
        {
        check_arg(0);
        bool ignoreCase=false;
        char* c_word4qry;
        parse_flag(c_word4qry, ignoreCase, ICASE_FLAG);
        bool hasPos = queryAll(*pinterp, c_word4qry, ignoreCase);
        const char* modePs = ignoreCase?"(even ignoring cases)":"";
        if(!hasPos) info("no one of `%s` found %s", c_word4qry, modePs);
        free(c_word4qry);
        }
        break;
    case 1:
        check_arg(1);
        si = splitQuo2(args, ' ');

        isArgOrd = _ordFileArg(&fileArgOrd, si.left, pinterp->db, &fnameArg);

        if(fileArgOrd==-1)_PriNoFileFound();
        else{
            size_t nPara;
            succParseInt = parseSize(si.right, &nPara);
            if(!succParseInt){
                warn("bad positive int input! You shall input:"); priHelp(1);
                
                ret = FTypeErr;
                freePairS(si);
                goto Clean;
            }

            cnt = countWordOf(*pinterp, fileArgOrd, nPara);
            if(cnt==FileNotFoundErr)_PriNoFileFound();
            else if(cnt==IndexErr) warn("The number %zu is out of range", nPara);
            else printCnt(cnt);
        }
        freePairS(si);

        break;
    case 2:
        {
        check_arg(2);
        char* c_word4fcy;
        bool ignoreCase = false;
        parse_flag(c_word4fcy, ignoreCase, ICASE_FLAG);
        size_t fcy = countFrequency(*pinterp, c_word4fcy, ignoreCase);
        printFcy(fcy);
        free(c_word4fcy);
        }
        break;
    case 3:
        if(args.len==0) fileArgOrd = 0;
        else{
            isArgOrd = _ordFileArg(&fileArgOrd, args, pinterp->db, &fnameArg);
            if(fileArgOrd==-1) _PriNoFileFound();

            si = splitQuo2(args, ' ');
        }

        if(fileArgOrd==0||si.right.len==0){
            size_t nFiles = listFile(*pinterp, fileArgOrd);

            if(nFiles==0)_PriNoFileFound();
            else info("%zu files listed", nFiles);
        }else{
            // use cnt as `para` to avoid defining a new symbol
            //  which will be complained by c++ compiler
            succParseInt = parseSize(si.right, &cnt);
            if(!succParseInt){
                warn("bad positive int input! You shall input:");
                priHelp(3);
            }else{
                lfpErr = listFilePara(*pinterp, fileArgOrd, cnt);
                if(lfpErr==lfpiePara) warn("out of paragraph range");
                else if (lfpErr==lfpieFile)_PriNoFileFound();
                else info("Para %zu listed", cnt);
            }
            freePairS(si);
        }
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
            info("switch to mode '%s'", getModeS(pinterp));
        }else{
            int multiMode = isMultiStr(args);
            if(multiMode==-1){
                warn("got unknown mode. please run 'help mode' for help");
                break;
            }
            pinterp->multiLinePara=multiMode;
        }
        break;
    case 6:
    #define PRI(dest) do{\
        if(args.len==0){\
            msgl("all %s:", #dest);\
            for(size_t i=0; i<N_CMD; i++) pri##dest(i);\
        }else{\
            size_t _destOrd = cmdOrd(args);\
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
        if(args.len!=0){
            warn("quit accepts no argument! Maybe you want another cmd?");
        }else ret = FQuit;
        goto Clean;

        break;

    default:
        {
        char* c_key=cstr(key);
        warn("unknow command %s, type \"%s\" for help", c_key, HLP);
        free(c_key);
        ret = FUnknowCmd;
        }
    }

Clean:
    free(fnameArg);
    freePairS(pair);
    return ret;

}
