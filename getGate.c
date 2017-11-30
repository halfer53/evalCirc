/*
 * GetGate.c
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "EvalCircuit.h"
#include "randomRD.h"
#include "getGate.h"



pgate8211_t reserveGate8211( ) { // unbuffered read or write of 8 gates in 8211 format
	reserve_fbuffer( LGATE8211_T );
	return (pgate8211_t) pCfbI();
}

pgate8214_t reserveGate8214( ) { // unbuffered read or write of 8 gates in 8214 format
	reserve_fbuffer( LGATE8214_T );
	return (pgate8214_t) pCfbI();
}

pgate8221_t reserveGate8221( ) { // unbuffered read or write of 8 gates in 8221 format
	reserve_fbuffer( LGATE8221_T );
	return (pgate8221_t) pCfbI();
}

pgate8224_t reserveGate8224( ) { // unbuffered read or write of 8 gates in 8224 format
	reserve_fbuffer( LGATE8224_T );
	return (pgate8224_t) pCfbI();
}

pgate8231_t reserveGate8231( ) { // unbuffered read or write of 8 gates in 8231 format
	reserve_fbuffer( LGATE8231_T );
	return (pgate8231_t) pCfbI();
}

pgate8234_t reserveGate8234( ) { // unbuffered read or write of 8 gates in 8234 format
	reserve_fbuffer( LGATE8234_T );
	return (pgate8234_t) pCfbI();
}

pgate8241_t reserveGate8241( ) { // unbuffered read or write of 8 gates in 8241 format
	reserve_fbuffer( LGATE8241_T );
	return (pgate8241_t) pCfbI();
}

inline pgate8244_t reserveGate8244( ) { // unbuffered read or write of 8 gates in 8244 format
    reserve_fbuffer( LGATE8244_T );
    return (pgate8244_t) pCfbI();
}

inline pgate8344_t reserveGate8344( ) { // unbuffered read or write of 8 gates in 8244 format
    reserve_fbuffer( LGATE8344_T );
    return (pgate8244_t) pCfbI();
}

void releaseGate8xxx( ) { // releases the currently-reserved portion of fbuffer
	release_reservation( );
}

void randGate( pgate_t g, long lev, long ibase ) { // generate a random gate
	// if lev==0, gate-inputs are connected to random circuit-inputs
	// if lev!=0, gate-inputs are connected to random gate-inputs from vm[ibase..ibase+pArgs.w-1]

	uint32_t r1 = randomRDk( pArgs.lgw );
	uint32_t r2 = randomRDk( pArgs.lgw );
    uint32_t r3 = randomRDk( pArgs.lgw );

	g->gtype = randomRDk( 8 );


	if( lev==0 ) { // level-0 gate inputs are connected to random circuit inputs
		g->in1 = (r1 % pArgs.a);
		g->in2 = (r2 % pArgs.a);
        g->in3 = (r3 % pArgs.a);
	} else { // level>0 gates are connected to random gate-outputs on prior level
		if( fHead.w == (1LL << (pArgs.lgw)) ) { // special case: width is a power of two

			g->in1 = ibase + r1;
			g->in2 = ibase + r2;
            g->in3 = ibase + r3;
		} else {
			g->in1 = ibase + (r1 % fHead.w);
			g->in2 = ibase + (r2 % fHead.w);
            g->in3 = ibase + (r3 % fHead.w);
		}
	}
}

// support for obsolete 12x0 format (1 gate, 2 inputs, arbitrary width, no type-desc)
void get1Gate12x0( pgate_t g ) {
	switch( pArgs.ginSize ) {
	case 8:
		g->in1 = read_8();
		g->in2 = read_8();
		break;
	case 16:
		g->in1 = read_16();
		g->in2 = read_16();
		break;
	case 24:
		g->in1 = read_24();
		g->in2 = read_24();
		break;
	case 32: default:
		g->in1 = read_32();
		g->in2 = read_32();
		break;
	}
}

void getGate12x0( pgate_t g ) { // obsolete gate-read from bpw file in 12x0 format
	reserve_fbuffer( 8 );
	get1Gate12x0( g );
}

void put1Gate12x0( pgate_t g ) { // obsolete gate-write in 12x0 format
	switch( pArgs.ginSize ) {
	case 8:
		write_8( g->in1 );
		write_8( g->in2 );
		break;
	case 16:
		write_16( g->in1 );
		write_16( g->in2 );
		break;
	case 24:
		write_24( g->in1 );
		write_24( g->in2 );
		break;
	case 32: default:
		write_32( g->in1 );
		write_32( g->in2 );
		break;
	}
}

void putGate12x0( pgate_t g ) { // write to bpw file in 12x0 format
	reserve_fbuffer( 8 );
	put1Gate12x0( g );
}



