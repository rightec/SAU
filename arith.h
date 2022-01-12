/**
@file		Arith.h
@brief		some common integer arithmetic macros/inline functions.

Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
[e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.

Intel CPU specific support is provided for the Turbo C and GNU C. 
Mawork with other compilers and CPU-s, but is not optimized for them.

@author		
@date		18/01/2011
@version	01.00
*/

/**
ARITH.H ---- 


*/

#ifndef __ARITH_H_INCLUDED__
#define __ARITH_H_INCLUDED__

#ifdef __GNUC__
//#include "gcc/arith.h"
#elif defined(__TURBOC__)
#include "bcc/arith.h"
#endif

#ifdef _MSC_VER
#define __emit__(x) __asm{ __emit (x) }
#endif


#ifndef min
/**
	returns the min between x and y.
*/
#define min(x,y)    (((x) < (y)) ?  (x) : (y))
#endif
#ifndef max
/**
	returns the max between x and y.
*/
#define max(x,y)    (((x) > (y)) ?  (x) : (y))
#endif
#ifndef __ABS
/**
	returns the ABS of x.
*/
#define __ABS(x)    (((x) < (0)) ? -(x) : (x))
#endif
#ifndef abs
/**
	returns the ABS of x.
*/
#define abs(x)      __ABS(x)
#endif
#ifndef scale
/**
	returns (x * n) / d.
*/
#define scale(x,n,d)    (((x) * (n)) / (d))
#endif
#ifndef imin
/**
	returns the min between (int)x and (int)y .
*/
#define imin(x,y)   min((int)(x),(int)(y))
#endif
#ifndef imax
/**
	returns the max between (int)x and (int)y .
*/
#define imax(x,y)   max((int)(x),(int)(y))
#endif
#ifndef iabs
/**
	returns the ABS od (int)x .
*/
#define iabs(x)     abs((int)(x))
#endif
#ifndef umin
/**
	returns the min between (unsigned int)x and (unsigned int)y .
*/
#define umin(x,y)   min((unsigned int)(x),(unsigned int)(y))
#endif
#ifndef umax
/**
	returns the max between (unsigned int)x and (unsigned int)y .
*/
#define umax(x,y)   max((unsigned int)(x),(unsigned int)(y))
#endif
#ifndef lmin
/**
	returns the min between (long)x and (long)y .
*/
#define lmin(x,y)   min((long)(x),(long)(y))
#endif
#ifndef lmax
/**
	return the max between (long)x and (long)y .
*/
#define lmax(x,y)   max((long)(x),(long)(y))
#endif
#ifndef labs
/**
	returns the ABS of (long)x.
*/
#define labs(x)     __ABS((long)(x))
#endif
#ifndef ulmin
/**
	returns the min between (unsigned long)x and (unsigned long)y .
*/
#define ulmin(x,y)  min((unsigned long)(x),(unsigned long)(y))
#endif
#ifndef ulmax
/**
	returns the max between (unsigned long)x and (unsigned long)y .
*/
#define ulmax(x,y)  max((unsigned long)(x),(unsigned long)(y))
#endif

/**
 * int swap
 */
#define iswap(x,y) {             \
int _swap_tmpval_ = (x);       \
(x) = (y);                   \
(y) = _swap_tmpval_;           \
}

/**
 * long swap
 */
#define lswap(x,y) {                   \
long _swap_tmpval_ = (x);         \
(x) = (y);                         \
(y) = _swap_tmpval_;               \
}

/**
 * int sort
 */
#define isort(x,y) {                        \
	if((int)(x) > (int)(y)) iswap(x,y)       \
		}

/**
 * unsigned int sort
 */
#define usort(x,y) {                                        \
if((unsigned int)(x) > (unsigned int)(y)) iswap(x,y)    \
}

/**
 * long sort
 */
#define lsort(x,y) {                                   \
	if((long)(x) > (long)(y)) lswap(x,y)                \
}

/**
 * unsigned long sort
 */
#define ulsort(x,y) {                                       \
	if((unsigned long)(x) > (unsigned long)(y)) lswap(x,y)  \
}


