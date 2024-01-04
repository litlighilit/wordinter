
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include "aparsearg.h"

#define loopMapLen(var) for(int var=0; var<MapLen; var++)

#define IND "  " //indent

#define strAppendp(seq, p) do{\
    for(int i=0; i<strlen(p); i++){\
      addItem((seq), p[i]);\
    }\
}while(0)

bool AlwaysGoOn(const char* _){return true;}

ArgParser newArgParser(int argc, char* const argv[],
     const char* helpOrNul
){
    ArgParser res = malloc(sizeof(ArgParserObj));
    res->preArgHook=AlwaysGoOn;
    CharSeq helps;
    if(helpOrNul!=NULL){
        helps=charpToSeq("helps:\n"IND);
        strAppendp(helps, helpOrNul);
        addItem(helps, '\n');
    } else initSeq(char, helps);
    res->helps = helps;

    res->argc = argc;
    res->argv = argv;

    loopMapLen(i){
        res->map[i]=
        res->boolOptAdded[i]=NULL;

        res->listKeyAdded[i]=
        res->keyAdded[i]=(NamedHelp){NULL, NULL};
    }

    return res;

}


void freeArgParser(ArgParser parser){
    deinitSeq(parser->helps);
    free(parser);
}

ParseArgRes newParseArgRes(){
    ParseArgRes res = {0};
    initSeq(CharSeq, res.args);
    for(int i=0; i<MapLen; i++) initSeq(CharSeq, res.keyListvals[i]);
    res.err=NULL;
    return res;
}

void freeParseArgRes(ParseArgRes res){
    free(res.err);
    forIndex(i, res.args){
        deinitSeq(getItem(res.args, i));
    }
    deinitSeq(res.args);
    for(int i=0; i<MapLen; i++){
        forIndex(j, res.keyListvals[i])
            deinitSeq(getItem(res.keyListvals[i], j));
        deinitSeq(res.keyListvals[i]);
    }
}

void addArgMap(ArgParser parser, char shortOpt, const char* longOpt){
    assert( checkOption(shortOpt) );

    const char** map = parser->map;
    int nhash = _MapOrd(shortOpt);
    assert(map[nhash]==NULL); // avoid overwrite

    const char*long_;
    if(longOpt[0]=='-'){
        assert(longOpt[1]=='-');
        long_=longOpt+2;
    }else long_=longOpt;
    map[nhash] = long_;

}

#define _hasDef(s) (s!=DefV) // can't use `s>DefV` !! as DefV is a big positive int
void addBoolOpt(ArgParser parser, char shortOpt, const char* help){
    assert( checkOption(shortOpt) );
    assert(help!=NULL);
    parser->boolOptAdded[_MapOrd(shortOpt)]=help;
}
#define NameHelpSep ((char)1)
void addKey(ArgParser parser, char shortKey, const char*name, const char* help){
    assert(name!=NULL && help!=NULL);
    assert( checkOption(shortKey) );
    parser->keyAdded[_MapOrd(shortKey)] = (NamedHelp){name, help};
}
void addListKey(ArgParser parser, char shortKey, const char*name, const char* help){
    assert(name!=NULL && help!=NULL);
    assert( checkOption(shortKey) );
    parser->listKeyAdded[_MapOrd(shortKey)] = (NamedHelp){name,help};
}

#define tab3 "\t\t\t"
void addArgHelp(ArgParser parser, const char* itemName, const char* help){
    assert(help!=NULL);
    
    static bool PreAdded = false; // ensure once
    static char* Pre = "args:\n";
    if(!PreAdded){
        strAppendp(parser->helps, Pre);
        PreAdded=true;
    }
     
        
    strAppendp(parser->helps, IND); // indent
    char* prehelp = catCStr(itemName, tab3);
    char* thehelp = catCStr(prehelp, help);

    strAppendp(parser->helps, thehelp);
    addItem(parser->helps, '\n');
    free(prehelp);
    free(thehelp);

}

