
#ifndef _MAIN_H
#define _MAIN_H

#define ProjName "WordInter"
#define HelpMsg  "\
desc: a file-based search engine\n\
usage: \n\
  THIS [FLAG] DIR [-c, --code CODE]\n\
    FLAG: \n\
      -l\tsingle line mode, treat each line as a paragragh\n\
      -L\tmultiline mode, only if there are more than 1 newline,\n\
        \tthen they are treated as two paragraphs  [default mode]\
"
#define License "Copyright (c) 2023 litlighilit(lcm). GPLv3 License"
#define Version "1.0.0"

#define PriInfo(item) msgl( #item ": %s", item)
#endif