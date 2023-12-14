
#ifndef _APARSEARG_H
#define _APARSEARG_H

/* ascii ahpha parse cmdline argument
  for opt arg, only one char is stored. e.g. -A is a oppsite flag of -a

  latter arg value will overwrite the former value
*/

/*doctest:
  #define argc_ 3
  char* argv_[argc_+1] = {"./main", "-l", "-c",  NULL};


  const char*usage = "";
    // XXX: not use argv_
  ArgParser parser = newArgParser(argc, argv, "base help...");
  
  enInfo(parser, (ProjInfo){.auther="lit",.usages=USAGES(
     "[-l|-L] [-c,--code CODE] dir|[-f,--file FILE]...\t run code and exit"
    ,"[-l|-L] dir|[-f,--file FILE]...\t\t\t enter REPL"
    )
   }
  );

  //char k='m';
  char listK='c';
  char flag='l';
  char filesK = 'f';
 
  addArgHelp(parser, "dir", "dir to process");
  
  addListKey(parser, listK, "CODE", "help for c");
  addArgMap(parser, listK, "--code");

  addBoolOpt(parser, flag, "help for flag");

  //addListKey(parser, filesK, "FILE", "file help");
  //addArgMap(parser, filesK, "--file");

  ParseArgRes res = parseArgs(parser);

// check err first
  if(res.err!=NULL){
    printf("err: %s\n", res.err);
    exit(-1);
  }
  else puts("no parse error...");


  printf("if want opt %c: %d\n", flag, wantOpt(res, flag));

  //const char* val = getVal(res, k);
  //printf("-%c %s\n", k, val);

  ListArg codes = getListVal(res, listK);

  puts("args of c:");
  //forEach(codes, printlnCharSeq);
  for(int i=0; i<codes.len; i++) printlnCharSeq(getItem(codes, i));

  puts("the is args:");
  for(int i=0; i<res.args.len; i++) printlnCharSeq(getItem(res.args, i));
  
  //printAllArgsHelp(parser);

  freeParseArgRes(res);
  freeArgParser(parser);

*/

#include "strimpl.h"
#include "cstrutils.h"


#define _MapOrd(c) (c-'a')
#define _MapUnord(c) (c+'a')

#define _MapSearchKey(res, map, s) do{\
    for(int _i=0; _i<MapLen; _i++){\
        const char* _s=map[_i];\
        if(_s==NULL)continue;\
        if(strcmp(_s,s)==0){\
            res=_MapUnord(_i);\
            break;\
        }\
    }\
}while(0)


typedef Seq(CharSeq) ListArg;
typedef Seq(CharSeq) ArgsSeq;


#define MAX_MAP 26 // mapping 'a'-'z' to 0-25

#define MapLen MAX_MAP



#define IS_LOWER(c) ('a'<=c && c<='z')
#define IS_UPPER(c) ('A'<=c && c<='Z')

#define checkOption IS_LOWER 
#define checkOpOption IS_UPPER

typedef struct{
    const char* name; //not NULL
    const char* help; //not NULL
} NamedHelp;

typedef struct {
    char** argv;
    int argc;
    bool (*preArgHook)(const char* arg); // not NULL. returns if go on. i.e. if false then exit
    CharSeq helps;
    const char* map[MapLen]; // not begins with "--"
    // XXX: the following use the same map.
    const char* boolOptAdded[MapLen];
    NamedHelp keyAdded[MapLen]; // also store name and help string
    NamedHelp listKeyAdded[MapLen]; // also store name and help string

} ArgParserObj;

typedef ArgParserObj* ArgParser;


ArgParser newArgParser(int argc, char* argv[],
     const char* helpOrNul);

void freeArgParser(ArgParser parser);

/* panic: if `shortName` has alreadly mapped before
*/
void addArgMap(ArgParser parser, char shortName, const char* longName);

// add bool option, when prasing, upper char means false
void addBoolOpt(ArgParser parser, char shortOpt, 
    const char* helpOrNull);

// XXX: name and help can't be NULL, used to print help
void addKey(ArgParser parser, char shortKey, const char* name,
    const char* help);

void addListKey(ArgParser parser, char shortKey, const char* name,
    const char* help);

void addArgHelp(ArgParser parser, const char* itemName, const char* help);

void printAllArgsHelp(ArgParser parser);

typedef struct {
    char* err;
    ArgsSeq args;
    bool options[MapLen];
    const char* keyvals[MapLen];
    ListArg keyListvals[MapLen];
} ParseArgRes;

ParseArgRes parseArgs(ArgParser);
void freeParseArgRes(ParseArgRes res);

#define wantOpt(parseArgRes, opt) (parseArgRes.options[_MapOrd(opt)])
#define getVal(parseArgRes, key) (parseArgRes.keyvals[_MapOrd(key)])
#define getListVal(parseArgRes, key) (parseArgRes.keyListvals[_MapOrd(key)])


#define DefVer "0.0.1"
#define DefLicense "MIT"

typedef Seq(CharSeq) UsageSeq;

typedef struct {
    UsageSeq seq;
    bool isNotEmpty;// just servered as a auto flag
} Usages;


/* 
XXX: not thread-safe, use static data
*/
Usages newUsages(const char* nullEnded[]);
void addUsage(Usages*, const char*);
// all argument shall be `char*`
#define USAGES(...) newUsages((const char*[]){__VA_ARGS__,NULL})

typedef struct{
    const char* proj;
	const char* version;
	const char* license;
	const char* auther;
	const char* desc;
    Usages usages;
} ProjInfo;

void printProjInfo(ProjInfo);

/* 
XXX: not thread-safe, use global-scope obj to communicate between functions
*/
void enInfo(ArgParser parser, ProjInfo info);

void deInfo(ProjInfo info);
#endif //#ifndef _ALPARSEARG_H