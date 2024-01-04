
#ifndef _SEQ_H
#define _SEQ_H

/**
 @file seq.h
 @brief macros for a generics sequence
*/

#include <stdio.h> // fprintf, stderr
#include <stdlib.h> // malloc, free, exit

#define DEF_SEQ_CAP 10

#define Seq(T) struct{\
    T*data;\
    int len;\
    int cap;\
    int itemSize;\
}

#define getData(seq) (seq.data)

/** initalize a empty sequence, which shall be freed via @ref deinitSeq
 @post free data via @ref deinitSeq
*/
#define initSeq(T,seq) do{\
    (seq).data=(T*)malloc(DEF_SEQ_CAP*sizeof(T)); \
    (seq).cap = DEF_SEQ_CAP; \
    (seq).len=0;\
    (seq).itemSize=sizeof(T);\
}while(0);

/// @post free data via @ref deinitSeq
#define initSeqOfLen(T, seq, len) do{\
    (seq).data=malloc((len)*sizeof(T));\
    (seq).cap=(len)+1;\
    (seq).len=(len);\
    (seq).itemSize=sizeof(T);\
}while(0)


/// @pre free data allocated via @ref initSeq or @ref initSeqOfLen
#define deinitSeq(seq) do{ free((seq).data);  }while(0)

#define clearSeq(seq) do{\
    seq.len=0;\
}while(0)

#define seqEqSeq(seq1, seq2) (memcmp(getData(seq1), getData(seq2), (seq1).len*seq1.itemSize)==0)

#define uncheckedGetItem(seq, idx) ((seq).data[idx])
#define uncheckedSetItem(seq, idx, val) do{ (seq).data[idx]=val;}while(0)

#define IndexError(seq, idx) (fprintf(stderr, "IndexError: index of `%d` out of range of seq `%s`(len=%d) ", idx, #seq, seq.len),exit(-1) )
#define checkedGetItem(seq, idx) ((idx)<(seq).len?(seq).data[idx]:IndexError(seq, idx),seq.data[idx])
#define checkedSetItem(seq, idx, val) do{ if((idx)>=(seq).len)IndexError(seq, idx); seq.data[idx]=(val);}while(0)

#ifndef SEQ_CHECK_RANGE
#define getItem checkedGetItem
#define setItem checkedSetItem
#else
#define getItem uncheckedGetItem
#define setItem uncheckedSetItem
#endif // #ifndef SEQ_CHECK_RANGE

#define addItem(seq, item) do{\
    uncheckedSetItem((seq), (seq).len, (item)); \
    (seq).len++; \
    if((seq).len==(seq).cap){  \
        (seq).cap*=2; \
        (seq).data = realloc((seq).data, (seq).cap*(seq).itemSize);\
    }\
}while(0)

#define pAddItem(seqp, item) do{\
    (seqp)->data[(seqp)->len]=item;\
    (seqp)->len++; \
    if((seqp)->len==(seqp)->cap){  \
        (seqp)->cap*=2; \
        (seqp)->data = realloc((seqp)->data, (seqp)->cap*(seqp)->itemSize);\
    }\
}while(0)

/// like [nim](https://nim-lang.org/docs/iterators.html#pairs.i%2Cseq%5BT%5D)'s `for i,_ in seq.pairs`
#define forIndex(i, seq) for(int i=0; i<(seq).len; i++)

/// item forEach iter, @p callback will be passed a argument of the `T` type
#define forEach(seq, callback) do{\
    forIndex(idx, (seq)){ \
        callback(uncheckedGetItem((seq),idx));\
    }\
}while(0)


/// @note @p cmp second arg is `key`
#define searchBy(res, seq, destKey, cmp) do{\
    forIndex(i, (seq)){\
        if(cmp(uncheckedGetItem(seq, i), destKey)){\
            res=uncheckedGetItem(seq, i);\
            break;\
        }\
    }\
}while(0)

#endif //#ifndef _SEQ_H