ParseArgRes parseArgs(ArgParser parser){
    ParseArgRes res=newParseArgRes();

    char* const* argv = parser->argv;
    for(int i=1; i < parser->argc; i++){
        char* longOpt = NULL; // for message feedback for error
        const char* arg = argv[i];
        if(!parser->preArgHook(arg)) exit(0);

        if(arg[0]=='-'){
            bool positive=true;//init
            char sht; // short option
            const char*val=NULL;
            if(arg[1]=='-'){ // if long opt
                const char*opt=arg+2;
                int iEq = findChar(opt, '=');
                if(iEq!=-1){
                    longOpt = newSubCStr(opt, iEq);
                    val=opt+iEq+1;
                    goto SelectKeyval;
                }
                longOpt = newCStr(opt);
                sht=-1;
                _MapSearchKey(sht, parser->map, opt);
                if(sht==-1){
                    res.err=catCStr("unknown long option: ",longOpt);
                    goto ClnRet;
                }
                
            }else{
                if(strlen(arg)>2){// not impl multi-short option
                    res.err="haven't supported multiply short option";
                    goto ClnRet;
                }
                sht=arg[1];
                if( checkOpOption(sht) ){
                    sht+='a'-'A';
                    positive = false;
                }
                if(!checkOption(sht)){
                    goto UnknownOpt;
                }
            }
            
            if(parser->boolOptAdded[_MapOrd(sht)]!=NULL){
                res.options[_MapOrd(sht)]=positive;
                continue;
            }
            SelectKeyval:

            if(val==NULL) val=argv[++i];
            if(!positive) goto UnknownOpt;
            if(val==NULL) goto NoValErr;
            int ord = _MapOrd(sht);
            if(parser->keyAdded[_MapOrd(sht)].name!=NULL){
                res.keyvals[ord]=val;
                continue;
            }
            if(parser->listKeyAdded[_MapOrd(sht)].name!=NULL){
                addItem(res.keyListvals[ord], charpToSeq(val));

                continue;
            }

            // fallthrough
            UnknownOpt:
                ;char*pre = "unknow option: ";
                size_t eLen = strlen(pre)+1;
                char*err=malloc(eLen+1);
                if(!positive) sht = sht - 'a' + 'A';
                sprintf(err, "%s%c", pre, sht);
                res.err=err;
                goto ClnRet;

            NoValErr:
                ;char*opt; 
                if(longOpt) opt=newCStr(longOpt);
                else {
                    opt=malloc(2);
                    opt[0]=sht;
                    opt[1]='\0';
                }
                res.err=catCStr("expected a value after the key: ", opt);
                free(opt);
                goto ClnRet;

            ClnRet:
                free(longOpt);
                return res;


        } else {
            addItem(res.args, charpToSeq(arg));
        }
    }


    return res;

}


#define tab "\t"
#define tab2 tab tab
void printAllArgsHelp(ArgParser parser){
    printCharSeq(parser->helps);

    const char* Pre="options:";

    // to ensure Pre is `puts` at most once
    bool hasPriPre=false;
    #define IndPrintf(FMT,...) do{\
        if(!hasPriPre){\
            hasPriPre=true;\
            puts(Pre);\
        }\
        printf(IND FMT, ## __VA_ARGS__);\
    }while(0)

    #define SHT "- " // SHT[1] is to be fill latter, after copyed to `res`
    #define newPre(res, i, ifLong_in) do{\
        const char* sn = parser->map[i];\
        ifLong_in = sn!=NULL;\
        if(ifLong_in){\
            res=catCStr(SHT", --", sn);\
        }else res=newCStr(SHT);\
        res[1]=_MapUnord(i);\
    }while(0)
    char** nhelp;
    nhelp=(char**)parser->boolOptAdded; //discard const, I know it's fine here
    loopMapLen(i){
        if(nhelp[i]==NULL)continue;
        char*pre;
        bool ifLong=false;
        newPre(pre, i, ifLong);
        
        const char* sep=tab3;
        if(ifLong)sep=tab2;
        
        IndPrintf("%s%s%s\n", pre, sep, nhelp[i]);
        char c=_MapUnord(i) - 'a' + 'A';
        char*ophelp=catCStr("opposite of ", pre);
        IndPrintf("-%c%s%s\n", c, sep, ophelp);

        free(ophelp);
        free(pre);
    }
    
    NamedHelp* ndhelp=parser->keyAdded;
    loopMapLen(i){
        if(ndhelp[i].name==NULL)continue;
        char*pre;
        bool ifLong=false;
        newPre(pre, i, ifLong);

        char* sep = tab2;
        if(ifLong) sep=tab;

        IndPrintf("%s %s%s%s\n", pre, ndhelp[i].name, sep, ndhelp[i].help);

        free(pre);
    }
    ndhelp=parser->listKeyAdded;
    loopMapLen(i){
        if(ndhelp[i].name==NULL)continue;
        char*pre;
        bool ifLong=false;
        newPre(pre, i, ifLong);

        char* sep = tab2;
        if(ifLong) sep=tab;

        IndPrintf("[%s %s]...%s%s\n",pre, ndhelp[i].name, sep, ndhelp[i].help);

        free(pre);
    }
    
}

