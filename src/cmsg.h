
#ifndef _CMSG_H
#define _CMSG_H
/** @file cmsg.h
 
  @brief colored message

  use ESC to en-color

  under Windows, call win-api to enable Virtual Terminal
  @see https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// suppress warning of "format string is not a string literal ..."
//  as we just use ANSI control sequence as printf's format string, which is fine
#pragma clang diagnostic ignored "-Wformat-security"

/// environment variable to disable color
#define NO_COLOR_ENV "NO_COLOR"

enum ColorIndex {
    ciGreen,
    ciYellow,
    ciRed,
};

enum cmsg_EnableColorLevel {
    clOff,
    clOnIfEnvAllow,  ///< only on if no `NO_CLOLR` env-var found
    clOnAnyWay,
};

/**
  @param enableColor if enable color for following info/warn/err
*/
int cmsgCfg(enum cmsg_EnableColorLevel cl);


/// like cmsgCfg() but will give a warning if cfg fails
int cmsgWarnOnFailCfg(enum cmsg_EnableColorLevel enableColorLevel);


#define ESC "\x1b" // ESC
#define EC(ns) (ESC "[" #ns "m")

#define DEF EC(0)

extern const char* const cmsg_colors[3];

#ifndef ERR_RET
#define ERR_RET -1
#endif

#ifndef cmsg_out
#define cmsg_out stdout
#endif

#ifndef cmsg_err
#define cmsg_err stderr
#endif


int fnocolorPri(FILE* f, int _, const char* s);

/// if @p f is a TTY, then print with color; else with no coor
int fcolorPriIfNoTty(FILE* f, int ci, const char* s);

// no color by default
extern int (*fPri)(FILE* f, int ci, const char* s);

// indexed by ColorIndex 
static const char* const PRE_MSG[] = { "[INFO] ", "[WARN] ", "[ERR] " };

#define msg(m,...) fmsg(cmsg_out , m,## __VA_ARGS__)

#define msgl(m,...) fmsgl(cmsg_out, m,##__VA_ARGS__)

int fmsg(FILE* f, const char* format, ...);

int fmsgl(FILE*f, const char* format, ...);

#define _priWithCI(f,ci) fPri(f, ci, PRE_MSG[ci])

#define info(m,...) (_priWithCI(cmsg_out, ciGreen), msgl(m, ## __VA_ARGS__))
#define warn(m,...) (_priWithCI(cmsg_out, ciYellow), msgl(m, ## __VA_ARGS__))
#define err(m,...) (_priWithCI(cmsg_err, ciRed), fmsgl(cmsg_err, m, ## __VA_ARGS__), exit(ERR_RET))


// used by both interpreter and main
#define sayBye() msgl("bye.");

#endif // #ifndef _CMSG_H
