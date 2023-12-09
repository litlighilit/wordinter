
#ifndef _INTERPRETER_H
#define _INTERPRETER_H

/*doctest:
  RecSeq seq;
  initSeq(Rec, seq);

  CharSeq str1 = charpToSeq("here is str1\nnewline\n\nA new para is here");
  Rec r1 = {"f1", str1};

  CharSeq str2 = charpToSeq("Oh, here is str2\n\n2");
  Rec r2 = {"f2", str2};

  addItem(seq, r1);
  addItem(seq, r2);

  puts("entering repl...");

  Interpreter interp = initInterpreter(seq, true);
  enterRepl(interp);


*/

#include <stdbool.h>
#include <stdio.h>

#include "types.h"
#include "strutils.h"
#include "interptype.h"
#include "dbop.h"
#include "msg.h"

enum Flag{
    FQuit=-1,
    FSucc,
    FMissArg,
    FTypeErr,
    FUnknowCmd,
    FEmptyCmd,
};

enum Flag
evalCmd(const Interpreter interp, const CharSeq line);

void enterRepl(const Interpreter interp);

bool priHelp(int n);
bool priAlias(int n);

int cmdOrd(const CharSeq cmd);
#endif //#ifndef _INTERPRETER_H