#ifndef bsizeof
/**
sizeof byte macro.
*/
#define bsizeof(s)  (sizeof(s) / sizeof(char))
#endif
#ifndef wsizeof
/**
sizeof word macro.
*/
#define wsizeof(s)  (sizeof(s) / sizeof(short))
#endif
#ifndef lsizeof
/**
sizeof long macro.
*/
#define lsizeof(s)  (sizeof(s) / sizeof(long))
#endif
#ifndef bitsof
/**
sizeof s in bit macro.
*/
#define bitsof(s)   (sizeof(s) * 8)
#endif
#ifndef bytesof
/**
sizeof byte macro.
*/
#define bytesof(s)  ((sizeof(s) + sizeof(char) - 1) / sizeof(char))
#endif
#ifndef wordsof
/**
sizeof word macro.
*/
#define wordsof(s)  ((sizeof(s) + sizeof(short) - 1) / sizeof(short))
#endif
#ifndef longsof
/**
sizeof long macro.
*/
#define longsof(s)  ((sizeof(s) + sizeof(long) - 1) / sizeof(long))
#endif
#ifndef itemsof
/**
array dimension macro.
*/
#define itemsof(arr)    (sizeof(arr) / sizeof((arr)[0]))
#endif
#ifndef offsetof
/**
fieled's struct offset macro.
*/
#define offsetof(structype,field) (int)(          \
	(char *)(&((structype *)(0))->field) -        \
	(char *)(0)                                    \
	)
#endif


#ifndef imul32
/**
 [i|u]mul32(x,y)
 multiply two int-s for a long result
 */
#define imul32(X,Y) ((long)(X) * (long)(Y))
#endif
#ifndef umul32
/**
 [i|u]mul32(x,y)
 multiply two int-s for a long result
 */
#define umul32(X,Y) ((unsigned long)(X) * (unsigned long)(Y))
#endif


#ifndef umuladd32
/**
 umuladd32(x,y,z)
 multiply two unsigned-s for a long result and add an unsigned
 */
#define umuladd32(X,Y,Z) (umul32((X),(Y))+(Z))
#endif



#ifndef iscale
/**
 [i|u]scale(X,N,D)
 scale an integer with long intermediate result but without using long
 arithmetic all the way
*/
#define iscale(X,N,D)   (int)(imul32(X,N) / (long)(D))
#endif
#ifndef uscale
/**
 [i|u]scale(X,N,D)
 scale an integer with long intermediate result but without using long
 arithmetic all the way
*/
#define uscale(X,N,D)   (unsigned int)(umul32(X,N) / (unsigned long)(D))
#endif
#ifndef irscale
/**
 [i|u]scale(X,N,D)
 scale an integer with long intermediate result but without using long
 arithmetic all the way
*/
#define irscale(X,N,D)  ((                                              \
iscale(((int)(X) << 1),N,D) +                                   \
(((int)(X) ^ (int)(N) ^ (int)(D)) >> (bitsof(int) - 1)) +       \
1                                                               \
) >> 1)
#endif
#ifndef urscale
/**
 [i|u]scale(X,N,D)
 scale an integer with long intermediate result but without using long
 arithmetic all the way
*/
#define urscale(X,N,D)  ((uscale(((int)(X) << 1),N,D) + 1) >> 1)
#endif

/**
 replicate_<FROM>2<TO>(byte_or_word_value)
 copy the lower byte(s) of a byte or word into the upper byte(s)
*/
#define __INLINE_REPLICATE__(V,TFROM,TTO) (                           \
((unsigned TTO)(unsigned TFROM)(V)) |                         \
((unsigned TTO)(unsigned TFROM)(V) << (sizeof(TFROM) * 8))     \
)

#ifndef replicate_b2b
#define replicate_b2b(BYTE) (BYTE)
#endif
#ifndef replicate_b2w
#define replicate_b2w(BYTE) __INLINE_REPLICATE__(BYTE,GR_int8,GR_int16)
#endif
#ifndef replicate_b2l
#define replicate_b2l(BYTE) replicate_w2l(replicate_b2w(BYTE))
#endif
#ifndef replicate_b2h
#define replicate_b2h(BYTE) replicate_l2h(replicate_w2l(replicate_b2w(BYTE)))
#endif

#ifndef replicate_w2w
#define replicate_w2w(WORD) (WORD)
#endif
#ifndef replicate_w2l
#define replicate_w2l(WORD) __INLINE_REPLICATE__(WORD,GR_int16,GR_int32)
#endif
#ifndef replicate_w2h
#define replicate_w2h(WORD) replicate_l2h(replicate_w2l(WORD))
#endif

#ifndef replicate_l2l
#define replicate_l2l(LONG) (LONG)
#endif
#ifndef replicate_l2h
#define replicate_l2h(LONG) __INLINE_REPLICATE__(LONG,GR_int32,GR_int64)
#endif

#ifndef replicate_h2h
#define replicate_h2h(LLONG) (LLONG)
#endif

#endif  /* whole file */

