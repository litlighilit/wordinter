

#include "cmsg.h"

#include <stdarg.h>

#if _WIN32
# include <io.h>
# define ISATTY(fd) _isatty(fd)
# define FILENO(f) _fileno(f)
#else
# include <unistd.h> // isatty
# if _POSIX_VERSION > 200800L
#  include <stdio.h> // fileno
# else
int fileno(FILE*);  // before POSIX.2008.1 
# endif
# define ISATTY(fd) isatty(fd)
# define FILENO(f) fileno(f)
#endif
#define fisatty(f) ISATTY(FILENO(f))

#ifdef _WIN32
#include <windows.h>
static bool vted = false;
static DWORD cmsg_pre_dwMode = 0; // used to recover
void _cmsg_recover_console_mode(void){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    SetConsoleMode(hOut, cmsg_pre_dwMode);
}

/// @warn can not reenter
/// @return 
static int _cmsg_enableVT(){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut == INVALID_HANDLE_VALUE) goto RetErr;
    if (!GetConsoleMode(hOut, &dwMode)) goto RetErr;
    cmsg_pre_dwMode = dwMode;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) goto RetErr;
    
    atexit(_cmsg_recover_console_mode);
    return 0;
    RetErr:
    return GetLastError();
}
#else
static bool vted = true;
static int _cmsg_enableVT(){ return 0; }
#endif

const char* const cmsg_colors[3]={EC(34), EC(33), EC(31)};


#if __STDC_VERSION__ > 202300L
#define VA_START(vl, startv) va_start(vl)
#else
#define VA_START(vl, startv) va_start(vl, startv)
#endif

#define VA_INIT(vl, startv) va_list vl; VA_START(vl, startv)

#define VFPRINT  \
    VA_INIT(vl, format);\
    int res = vfprintf(f, format, vl);\
    va_end(vl);

int fmsg(FILE* f, const char* format, ...){
    VFPRINT
    return res;
}

int fmsgl(FILE*f, const char* format, ...){
    VFPRINT
    fputc('\n', f);
    return res;
}

static int fcolorPri(FILE* f, int ci, const char* s){
    fmsg(f,cmsg_colors[ci]);
    int res = fmsg(f,s);
    fmsg(f,DEF);
    return res;
}

int fnocolorPri(FILE* f, int _, const char* s){
    fmsg(f, s);
}

int fcolorPriIfNoTty(FILE* f, int ci, const char* s){
    if(fisatty(f)) return fcolorPri(f, ci, s);
    
    return fnocolorPri(f, ci, s);
}

int (*fPri)(FILE* f, int ci, const char* s) = fcolorPriIfNoTty;

static int CmsgColorOn(){
    if(!vted){
        int ret = _cmsg_enableVT();
        if(ret!=0) return ret;
        vted = true;
    }
    fPri = fcolorPriIfNoTty;


    return 0;
}

static void CmsgColorOff(){
    if(!vted) return;

    fPri = fnocolorPri;

}

static inline bool EnvNoColor(){
    char* s = getenv(NO_COLOR_ENV);
    return s!=NULL;
}

int cmsgCfg(enum cmsg_EnableColorLevel cl){
    if(!cl){
        CmsgColorOff();
        return 0;
    }
    if(cl==clOnIfEnvAllow){
        if(EnvNoColor()){
            CmsgColorOff();
            return 0;
        }
        return CmsgColorOn();
    }
    return CmsgColorOn();
}

int cmsgWarnOnFailCfg(enum cmsg_EnableColorLevel enableColorLevel){
    (cmsgCfg(enableColorLevel)==0||warn("can't init color output"));
}
