
#ifndef _INTERPRETER_H
#define _INTERPRETER_H
/**
 @file interpreter.h
*/
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

/// flag used to show @ref evalCmd status
enum Flag{
    FQuit=-1,
    FSucc,
    FMissArg,
    FTypeErr,
    FUnknowCmd,
    FEmptyCmd,
};

/** feed @p line command to @p interp to evaluate
  @param[in,out] interp
  @param line command line to evaluate
  @returns evaluation flag
*/
enum Flag
evalCmd(Interpreter* interp, const CharSeq line);

/// enter REPL (Repeat Evaluate and Print Loop)
void enterRepl(Interpreter* interp);

/// @brief print help of @p n cmd
/// @param n order of cmd to print
/// @returns if success (whether @p n is in valid range)
bool priHelp(int n);


/// @brief print alias of @p n cmd
/// @param n order of cmd to print
/// @returns if success (whether @p n is in valid range)
bool priAlias(int n);

/// get order of @p cmd
int cmdOrd(const CharSeq cmd);
#endif //#ifndef _INTERPRETER_H
