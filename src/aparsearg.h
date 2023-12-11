
#ifndef _APARSEARG_H
#define _APARSEARG_H

/* ascii ahpha parse cmdline argument
  for opt arg, only one char is stored. e.g. -A is a oppsite flag of -a

  latter arg value will overwrite the former value
*/

/*doctest:
  #define argc_ 3
  char* argv_[argc_+1] = {"./main", "-l", "-c",  NULL};

    // XXX: not use argv_
  ArgParser pa = newArgParser(argc, argv, NULL, NULL);


  char k='c';
  char f='l';
  addArgMap(pa, k, "--code");
  addListKey(pa, k, "CODE", "help for c");
  addBoolOpt(pa, f, "lll");

  ParseArgRes res = parseArgs(pa);

  if(res.err!=NULL)printf("err: %s\n", res.err);
  else puts("no parse error...");


  printf("if want opt %c: %d\n", f, wantOpt(res, f));

  //const char* val = getVal(res, k);
  //printf("-%c %s\n", k, val);

  ListArg codes = getListVal(res, k);

  puts("args of c:")
  //forEach(codes, printCharSeq);
  for(int i=0; i<codes.len; i++) printCharSeq(getItem(codes, i));

  puts("the is args:");
  for(int i=0; i<res.args.len; i++) printCharSeq(getItem(res.args, i));
  
  printAllHelp(pa);

  freeParseArgRes(res);
  freeArgParser(pa);

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
    const char* proj; // not null
    CharSeq helps;
    const char* map[MapLen]; // not begins with "--"
    // XXX: the following use the same map.
    const char* optAdded[MapLen];
    NamedHelp keyAdded[MapLen]; // also store name and help string
    NamedHelp listKeyAdded[MapLen]; // also store name and help string

} ArgParserObj;

typedef ArgParserObj* ArgParser;


ArgParser newArgParser(int argc, char* argv[],
     const char* helpOrNul,
     const char* projNameOrNul);

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

void printAllHelp(ArgParser parser);

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

#endif //#ifndef _ALPARSEARG_H
