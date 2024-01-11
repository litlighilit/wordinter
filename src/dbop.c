
#include <string.h>
#include "dbop.h"

#ifndef PARAM_START // start with 0 or 1
#define PARAM_START 1
#endif


#define toWordStream(seq) toStrStream(seq)
#define toParaStream(seq) toStrStream(seq)


#define next_word(stream) tokenToCharSeq(nextWord(&stream))
#define next_para(stream, multiLinePara) tokenToCharSeq(nextPara(&stream, multiLinePara))

#define doInPara(para, doWith_Word) do{\
            StrStream wstream = toWordStream(para);\
            \
            slen_t __n=skipW(para,0);\
            wstream.idx = __n;\
            \
            for(int iWord=PARAM_START; ; iWord++){\
                CharSeq Word = next_word(wstream);\
                if(Word.len==0)break;\
                doWith_Word;\
                deinitSeq(Word);\
            }\
}while(0)

// filterDo(const RecSeq rs, const char* word, doWith_pos) where callback can access `pos`
#define filterDo(interp, filter, word, doWith_pos) do{\
    RecSeq rs=interp.db;\
    forIndex(__i, rs){\
        Rec rec = getItem(rs, __i);\
        Pos pos; pos.fname=rec.fname;\
        CharSeq s = getData(rec);\
        \
        StrStream pstream = toParaStream(s);\
        \
        for(size_t iPara=PARAM_START; ; iPara++){\
            CharSeq para = next_para(pstream, interp.multiLinePara);\
            if(para.len==0)break;\
            pos.para=iPara;\
            doInPara(para,\
                if(filter(Word, word)){\
                    pos.idx = iWord;\
                    doWith_pos;\
                }\
            );\
            deinitSeq(para);\
        }\
    }\
}while(0)

typedef bool (*Cmper)(const CharSeq seq, const char* s);

bool queryAll(const Interpreter interp, const char* word, bool ignoreCase){
    Cmper cmp;
    if(ignoreCase) cmp = seqIEqStr;
    else cmp = seqEqStr;
    
    bool hasPos = false;
    #define doWith do{ hasPos=true;printPos(pos); } while(0)
    filterDo(interp, cmp, word, doWith );

    return hasPos;
}


#define checkFname(rec, fname) ( strcmp((rec).fname, fname) == 0 )

size_t countWordOf(const Interpreter interp, slen_t fileOrd, size_t para){
    
    RecSeq rs = interp.db;
    if(fileOrd<1 || fileOrd>rs.len) return FileNotFoundErr;
    slen_t fileIdx = fileOrd-1;
    Rec rec = uncheckedGetItem(rs, fileIdx);
    CharSeq fileCont = getData(rec);

    StrStream pstream = toParaStream(fileCont);

    CharSeq Para;

    size_t i=PARAM_START;
    
    while(1){
        CharSeq seq = next_para(pstream, interp.multiLinePara);
        if(seq.len==0)goto RangeErr;
        if(i==para){
            Para = seq;
            break;
        }
        i++;
        deinitSeq(seq);
    }

    size_t cnt = 0;
    doInPara(Para, cnt++);
    deinitSeq(Para);
    return cnt;

RangeErr:
    return IndexErr;
}

size_t countFrequency(const Interpreter interp, const char* word, bool ignoreCase){
    Cmper cmp;
    if(ignoreCase) cmp = seqIEqStr;
    else cmp = seqEqStr;

    size_t cnt=0;
    filterDo(interp, cmp, word, cnt++);
    return cnt;
}


size_t listFile(const Interpreter interp, slen_t fileOrd){
    size_t res=0;
    RecSeq rs = interp.db;
    if(fileOrd==0){
        forIndex(idx, rs){
            Rec rec = getItem(rs, idx);
            size_t ord = idx+1;
            printf("%zu. ", ord);
            puts(rec.fname);
            res++;
        }
    }else{
        if(fileOrd<1||fileOrd>rs.len) return 0;
        size_t fileIdx = fileOrd-1;
        Rec rec=uncheckedGetItem(rs, fileIdx);


        printf("%s\n", rec.fname);
        for(size_t _=0; _<strlen(rec.fname); _++) putchar('=');
        putchar('\n');
        printlnCharSeq(rec.data);
        res++;

    }
    return res;
}