/** @mainpage
 *  @brief @c Word Interface
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
#define Version "1.1.4"

#define Proj "WordInter"
#define Desc "a file-based search engine"

#define singleLineParaFlag "-l"

#define lHelp "single line mode, treat each line as a paragragh."
const char* Notes[] = {
"if '-l' is not set, it'll be multiline mode[default mode]: \n\
        \tonly if there are more than 1 newline, then they are treated as two paragraphs", 
"if neither DIR nor -f FILE argument is given,\
   or given data can not be read, \n\
    \tthen read one directory from stdin until got one valid",
};

#define filesKey "-f"
#define fHelp "files to read from"

#define codesKey "-c"
#define cHelp "code to run, try -c 'help CODE' for specific CODE's help"

#include "aparsearg.h"
#include "fsinter.h"
#include "interpreter.h"
/// add usage for @ref main CLI
Usages genUsage(){ return USAGES(
             "[-l|-L] [-c,--code CODE] [dir|[-f,--file FILE] ...]\t run code and exit"
            ,"[-l|-L] [dir|[-f,--file FILE] ...]\t\t\t enter REPL"
        );
}
#endif
