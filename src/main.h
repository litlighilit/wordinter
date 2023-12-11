
#ifndef _MAIN_H
#define _MAIN_H

#define ProjName "WordInter"
#define HelpMsg  "\
desc: a file-based search engine\n\
usage: \n\
  THIS [FLAG] DIR         \tenter REPL\n\
  THIS [FLAG] DIR -c CODE \trun CODE and exit\n\
    FLAG: \n\
      -l\tsingle line mode, treat each line as a paragragh\n\
      -L\tmultiline mode, only if there are more than 1 newline,\n\
        \tthen they are treated as two paragraphs  [default mode]\
"

#define Author "litlighilit(lcm)"
#define License "GPLv3"
#define Version "1.0.0"

#define Proj "WordInter"
#define Desc "a file-based search engine"

#define K(arr) arr[1]

#define singleLineParaFlag "-l"
const char singLineP = K(singleLineParaFlag);
#define lHelp "single line mode, treat each line as a paragragh."
#define detailsHelp "if '-l' is not set, it'll be multiline mode[default mode], only if there are more than 1 newline,\n\
        \tthen they are treated as two paragraphs"

#define filesKey "-f"
const char filesK = K(filesKey);
#define fHelp "files to read from"

#define codesKey "-c"
const char codesK = K(codesKey);
#define cHelp "code to run, try -c 'help CODE' for specific CODE's help"

ProjInfo INFO = (ProjInfo){
        .auther=Author, 
        .license=License,

        .proj=Proj,
        .version=Version,
        .desc=Desc,
        //.usages=... // will (and have to be) set within `main` function
    };
Usages genUsage(){ return USAGES(
             "[-l|-L] [-c,--code CODE] dir|[-f,--file FILE]...\t run code and exit"
            ,"[-l|-L] dir|[-f,--file FILE]...\t\t\t enter REPL"
        );
}
#endif