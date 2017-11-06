/*
 * randomRD.c
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */


#include <stdio.h>
#include <stdint.h>
#include "randomRd.h"
// #include <immintrin.h>


#ifndef __RDRND__
#pragma GCC push_options
#pragma GCC target("rdrnd")
#endif /* __RDRND__ */
extern __inline int
__attribute__((__gnu_inline__, __always_inline__, __artificial__))
_rdrand64_step (unsigned long long *__P)
{
  return __builtin_ia32_rdrand64_step (__P);
}
#ifdef __DISABLE_RDRND__
#undef __DISABLE_RDRND__
#pragma GCC pop_options
#endif /* __DISABLE_RDRND__ */

// access the 64-bit hardware RNG in the amd64 architecture
int randomRD64( unsigned long long *rand64 )
{
    int retries = 10; // as recommended by Intel

    while( retries-- ) {
        if ( _rdrand64_step(rand64) ) {
            return 1;
        }
    }
    return 0; // extremely unlikely, unless there's a hardware fault
}

static unsigned long long rstore = 0; // retain "unused" bits from the previous randomRD call
static int ravail = 0; // number of unused bits in rstore

// return k bits from the randomRD stream, 0 <= k <= 64
// TODO: implement a fast low-quality RNG, validate against a high-quality one
// TODO: return an arithmetic sequence (i.e. a "strided workload" for the cache),
// to confirm that some high-width circuits can be efficiently evaluated
uint64_t randomRDk( int k ) {
	uint64_t retval = 0;

	if( k > ravail ) { // store isn't sufficient to meet demand
		retval = rstore & ((1LL << ravail) - 1LL); // use all available bits
		k -= ravail;
		randomRD64( &rstore ); // replenish store
		ravail = 64;
		retval = (retval << k) | (rstore & ((1LL << k) - 1LL));
	} else { // usual case
		retval = rstore & ((1LL << k) - 1LL);
	}

	rstore >>= k;
	ravail -= k;

	return retval;
}
