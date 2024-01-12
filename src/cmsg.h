
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

/// environment variable to disable color
#define NO_COLOR_ENV "NO_COLOR"

enum ColorIndex {
    ciGreen,
    ciYellow,
    ciRed,
};

enum cmsg_EnableColorLevel {
    clOff,
    clOnIfEnvAllow,
    clOnAnyWay,
};

/**
  @param enableColor if enable color for following info/warn/err
*/
int cmsgCfg(enum cmsg_EnableColorLevel cl);

#define ESC "\x1b" // ESC
#define EC(ns) (ESC "[" #ns "m")

#define DEF EC(0)

extern const char* cmsg_colors[3];

#ifndef ERR_RET
#define ERR_RET -1
#endif

#ifndef cmsg_out
#define cmsg_out stdout
#endif

#ifndef cmsg_err
#define cmsg_err stderr
#endif

#define fmsg(f, m,...) fprintf(f, m,## __VA_ARGS__)
#define fmsgl(f, m,...) (fmsg(f, m,##__VA_ARGS__), fputc('\n', f))

#define msg(m,...) fmsg(cmsg_out , m,## __VA_ARGS__)

#define msgl(m,...) fmsgl(cmsg_out, m,##__VA_ARGS__)

#define fcolorPri(f, ci, s) (fmsg(f,cmsg_colors[ci]), fmsg(f,s), fmsg(f,DEF))

#define info(m,...) (fcolorPri(cmsg_out, ciGreen,  "[INFO] "), msgl(m, ## __VA_ARGS__))
#define warn(m,...) (fcolorPri(cmsg_out, ciYellow, "[WARN] "), msgl(m, ## __VA_ARGS__))
#define err(m,...)  (fcolorPri(cmsg_err, ciRed, "[ERR] "), fmsgl(cmsg_err, m, ## __VA_ARGS__), exit(ERR_RET))

/// like cmsgCfg() but will give a warning if cfg fails
#define cmsgWarnOnFailCfg(enableColorLevel) (cmsgCfg(enableColorLevel)==0||warn("can't init color output"))

// used by both interpreter and main
#define sayBye() msgl("bye.");

#endif // #ifndef _CMSG_H
