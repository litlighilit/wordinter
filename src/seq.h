
#ifndef _SEQ_H
#define _SEQ_H

// macros for a generics sequence


#include <stdlib.h> // malloc, free

#define DEF_SEQ_CAP 10

#define Seq(T) struct{\
    T*data;\
    int len;\
    int cap;\
}

#define getData(seq) (seq.data)

#define initSeq(T,seq) do{\
    (seq).data=(T*)malloc(DEF_SEQ_CAP*sizeof(T)); \
    (seq).cap = DEF_SEQ_CAP; \
    (seq).len=0;\
}while(0);

#define initSeqOfLen(T, seq, len) do{\
    (seq).data=(T*)malloc(len*sizeof(T));\
    (seq).cap=len+1;\
    (seq).len=len;\
}while(0)

#define deinitSeq(seq) do{ free((seq).data); (seq).cap=0; }while(0)

#define clearSeq(seq) do{\
    seq.len=0;\
}while(0)

#define seqEqSeq(seq1, seq2) (memcmp(getData(seq1), getData(seq2), (seq1).len)==0)

#define addItem(seq, item) do{\
    setItem((seq), (seq).len, (item)); \
    (seq).len++; \
    if((seq).len==(seq).cap){  \
        (seq).cap*=2; \
        (seq).data = realloc((seq).data, (seq).cap);\
    }\
}while(0)

#ifndef SEQ_CHECK_RANGE
#define getItem(seq, idx) ((seq).data[idx])
#define setItem(seq, idx, val) do{ (seq).data[idx]=val;}while(0)

#else 
#error "can't be used yet"
#include <stdio.h>
int RangeError(){ fputs("RangeError", stderr); exit(-1); return NULL; }
#define getItem(seq, idx) (idx<seq.len?seq.data[idx]:RangeError())
#define setItem(seq, idx, val) do{ if(idx>=seq.len)RangeError(); seq.data[idx]=val;}while(0)
#endif // #ifndef SEQ_CHECK_RANGE



// like nim's `for i,_ in seq.items`
#define forIndex(i, seq) for(int i=0; i<(seq).len; i++)

// item forEach iter, `callback` will be passed a argument of the `T` type
#define forEach(seq, callback) do{\
    forIndex(idx, (seq)){ \
        callback(getItem((seq),idx));\
    }\
}while(0)


// `cmp`'s second arg is `key`
#define searchBy(res, seq, destKey, cmp) do{\
    forIndex(i, (seq)){\
        if(cmp(getItem(seq, i), destKey)){\
            res=getItem(seq, i);\
            break;\
        }\
    }\
}while(0)

#endif //#ifndef _SEQ_H
