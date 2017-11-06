/* getGate.h
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */

#ifndef GETGATE_H_
#define GETGATE_H_

#include <stdint.h>
#include "bpw_io.h"

typedef struct _gate { // general case -- inefficient space, but rapid access
	uint64_t in1; // allow huge circuit-widths
	uint64_t in2;
	uint8_t gtype; // huge basis
} gate_t, *pgate_t;

typedef struct _gate8211 { // eight 2-input gates: two 1-byte input descriptors and 1-bit type
	uint8_t gid[16]; // input descriptors
	uint8_t gtype; // interpretation depends on pArgs->gbasis
} gate8211_t, *pgate8211_t;
// length, when packed unaligned; sizeof() will probably be 20
#define LGATE8211_T 17

typedef struct _gate8214 { // eight 2-input gates: two 1-byte input descriptors and 4-bit type
	uint8_t gid[16];
	uint8_t gtype[4];  // two type-nibbles in each cell
} gate8214_t, *pgate8214_t;
// length -- note that this struct is 5 four-byte words, so it packs nicely in a file
#define LGATE8214_T 20

typedef struct _gate8221 { // eight 2-input gates: two 2-byte input descriptors and 1-bit type
	uint16_t gid[16];
	uint8_t gtype;
} gate8221_t, *pgate8221_t;
#define LGATE8221_T 33

typedef struct _gate8224 { // eight 2-input gates: two 2-byte input descriptors and 4-bit type
	uint16_t gid[16];
	uint8_t gtype[4];  // two type-nibbles in each cell
} gate8224_t, *pgate8224_t;
// note that this struct is 9 four-byte words
#define LGATE8224_T 36

typedef struct _gate8231 { // eight 2-input gates: two 3-byte input descriptors and 1-bit type
	uint8_t gid[48]; // input descriptors (clumsy because no 24-bit datatype), little-endian
	uint8_t gtype;
} gate8231_t, *pgate8231_t;
#define LGATE8231_T 49

typedef struct _gate8234 { // eight 2-input gates: two 3-byte input descriptors and 4-bit type
	uint8_t gid[48]; // input descriptors (clumsy because no 24-bit datatype), little-endian
	uint8_t gtype[4];
} gate8234_t, *pgate8234_t;
#define LGATE8234_T 52

typedef struct _gate8241 { // eight 2-input gates: two 4-byte input descriptors and 1-bit type
	uint32_t gid[16];
	uint8_t gtype;
} gate8241_t, *pgate8241_t;
#define LGATE8241_T 65

typedef struct _gate8244 { // eight 2-input gates: two 4-byte input descriptors and 4-bit type
	uint32_t gid[16];
	uint8_t gtype[4];  // two type-nibbles in each cell
} gate8244_t, *pgate8244_t;
#define LGATE8244_T 68 // seventeen four-byte words

pgate8211_t reserveGate8211( );
pgate8214_t reserveGate8214( );
pgate8221_t reserveGate8221( );
pgate8224_t reserveGate8224( );
pgate8231_t reserveGate8231( );
pgate8234_t reserveGate8234( );
pgate8241_t reserveGate8241( );
pgate8244_t reserveGate8244( );
void releaseGate8xxx( );

void randGate( pgate_t, long lev, long ibase);

#endif /* GETGATE_H_ */
