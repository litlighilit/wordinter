
#ifndef _MSG_H
#define _MSG_H
//[header-only]

#define ERR_RET -1

#include <stdio.h>

#define err(msg, ...) do{\
    fprintf(stderr, "[ERR] "msg ,##__VA_ARGS__);\
    fputc('\n', stderr);\
    exit(ERR_RET);\
}while(0)

#define msgNoNL(msg, ...) do{\
    printf(msg ,##__VA_ARGS__);\
}while(0)

/// message with newline
#define msgl(msg,...) do{\
  msgNoNL(msg, ## __VA_ARGS__);\
  _pNL\
}while(0)

#define _pNL putchar('\n');
#define psMsgWith(ps, msg, ...) do{\
    printf(ps);\
    putchar(' ');\
    msgl(msg ,##__VA_ARGS__);\
}while(0)

// message without newline
#define msg msgNoNL

#define warn(msg, ...) psMsgWith("[WARN] ", msg, ## __VA_ARGS__)
#define info(msg, ...) psMsgWith("[INFO] ", msg ,##__VA_ARGS__)

// used by both interpreter and main
#define sayBye() msgl("bye.");
#endif //#ifndef _MSG_H