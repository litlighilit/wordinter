
#include <string.h>
#include "dbop.h"

#ifndef IDX // start with 0 or 1
#define IDX 1
#endif


#define toWordStream(seq) toStrStream(seq)
#define toParaStream(seq) toStrStream(seq)


#define next_word(stream) tokenToCharSeq(nextWord(&stream))
#define next_para(stream) tokenToCharSeq(nextPara(&stream))

#define doInPara(para, doWith_Word) do{\
            StrStream wstream = toWordStream(para);\
            \
            int __n=skipPunc(para,0);\
            wstream.idx = __n;\
            \
            for(int iWord=IDX; ; iWord++){\
                CharSeq Word = next_word(wstream);\
                if(Word.len==0)break;\
                doWith_Word;\
                deinitSeq(Word);\
            }\
}while(0)

// filterDo(const RecSeq rs, const char* word, doWith_pos) where callback can access `pos`
#define filterDo(rs, word, doWith_pos) do{\
    forIndex(__i, rs){\
        Rec rec = getItem(rs, __i);\
        Pos pos; pos.fname=rec.fname;\
        CharSeq s = getData(rec);\
        \
        StrStream pstream = toParaStream(s);\
        \
        for(int iPara=IDX; ; iPara++){\
            CharSeq para = next_para(pstream);\
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

void queryAll(const RecSeq rs, const char* word){
    filterDo(rs, word, printPos(pos));
}


#define checkFname(rec, fname) ( strcmp((rec).fname, fname) == 0 )

int countWordOf(const RecSeq rs, const char* fname, int para){
    Rec rec;
    searchBy(rec, rs, fname, checkFname);
    
    if(strcmp(fname, rec.fname)!=0) return FileNotFoundErr;
    CharSeq fileCont = getData(rec);

    StrStream pstream = toParaStream(fileCont);

    CharSeq Para;

    int i=IDX;
    
    while(1){
        CharSeq seq = next_para(pstream);
        if(seq.len==0)goto RangeErr;
        if(i==para){
            Para = copyCharSeq(seq);
            goto CountWord;
        }
        i++;
        deinitSeq(seq);
    }

CountWord:
    int cnt = 0;
    doInPara(Para, cnt++);
    deinitSeq(Para);
    return cnt;

RangeErr:
    return OverRangeErr;
}

int countFrequency(const RecSeq rs, const char* word){
    int cnt=0;
    filterDo(rs, word, cnt++);
    return cnt;
}


int listFile(const RecSeq rs, const char* fnameOrNULL){
    int res=0;
    if(fnameOrNULL==NULL){
        forIndex(idx, rs){
            Rec rec = getItem(rs, idx);
            puts(rec.fname);
            res++;
        }
    }else{
        const char*fname = fnameOrNULL;
        Rec rec={};
        #define cmp(rec, fname) (strcmp(rec.fname, fname)==0)
        searchBy(rec, rs, fname, cmp);
        #undef cmp

        if(rec.fname==NULL) return 0;

        printf("%s: \n", rec.fname);
        printCharSeq(rec.data);
        res++;

    }
    return res;
}