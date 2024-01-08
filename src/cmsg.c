

#include "cmsg.h"

#ifdef _WIN32
#include <windows.h>
bool vted = false;
int enableVT(){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) goto RetErr;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) goto RetErr;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) goto RetErr;
    
    return 0;
    RetErr:
    return GetLastError();
}
#else
bool vted = true;
int enableVT(){ return 0; }
#endif

bool colorEnabled = false;


const char* cmsg_colors[3]={"","",""};


static int CmsgColorOn(){
    if(colorEnabled) return 0;
    if(!vted){
        int ret = enableVT();
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

int cmsgCfg(bool enableColor){
    if(enableColor) return CmsgColorOn();
    else{
        CmsgColorOff();
        return 0;
    }
}

