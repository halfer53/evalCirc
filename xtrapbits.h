/*
 * xtrapbits.h
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 *
 *      Extension of "classic" 32-bit xtrapbits.h (v1.6) for the amd64 model of gcc
 *
 */

#ifndef __XTRAPBITS__
#define __XTRAPBITS__

#define False 0 // for compatibility with stdbool.h
#define True 1

/* $XFree86$ */
/*
 * This include file is designed to be a portable way for systems to define
 * bit field manipulation of arrays of bits.
 */

/*****************************************************************************
Copyright 1987, 1988, 1989, 1990, 1994 by Digital Equipment Corporation,
Maynard, MA

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*****************************************************************************/
/*
 *
 *  CONTRIBUTORS:
 *
 *      Dick Annicchiarico
 *      Robert Chesler
 *      Dan Coutu
 *      Gene Durso
 *      Marc Evans
 *      Alan Jamison
 *      Mark Henry
 *      Ken Miller
 *
 */
typedef unsigned char *UByteP;  /* Pointer to an unsigned byte array */
#define BitsInByte    8L        /* The number of bits in a byte */

#define BitInByte(bit)	        /* Returns the bit mask of a byte */ \
    (1UL << (((bit) % BitsInByte)))

#define BitInInt16(bit)	        /* Returns the bit mask of a 16-bit short int */ \
    (1UL << (((bit) % (BitsInByte * 2L))))

#define BitInInt32(bit)	        /* Returns the bit mask of a 32-bit int */ \
    (1UL << (((bit) % (BitsInByte * 4L))))

#define BitInWord(bit)	        /* Returns the bit mask of a 64-bit word (aka long long int) */ \
    (1ULL << (((bit) % (BitsInByte * 8L))))

#define ByteInArray(array,bit)	/* Returns the byte offset to get to a bit */ \
    (((UByteP)(array))[(bit) / BitsInByte])

#define BitIsTrue(array,bit)    /* Test to see if a specific bit is True */ \
    (ByteInArray(array,bit) & BitInByte(bit))

#define BitIsFalse(array,bit)   /* Test to see if a specific bit is False */ \
    (!(BitIsTrue(array,bit)))

#define BitTrue(array,bit)      /* Set a specific bit to be True */ \
    (ByteInArray(array,bit) |= BitInByte(bit))

#define BitFalse(array,bit)     /* Set a specific bit to be False */ \
    (ByteInArray(array,bit) &= ~BitInByte(bit))

#define BitToggle(array,bit)    /* Toggle a specific bit */ \
    (ByteInArray(array,bit) ^= BitInByte(bit))

#define BitCopy(dest,src,bit)   /* Copy a specific bit */ \
    BitIsTrue((src),(bit)) ? BitTrue((dest),(bit)) : BitFalse((dest),(bit))

#define BitValue(array,bit)     /* Return True or False depending on bit */ \
    (BitIsTrue((array),(bit)) ? True : False)

#define BitSet(array,bit,value) /* Set bit to given value in array */ \
    (value) ? BitTrue((array),(bit)) : BitFalse((array),(bit))

#endif /* __XTRAPBITS__ */
