//----------------------------------------------------------------
//
//  4190.307 Operating Systems (Fall 2024)
//
//  Project #4: xSwap: Compressed Swap for xv6
//
//  November 7, 2024
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//----------------------------------------------------------------

// Memory zones
#define ZONE_FIXED  0
#define ZONE_NORMAL 1
#define ZONE_ZMEM   2

// Types for zalloc()/zfree()
#define ZHALF       2
#define ZFULL       4


// LZO compression library
#define LZO1X_1_MEM_COMPRESS      (16*1024)



extern int nalloc4k, zalloc4k, zalloc2k;
extern int nswapin, nswapout;

extern int lzo1x_compress(const unsigned char *src, uint32 src_len, unsigned char *dst, uint32 *dst_len, void *wrkmem);
extern int lzo1x_decompress(const unsigned char *src, uint32 src_len, unsigned char *dst, uint32 *dst_len);



