

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

/// @returns 0 on success
static int _cmsg_enableVT();
#ifdef _WIN32
#include <windows.h>
static bool vted = false;
static DWORD cmsg_pre_dwMode = 0; // used to recover
void _cmsg_recover_console_modeOfHandle(DWORD handle){
    HANDLE hOut = GetStdHandle(handle);
    if (hOut == INVALID_HANDLE_VALUE) return;
    SetConsoleMode(hOut, cmsg_pre_dwMode);
}

void _cmsg_recover_console_mode(void){
    _cmsg_recover_console_modeOfHandle(STD_OUTPUT_HANDLE);
    _cmsg_recover_console_modeOfHandle(STD_ERROR_HANDLE);
}

/// @returns err code
static int _cmsg_enableVTOfHandle(DWORD handle){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(handle);
    DWORD dwMode = 0;
    if (hOut == INVALID_HANDLE_VALUE) goto RetErr;

    // if a pipe, no need (and can-not) to enable Virual for color
    if (!GetConsoleMode(hOut, &dwMode)){
        if(FILE_TYPE_PIPE==GetFileType(hOut)) return 0;
        else goto RetErr;
    }
    cmsg_pre_dwMode = dwMode;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) goto RetErr;
    
    return 0;
    RetErr:
    return GetLastError();
}
static int _cmsg_enableVT(){
    // Though initially,
    //  OUTPUT and ERROR is the active console screen buffer `CONOUT$`
    //  (i.e. they two refer to the same one console buffer)
    // in case that it's adjusted, we `enable` both of them:
    int ret;
    ret = _cmsg_enableVTOfHandle(STD_OUTPUT_HANDLE);
    if(ret!=0) return ret;
    ret = _cmsg_enableVTOfHandle(STD_ERROR_HANDLE);
    atexit(_cmsg_recover_console_mode);
    return ret;
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
