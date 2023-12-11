
#include <stdlib.h>
#include <assert.h>
#include "aparsearg.h"

#define loopMapLen(var) for(int var=0; var<MapLen; var++)
inline ArgParser newArgParser(int argc, char* argv[],
     const char* helpOrNul,
     const char* projNameOrNul
){
    ArgParser res = malloc(sizeof(ArgParserObj));
    if(projNameOrNul==NULL) res->proj=basenameRef(argv[0]);

    CharSeq helps;
    if(helpOrNul!=NULL) helps=charpToSeq(helpOrNul);
    else initSeq(char, helps);
    res->helps = helps;

    res->argc = argc;
    res->argv = argv;

    loopMapLen(i){
        res->map[i]=
        res->optAdded[i]=NULL;

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
    ParseArgRes res = {};
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

    const char**const map = parser->map;
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
    parser->optAdded[_MapOrd(shortOpt)]=help;
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

void addArgHelp(ArgParser parser, const char* itemName, const char* help){
    assert(help!=NULL);
    
    addItem(parser->helps, '\n');

    char* prehelp = catCStr(itemName, ": \t");
    char* thehelp = catCStr(prehelp, help);

    for(int i=0; i<strlen(help); i++){
        addItem(parser->helps, thehelp[i]);
    }
    addItem(parser->helps, '\n');
    free(prehelp);
    free(thehelp);

}

void printAllHelp(ArgParser parser){
    puts(parser->proj);
    printCharSeq(parser->helps);

    #define SHT "- " // SHT[1] is to be fill latter
    #define newPre(res, i) do{\
        const char* sn = parser->map[i];\
        if(sn!=NULL){\
            res=catCStr(SHT", --", sn);\
        }else res=newCStr(SHT);\
        res[1]=_MapUnord(i);\
    }while(0)
    char** nhelp;
    nhelp=(char**)parser->optAdded;
    loopMapLen(i){
        if(nhelp[i]==NULL)continue;
        char*pre;
        newPre(pre, i);

        const char* sep = "\t\t";
        printf("%s%s%s\n",pre, sep, nhelp[i]);

        free(pre);
    }
    
    NamedHelp* ndhelp=parser->keyAdded;
    loopMapLen(i){
        if(ndhelp[i].name==NULL)continue;
        char*pre;
        newPre(pre, i);

        const char* sep = "\t";
        printf("%s %s%s%s\n", pre, ndhelp[i].name, sep, ndhelp[i].help);

        free(pre);
    }
    ndhelp=parser->listKeyAdded;
    loopMapLen(i){
        if(ndhelp[i].name==NULL)continue;
        char*pre;
        newPre(pre, i);

        const char* sep = "\t";
        printf("[%s %s]...%s%s\n",pre, ndhelp[i].name, sep, ndhelp[i].help);

        free(pre);
    }
    /*
    for(int n=0; n<_HN; n++){
        for(int i=0; i<MapLen; i++){
            if(helps[n][i]==DefV || helps[n][i]==NULL) continue;

            char*sepLong=newCStr("");
            char*sep="\t\t";
            const char* sn=parser->map[i];
            if(sn!=NULL){
                free(sepLong);
                sepLong=catCStr(", --",sn);
                sep="\t";
            }
            printf("-%c%s %s %s\n",
                _MapUnord(i), sepLong, sep, helps[n][i]
            );

            free(sepLong);
        }
    }*/
    
}


ParseArgRes parseArgs(ArgParser parser){
    ParseArgRes res=newParseArgRes();

    char** argv = parser->argv;
    for(int i=1; i < parser->argc; i++){
        char* longOpt = NULL; // for message feedback for error
        const char* arg = argv[i];
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
            
            if(parser->optAdded[_MapOrd(sht)]!=NULL){
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
                char*pre = "unknow option: ";
                size_t eLen = strlen(pre)+1;
                char*err=malloc(eLen+1);
                if(!positive) sht = sht - 'a' + 'A';
                sprintf(err, "%s%c", pre, sht);
                res.err=err;
                goto ClnRet;

            NoValErr:
                char*opt; 
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
