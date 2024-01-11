

#include "cmsg.h"

#ifdef _WIN32
#include <windows.h>
bool vted = false;
DWORD cmsg_pre_dwMode = 0; // used to recover
void _cmsg_recover_console_mode(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    SetConsoleMode(hOut, cmsg_pre_dwMode);
}

/// @warn can not reenter
/// @return 
int _cmsg_enableVT(){
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
bool vted = true;
int _cmsg_enableVT(){ return 0; }
#endif

bool colorEnabled = false;


const char* cmsg_colors[3]={"","",""};


static int CmsgColorOn(){
    if(colorEnabled) return 0;
    if(!vted){
        int ret = _cmsg_enableVT();
        if(ret!=0) return ret;
        vted = true;
    }
    cmsg_colors[ciGreen] = EC(34);
    cmsg_colors[ciYellow] = EC(33);
    cmsg_colors[ciRed] = EC(31);

    colorEnabled = true;

    return 0;
}
const char* _EmptyS="";
static void CmsgColorOff(){
    if(!vted) return;
    if(!colorEnabled) return;

    cmsg_colors[ciGreen] = _EmptyS;
    cmsg_colors[ciYellow] = _EmptyS;
    cmsg_colors[ciRed] = _EmptyS;

    colorEnabled = false;
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

