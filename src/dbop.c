
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
            int __n=skipW(para,0);\
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
#define filterDo(interp, word, doWith_pos) do{\
    RecSeq rs=interp.db;\
    forIndex(__i, rs){\
        Rec rec = getItem(rs, __i);\
        Pos pos; pos.fname=rec.fname;\
        CharSeq s = getData(rec);\
        \
        StrStream pstream = toParaStream(s);\
        \
        for(int iPara=PARAM_START; ; iPara++){\
            CharSeq para = next_para(pstream, interp.multiLinePara);\
            if(para.len==0)break;\
            pos.para=iPara;\
            doInPara(para,\
                if(seqEqStr(Word, word)){\
                    pos.idx = iWord;\
                    doWith_pos;\
                }\
            );\
            deinitSeq(para);\
        }\
    }\
}while(0)

void queryAll(const Interpreter interp, const char* word){
    filterDo(interp, word, printPos(pos));
}


#define checkFname(rec, fname) ( strcmp((rec).fname, fname) == 0 )

int countWordOf(const Interpreter interp, int fileOrd, int para){
    
    RecSeq rs = interp.db;
    if(fileOrd<1 || fileOrd>rs.len) return FileNotFoundErr;
    int fileIdx = fileOrd-1;
    Rec rec = uncheckedGetItem(rs, fileIdx);
    CharSeq fileCont = getData(rec);

    StrStream pstream = toParaStream(fileCont);

    CharSeq Para;

    int i=PARAM_START;
    
    while(1){
        CharSeq seq = next_para(pstream, interp.multiLinePara);
        if(seq.len==0)goto RangeErr;
        if(i==para){
            Para = copyCharSeq(seq);
            goto CountWord;
        }
        i++;
        deinitSeq(seq);
    }

CountWord:
    ;int cnt = 0;
    doInPara(Para, cnt++);
    deinitSeq(Para);
    return cnt;

RangeErr:
    return IndexErr;
}

int countFrequency(const Interpreter interp, const char* word){
    int cnt=0;
    filterDo(interp, word, cnt++);
    return cnt;
}


int listFile(const Interpreter interp, int fileOrd){
    int res=0;
    RecSeq rs = interp.db;
    if(fileOrd==0){
        forIndex(idx, rs){
            Rec rec = getItem(rs, idx);
            int ord = idx+1;
            printf("%d. ", ord);
            puts(rec.fname);
            res++;
        }
    }else{
        if(fileOrd<1||fileOrd>rs.len) return 0;
        int fileIdx = fileOrd-1;
        Rec rec=uncheckedGetItem(rs, fileIdx);


        printf("%s\n", rec.fname);
        for(int _=0; _<strlen(rec.fname); _++) putchar('=');
        putchar('\n');
        printlnCharSeq(rec.data);
        res++;

    }
    return res;
}