static const char* helpWanted[] = {
     "-h"
    ,"-help"
    ,"--help"
    ,"-?"
    ,"/?"
};
static const char* versionWanted[] = {
     "-v"
    ,"-version"
    ,"--version"
};

#define literalArrLen(arr) (sizeof(arr)/sizeof(arr[0]))
static const size_t lenHelpWanted = literalArrLen(helpWanted);
static const size_t lenVersionWanted = literalArrLen(versionWanted);



void printProjVerionInfo(ProjInfo info){
    printf("Proj: %s\nVersion: %s\n", info.proj, info.version);
}

void printProjCopyrightInfo(ProjInfo info){
    const char* year = strrchr(__DATE__, ' ') + 1;

    const char*autherInfo, *sep;
    if(info.auther==NULL) sep=autherInfo="";
    else{
        autherInfo=info.auther;
        sep="by ";
    }
    printf("Copyright (c) %s %s%s. %s License\n", year, sep, autherInfo, info.license);

}

void printProjUsageInfo(ProjInfo info){
    if(info.usages.isNotEmpty){
        puts("usage:");
        UsageSeq seq = info.usages.seq;
        forIndex(i, seq){
            char*s=cstr(getItem(seq, i));
            printf(IND);printf(s, info.proj);
            putchar('\n');
            free(s);
        }
        putchar('\n');
    }
}

static ProjInfo GprojInfo;
static ArgParser Gparser;

bool parseInfo(const char* arg){
    for(size_t i=0; i<lenHelpWanted; i++){
        if(strcmp(helpWanted[i], arg)==0){
            printProjVerionInfo(GprojInfo);
            if(GprojInfo.desc!=NULL) printf("desc:\n  %s\n", GprojInfo.desc);
            printProjUsageInfo(GprojInfo);
            putchar('\n');
            printAllArgsHelp(Gparser);
            return false;
        }
    }
    for(size_t i=0; i<lenVersionWanted; i++){
        if(strcmp(versionWanted[i], arg)==0){
            printProjVerionInfo(GprojInfo);
            printProjCopyrightInfo(GprojInfo);
            return false;
        }
    }

    return true;

}

// designed: after called, for GprojInfo, auther and desc is still NULL-able (serverd as a flag)
void enInfo(ArgParser parser, ProjInfo info){
    memcpy(&GprojInfo, &info, sizeof(ProjInfo));
    if(info.proj==NULL)GprojInfo.proj=basenameRef(parser->argv[0]);
    if(info.version==NULL)GprojInfo.version=DefVer;
    if(info.license==NULL)GprojInfo.license=DefLicense;
    parser->preArgHook = parseInfo;
    Gparser=parser;
}

void addUsage(Usages* p, const char* usage){
    assert(p->isNotEmpty);
    const char* ProjFmt = "%s ";
    char* newStr;
    if(strstr(usage, ProjFmt)!=usage) {
        newStr = catCStr(ProjFmt, usage);
    }else{
        newStr=newCStr(usage);
    }
    CharSeq nows;
    nows=charpToSeq(newStr);
    free(newStr);
    addItem(p->seq, nows);

}
Usages newUsages(const char* nullEnded[]){
    Usages res={.isNotEmpty=true};
    initSeq(CharSeq, res.seq);
    char*now;
    CharSeq nows;
    for(int i=0; (now=(char*)nullEnded[i])!=NULL; i++){
        addUsage(&res, now);
    }
    return res;
}
void freeUsage(Usages u){
    forIndex(i, u.seq){
        deinitSeq(getItem(u.seq, i));
    }
    deinitSeq(u.seq);
}

void deInfo(ProjInfo info){
    if(!info.usages.isNotEmpty)freeUsage(info.usages);
}
