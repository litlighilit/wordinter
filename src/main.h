/** @mainpage
 *  @brief @c Word Interpreter
 *  @brief a file-based search engine 
 * 
 *  
 *  support both `CLI` and `REPL`
 *  for details, see @ref main.h
 */
#ifndef _MAIN_H
#define _MAIN_H

#define Author "litlighilit(lcm)"
#define License "GPLv3"
#define Version "1.0.0"

#define Proj "WordInter"
#define Desc "a file-based search engine"

#define singleLineParaFlag "-l"

#define lHelp "single line mode, treat each line as a paragragh."
#define detailsHelp "if '-l' is not set, it'll be multiline mode[default mode], only if there are more than 1 newline,\n\
        \tthen they are treated as two paragraphs"

#define filesKey "-f"
#define fHelp "files to read from"

#define codesKey "-c"
#define cHelp "code to run, try -c 'help CODE' for specific CODE's help"

/// add usage for @ref main CLI
Usages genUsage(){ return USAGES(
             "[-l|-L] [-c,--code CODE] dir|[-f,--file FILE]...\t run code and exit"
            ,"[-l|-L] dir|[-f,--file FILE]...\t\t\t enter REPL"
        );
}
#endif