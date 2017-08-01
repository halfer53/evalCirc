/*
 * EvalCirc.c
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "EvalCircuit.h"
#include "evalCirc.h"
#include "getGate.h"

long evalOrCreateCirc8211() { // tuned for g8211 evals; handles creates and random-evals

	long level = 0;
	long ngate = 0;
	long ibase;
	pgate8211_t pg8211;
	gate8211_t randGate8211; // dummy filebuffer, used when evaluating at random
	uint8_t g8211Type;

	assert( fHead.w <= 64 );  // vm fits in four 64-bit words
	assert( fHead.w == (1LL << pArgs.lgw) ); // w is a power of 2 in this implementation

	if( pArgs.lgw > 6 ) {
		printf( "Excessive circuit width %lu for a file in g8211 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	uint64_t *p = (uint64_t *) &(pArgs.vm[0]);
	uint64_t vmMask = (((1LL << (fHead.w - 1)) - 1) << 1) | 0x1; // fHead.w LSBs are 1
	uint64_t vm0 = *p & vmMask;  // copy circuit inputs into vm0 (and 0 elsewhere in vm0)
	uint64_t vm1 = 0; // the other three regions of vm are initially 0
	uint64_t vm2 = 0;
	uint64_t vm3 = 0;

	uint64_t vmiMask = fHead.w - 1; // for interpreting g.in1 and g.in2

	while( ngate < fHead.n ) {

		if( level % 2 ) {
			vm3 = 0;  //  clear the current gate-output area, to simplify our byte-writes
		} else {
			vm2 = 0;
		}

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm

			if( pArgs.circuitFileName == NULL ) {
				pg8211 = &randGate8211;  // special case: random evaluation without file I/O
			} else {
				pg8211 = reserveGate8211( ); // prepare to read or write 8 gates
			}

			if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
				g8211Type = 0; // initialise this bitarray, before writing bits into it
			} else {
				g8211Type = pg8211->gtype; // prepare to dissect this bitarray, during a read
			}

			for( int i=0; i<8; i++ ) {

				gate_t g;

				// create or read g
				if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
					ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level
					randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
					(pg8211->gid)[ 2*i ] = g.in1;
					(pg8211->gid)[ 2*i + 1 ] = g.in2;
					g8211Type = (g.gtype << 7) + (g8211Type >> 1);
				} else {
					g.in1 = (pg8211->gid)[ 2*i ];
					g.in2 = (pg8211->gid)[ 2*i + 1 ];
					g.gtype = (g8211Type >> i) & 0x1;
				}

				if( !pArgs.createFile ) { // evaluate g
					uint8_t vin1, vin2, vout;

					if( pArgs.zippy <= 0 ) { // evaluate g with error-checks

						switch( (g.in1 >> (pArgs.lgw)) & 0x03 ) {
						case 0x00: // circuit-input area of vm[]
							if( level != 0 ) {
								printf( "COPY pseudogates are not implemented.\n" );
								exit( EXIT_FAILURE );
							} else {
								vin1 = ( vm0 >> (g.in1 & vmiMask) ) & 1LL;
							}
							break;
						case 0x01:
							printf( "COPY pseudogates are not implemented.\n" );
							exit( EXIT_FAILURE );
							break;
						case 0x02: // gate-inputs on odd-numbered levels
							if( level % 2 ) {
								vin1 = ( vm2 >> (g.in1 & vmiMask) ) & 1LL;
							} else {
								printf( "Illegal input for gate %ld of level %ld.\n",	igate, level );
								exit( EXIT_FAILURE );
							}
							break;
						case 0x03: // gate-inputs on even-numbered levels
							if( !(level % 2) ) {
								vin1 = ( vm3 >> (g.in1 & vmiMask) ) & 1LL;
							} else {
								printf( "Illegal input for gate %ld of level %ld.\n",	igate, level );
								exit( EXIT_FAILURE );
							}
							break;
						}

						switch( (g.in2 >> (pArgs.lgw)) & 0x03 ) {
						case 0x00: // circuit-input range
							if( level != 0 ) {
								printf( "COPY pseudogates are not implemented.\n" );
								exit( EXIT_FAILURE );
							} else {
								vin2 = ( vm0 >> (g.in2 & vmiMask) ) & 1LL;
							}
							break;
						case 0x01:
							printf( "COPY pseudogates are not implemented.\n" );
							exit( EXIT_FAILURE );
							break;
						case 0x02: // gate-inputs on odd-numbered levels
							if( level % 2 ) {
								vin2 = ( vm2 >> (g.in2 & vmiMask) ) & 1LL;
							} else {
								printf( "Illegal input for gate %ld of level %ld.\n",	igate, level );
								exit( EXIT_FAILURE );
							}
							break;
						case 0x03: // gate-inputs on even-numbered levels
							if( !(level % 2) ) {
								vin2 = ( vm3 >> (g.in2 & vmiMask) ) & 1LL;
							} else {
								printf( "Illegal input for gate %ld of level %ld.\n",	igate, level );
								exit( EXIT_FAILURE );
							}
							break;
						}
					} else if ( ! pArgs.createFile ) { // evaluate g rapidly

						if( level == 0 ) {
							vin1 = ( vm0 >> (g.in1 & vmiMask) ) & 1LL;
							vin2 = ( vm0 >> (g.in2 & vmiMask) ) & 1LL;
						} else if( level % 2 ) {
							vin1 = ( vm2 >> (g.in1 & vmiMask) ) & 1LL;
							vin2 = ( vm2 >> (g.in2 & vmiMask) ) & 1LL;
						} else {
							vin1 = ( vm3 >> (g.in1 & vmiMask) ) & 1LL;
							vin2 = ( vm3 >> (g.in2 & vmiMask) ) & 1LL;
						}

					}

					if( g.gtype != 0 ) {
						vout = ( !(vin1 & vin2) ) & 1LL;  // 2-nand
					} else {
						vout = ( !(vin1 | vin2) ) & 1LL;  // 2-nor
					}

					vmchar = (vout << 7) + (vmchar >> 1);
				}  // end of gate-eval


				ngate++;

			} // end of 8-gate loop

			if( ! pArgs.createFile ) {	// write 8 gate-outputs to vmi
				if( !(level % 2) ) {
					vm2 |= ( vmchar << (igate >> 3) );
				} else {
					vm3 |= ( vmchar << (igate >> 3) );
				}
			}

			if( pArgs.createFile ) {
				pg8211->gtype = g8211Type; // write gtype field into vm
			}

			if( pArgs.circuitFileName != NULL ) {
				releaseGate8xxx();  // advances curr_fb->fbi to next 8-gate record
			}

		} // end of level

		level++;

	} // end of circuit

	// copy vmi into vm[]
	p = (uint64_t *) &(pArgs.vm[0]);
	*p = vm0;  // copy 8 bytes from vm0 into circuit-input area
	p = (uint64_t *) &(pArgs.vm[(1 << pArgs.lgw)/8]); // COPY output area
	*p = vm1;  // note: this overwrites some of the previous write, if pArgs.lgw < 6
	p = (uint64_t *) &(pArgs.vm[2*(1 << pArgs.lgw)/8]); // even-level output area
	*p = vm2;
	p = (uint64_t *) &(pArgs.vm[3*(1 << pArgs.lgw)/8]); // odd-level output area
	*p = vm3;  // we allocate at least 32B in vm[] to avoid out-of-bounds error here for lgw<6

	return (2 + (level % 2)) * fHead.w; // index into vm[]: base of circuit-output area

}

long evalCirc8214() { // tuned for g8214 evals

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level

	if( pArgs.lgw > 6 ) {
		printf( "Excessive circuit width %lu for a file in g8214 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm
			pgate8214_t pg8214 = reserveGate8214( ); // prepare to read or write 8 gates
			uint8_t gtypebyte = 0;

			for( int i=0; i<8; i++ ) {

				// unpack a gate
				gate_t g;
				g.in1 = (pg8214->gid)[ 2*i ];
				g.in2 = (pg8214->gid)[ 2*i + 1 ];
				if( i%2 ) {
					gtypebyte = (pg8214->gid)[ i/2 ];
					g.gtype = gtypebyte & 0xf;  // first nibble is in the LSBs
				} else {
					g.gtype = (gtypebyte >> 4) & 0xf; // second nibble in the MSBs
				}

				// evaluate g
				uint64_t vin1, vin2, vout;
				vin1 = ((pArgs.vm)[(g.in1) / 8] >> ((g.in1) % 8)) & 1;
				vin2 = ((pArgs.vm)[(g.in2) / 8] >> ((g.in2) % 8)) & 1;
				if( g.gtype & 0x1 ) { // TODO: handle more than two gate-types
					vout = ( !(vin1 & vin2) ) & 1;  // 2-nand
				} else {
					vout = ( !(vin1 | vin2) ) & 1;  // 2-nor
				}
				vmchar = (vout << 7) + (vmchar >> 1);

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

			pArgs.vm[ (obase+igate)>>3 ] = vmchar;

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

void createCirc8214() { // creates a bpw file in g8214 format

	long level = 0;
	long ngate = 0;
	long ibase;
	pgate8214_t pg8214;
	uint8_t gtypebyte = 0;

	if( pArgs.lgw > 6 ) {
		printf( "Excessive circuit width %lu for a file in g8214 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			pg8214 = reserveGate8214( ); // prepare to write 8 gates

			for( int i=0; i<8; i++ ) {

				gate_t g;
				randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
				(pg8214->gid)[ 2*i ] = g.in1;
				(pg8214->gid)[ 2*i + 1 ] = g.in2;

				if( i%2 ) {
					gtypebyte = (g.gtype & 0xf); // first nibble in LSBs
				} else {
					gtypebyte |= ((g.gtype & 0xf) << 4); // second nibble in MSBs
					(pg8214->gtype)[ i/2 ] = gtypebyte;
				}

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

		} // end of level

		level++;

	} // end of circuit

}

long evalOrCreateCirc8221() { // tuned for gate8221_t

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level
	long ibase;
	pgate8221_t pg8221;
	gate8221_t randGate8221; // dummy filebuffer, used when evaluating at random
	uint8_t g8221Type;

	if( pArgs.lgw > 14 ) {
		printf( "Excessive circuit width %lu for a file in g8221 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level
		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm

			if( pArgs.circuitFileName == NULL ) {
				pg8221 = &randGate8221;  // special case: random evaluation without file I/O
			} else {
				pg8221 = reserveGate8221( ); // prepare to read or write 8 gates
			}

			if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
				g8221Type = 0; // initialise this bitarray, before writing bits into it
			} else {
				g8221Type = pg8221->gtype; // prepare to dissect this bitarray, during a read
			}

			for( int i=0; i<8; i++ ) {

				gate_t g;

				if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
					randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
					(pg8221->gid)[ 2*i ] = g.in1;
					(pg8221->gid)[ 2*i + 1 ] = g.in2;
					g8221Type = (g.gtype << 7) + (g8221Type >> 1);
				} else {
					g.in1 = (pg8221->gid)[ 2*i ];
					g.in2 = (pg8221->gid)[ 2*i + 1 ];
					g.gtype = (g8221Type >> i) & 0x1;
				}

				if( ! pArgs.createFile ) { // evaluate g
					uint64_t vin1, vin2, vout;

					vin1 = ((pArgs.vm)[(g.in1) / 8L] >> ((g.in1) % 8L)) & 1LL;
					vin2 = ((pArgs.vm)[(g.in2) / 8L] >> ((g.in2) % 8L)) & 1LL;

					if( g.gtype != 0 ) {
						vout = ( !(vin1 & vin2) ) & 1LL;  // 2-nand
					} else {
						vout = ( !(vin1 | vin2) ) & 1LL;  // 2-nor
					}

					vmchar = (vout << 7) + (vmchar >> 1);
				}

				ngate++;

			} // end of 8-gate loop

			if( pArgs.createFile ) {
				pg8221->gtype = g8221Type; // write gtype field
			}

			if( pArgs.circuitFileName != NULL ) {
				releaseGate8xxx();  // advances curr_fb->fbi
			}

			if( ! pArgs.createFile ) {	// write 8 gate-outputs to vm
				pArgs.vm[ (obase+igate)>>3 ] = vmchar;
				// equivalent: ByteInArray(pArgs.vm, obase+igate) = vmchar;
			}

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

long evalCirc8224() { // tuned for g8224 evals

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level

	if( pArgs.lgw > 14 ) {
		printf( "Excessive circuit width %lu for a file in g8224 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm
			pgate8224_t pg8224 = reserveGate8224( ); // prepare to read or write 8 gates
			uint8_t gtypebyte = 0;

			for( int i=0; i<8; i++ ) {

				// unpack a gate
				gate_t g;
				g.in1 = (pg8224->gid)[ 2*i ];
				g.in2 = (pg8224->gid)[ 2*i + 1 ];
				if( i%2 ) {
					gtypebyte = (pg8224->gid)[ i/2 ];
					g.gtype = gtypebyte & 0xf;  // first nibble is in the LSBs
				} else {
					g.gtype = (gtypebyte >> 4) & 0xf; // second nibble in the MSBs
				}

				// evaluate g
				uint64_t vin1, vin2, vout;
				vin1 = ((pArgs.vm)[(g.in1) / 8] >> ((g.in1) % 8)) & 1;
				vin2 = ((pArgs.vm)[(g.in2) / 8] >> ((g.in2) % 8)) & 1;
				if( g.gtype & 0x1 ) { // TODO: handle more than two gate-types
					vout = ( !(vin1 & vin2) ) & 1;  // 2-nand
				} else {
					vout = ( !(vin1 | vin2) ) & 1;  // 2-nor
				}
				vmchar = (vout << 7) + (vmchar >> 1);

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

			pArgs.vm[ (obase+igate)>>3 ] = vmchar;

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

void createCirc8224() { // creates a bpw file in g8224 format

	long level = 0;
	long ngate = 0;
	long ibase;
	pgate8224_t pg8224;
	uint8_t gtypebyte = 0;

	if( pArgs.lgw > 14 ) {
		printf( "Excessive circuit width %lu for a file in g8224 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			pg8224 = reserveGate8224( ); // prepare to write 8 gates

			for( int i=0; i<8; i++ ) {

				gate_t g;
				randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
				(pg8224->gid)[ 2*i ] = g.in1;
				(pg8224->gid)[ 2*i + 1 ] = g.in2;

				if( i%2 ) {
					gtypebyte = (g.gtype & 0xf); // first nibble in LSBs
				} else {
					gtypebyte |= ((g.gtype & 0xf) << 4); // second nibble in MSBs
					(pg8224->gtype)[ i/2 ] = gtypebyte;
				}

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

		} // end of level

		level++;

	} // end of circuit

}

long evalOrCreateCirc8231() { // tuned for gate8231_t

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level
	long ibase;
	pgate8231_t pg8231;
	gate8231_t randGate8231; // dummy filebuffer, used when evaluating at random
	uint8_t g8231Type;

	if( pArgs.lgw > 22 ) {
		printf( "Excessive circuit width %lu for a file in g8231 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level
		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm

			if( pArgs.circuitFileName == NULL ) {
				pg8231 = &randGate8231;  // special case: random evaluation without file I/O
			} else {
				pg8231 = reserveGate8231( ); // prepare to read or write 8 gates
			}

			if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
				g8231Type = 0; // initialise this bitarray, before writing bits into it
			} else {
				g8231Type = pg8231->gtype; // prepare to dissect this bitarray, during a read
			}

			for( int i=0; i<8; i++ ) {

				gate_t g;

				if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
					randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
					(pg8231->gid)[ 6*i + 0 ] = g.in1 & 0xff; // 3 bytes per gate-input
					(pg8231->gid)[ 6*i + 1 ] = (g.in1 >> 8) & 0xff;
					(pg8231->gid)[ 6*i + 2 ] = (g.in1 >> 16) & 0xff;
					(pg8231->gid)[ 6*i + 3 ] = g.in2 & 0xff;
					(pg8231->gid)[ 6*i + 4 ] = (g.in2 >> 8) & 0xff;
					(pg8231->gid)[ 6*i + 5 ] = (g.in2 >> 16) & 0xff;
					g8231Type = (g.gtype << 7) + (g8231Type >> 1);
				} else {
					g.in1 = (pg8231->gid)  [ 6*i + 0 ] |
							( (pg8231->gid)[ 6*i + 1 ] << 8 ) |
							( (pg8231->gid)[ 6*i + 2 ] << 16 );
					g.in2 = (pg8231->gid)  [ 6*i + 3 ] |
							( (pg8231->gid)[ 6*i + 4 ] << 8 ) |
							( (pg8231->gid)[ 6*i + 5 ] << 16 );
					g.gtype = (g8231Type >> i) & 0x1;
				}

				if( ! pArgs.createFile ) { // evaluate g
					uint64_t vin1, vin2, vout;

					vin1 = ((pArgs.vm)[(g.in1) / 8L] >> ((g.in1) % 8L)) & 1LL;
					vin2 = ((pArgs.vm)[(g.in2) / 8L] >> ((g.in2) % 8L)) & 1LL;

					if( g.gtype != 0 ) {
						vout = ( !(vin1 & vin2) ) & 1LL;  // 2-nand
					} else {
						vout = ( !(vin1 | vin2) ) & 1LL;  // 2-nor
					}

					vmchar = (vout << 7) + (vmchar >> 1);
				}

				ngate++;

			} // end of 8-gate loop

			if( pArgs.createFile ) {
				pg8231->gtype = g8231Type; // write gtype field
			}

			if( pArgs.circuitFileName != NULL ) {
				releaseGate8xxx();  // advances curr_fb->fbi
			}

			if( ! pArgs.createFile ) {	// write 8 gate-outputs to vm
				pArgs.vm[ (obase+igate)>>3 ] = vmchar;
				// equivalent: ByteInArray(pArgs.vm, obase+igate) = vmchar;
			}

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

long evalOrCreateCirc8241() { // for gate8241_t

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level
	long ibase;
	pgate8241_t pg8241;
	gate8241_t randGate8241; // dummy filebuffer, used when evaluating at random
	uint8_t g8241Type;

	// confirm there's no padding between .gid[] and .gtype
	assert( ((PVOID) &(randGate8241.gtype) - (PVOID) &(randGate8241.gid[0])) == LGATE8241_T - 1 );

	if( pArgs.lgw > 30 ) {
		printf( "Excessive circuit width %lu for a file in g8241 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level
		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm

			if( pArgs.circuitFileName == NULL ) {
				pg8241 = &randGate8241;  // special case: random evaluation without file I/O
			} else {
				pg8241 = reserveGate8241( ); // prepare to read or write 8 gates
			}

			if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
				g8241Type = 0; // initialise this bitarray, before writing bits into it
			} else {
				g8241Type = pg8241->gtype; // prepare to dissect this bitarray, during a read
			}

			for( int i=0; i<8; i++ ) {

				gate_t g;

				if( (pArgs.createFile) || (pArgs.circuitFileName == NULL) ) {
					randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
					(pg8241->gid)[ 2*i ] = g.in1;
					(pg8241->gid)[ 2*i + 1 ] = g.in2;
					g8241Type = (g.gtype << 7) + (g8241Type >> 1);
				} else {
					g.in1 = (pg8241->gid)[ 2*i ];
					g.in2 = (pg8241->gid)[ 2*i + 1 ];
					g.gtype = (g8241Type >> i) & 0x1;
				}

				if( ! pArgs.createFile ) { // evaluate g
					uint64_t vin1, vin2, vout;

					vin1 = ((pArgs.vm)[(g.in1) / 8L] >> ((g.in1) % 8L)) & 1LL;
					vin2 = ((pArgs.vm)[(g.in2) / 8L] >> ((g.in2) % 8L)) & 1LL;

					if( g.gtype == 0 ) {
						vout = ( !(vin1 & vin2) ) & 1LL;  // 2-nand
					} else {
						vout = ( !(vin1 | vin2) ) & 1LL;  // 2-nor
					}

					vmchar = (vout << 7) + (vmchar >> 1);
				}

				ngate++;

			} // end of 8-gate loop

			if( pArgs.createFile ) {
				pg8241->gtype = g8241Type; // write gtype field
			}

			if( pArgs.circuitFileName != NULL ) {
				releaseGate8xxx();  // advances curr_fb->fbi
			}

			if( ! pArgs.createFile ) {	// write 8 gate-outputs to vm
				pArgs.vm[ (obase+igate)>>3 ] = vmchar;
				// equivalent: ByteInArray(pArgs.vm, obase+igate) = vmchar;
			}

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

long evalCirc8234() { // tuned for g8234 evals

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level

	if( pArgs.lgw > 22 ) {
		printf( "Excessive circuit width %lu for a file in g8234 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm
			pgate8234_t pg8234 = reserveGate8234( ); // prepare to read or write 8 gates
			uint8_t gtypebyte = 0;

			for( int i=0; i<8; i++ ) {

				// unpack a gate
				gate_t g;
				g.in1 = (pg8234->gid)  [ 6*i + 0 ] |
						( (pg8234->gid)[ 6*i + 1 ] << 8 ) |
						( (pg8234->gid)[ 6*i + 2 ] << 16 );
				g.in2 = (pg8234->gid)  [ 6*i + 3 ] |
						( (pg8234->gid)[ 6*i + 4 ] << 8 ) |
						( (pg8234->gid)[ 6*i + 5 ] << 16 );
				if( i%2 ) {
					gtypebyte = (pg8234->gid)[ i/2 ];
					g.gtype = gtypebyte & 0xf;  // first nibble is in the LSBs
				} else {
					g.gtype = (gtypebyte >> 4) & 0xf; // second nibble in the MSBs
				}

				// evaluate g
				uint64_t vin1, vin2, vout;
				vin1 = ((pArgs.vm)[(g.in1) / 8] >> ((g.in1) % 8)) & 1;
				vin2 = ((pArgs.vm)[(g.in2) / 8] >> ((g.in2) % 8)) & 1;
				if( g.gtype & 0x1 ) { // TODO: handle more than two gate-types
					vout = ( !(vin1 & vin2) ) & 1;  // 2-nand
				} else {
					vout = ( !(vin1 | vin2) ) & 1;  // 2-nor
				}
				vmchar = (vout << 7) + (vmchar >> 1);

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

			pArgs.vm[ (obase+igate)>>3 ] = vmchar;

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

void createCirc8234() { // creates a bpw file in g8234 format

	long level = 0;
	long ngate = 0;
	long ibase;
	pgate8234_t pg8234;
	uint8_t gtypebyte = 0;

	if( pArgs.lgw > 30 ) {
		printf( "Excessive circuit width %lu for a file in g8244 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			pg8234 = reserveGate8234( ); // prepare to write 8 gates

			for( int i=0; i<8; i++ ) {

				gate_t g;
				randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
				(pg8234->gid)[ 6*i + 0 ] = g.in1 & 0xff; // 3 bytes per gate-input
				(pg8234->gid)[ 6*i + 1 ] = (g.in1 >> 8) & 0xff;
				(pg8234->gid)[ 6*i + 2 ] = (g.in1 >> 16) & 0xff;
				(pg8234->gid)[ 6*i + 3 ] = g.in2 & 0xff;
				(pg8234->gid)[ 6*i + 4 ] = (g.in2 >> 8) & 0xff;
				(pg8234->gid)[ 6*i + 5 ] = (g.in2 >> 16) & 0xff;
				if( i%2 ) {
					gtypebyte = (g.gtype & 0xf); // first nibble in LSBs
				} else {
					gtypebyte |= ((g.gtype & 0xf) << 4); // second nibble in MSBs
					(pg8234->gtype)[ i/2 ] = gtypebyte;
				}

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

		} // end of level

		level++;

	} // end of circuit

}
long evalCirc8244() { // tuned for g8244 evals

	long level = 0;
	long ngate = 0;
	long obase = 0; // index into vm, for first output on current level

	if( pArgs.lgw > 30 ) {
		printf( "Excessive circuit width %lu for a file in g8244 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		obase = (2 + (level % 2)) * fHead.w; // outputs go here

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			unsigned char vmchar = 0; // we collect 8 gate-outputs in vmchar, then write to vm
			pgate8244_t pg8244 = reserveGate8244( ); // prepare to read or write 8 gates
			uint8_t gtypebyte = 0;

			for( int i=0; i<8; i++ ) {

				// unpack a gate
				gate_t g;
				g.in1 = (pg8244->gid)[ 2*i ];
				g.in2 = (pg8244->gid)[ 2*i + 1 ];
				if( i%2 ) {
					gtypebyte = (pg8244->gid)[ i/2 ];
					g.gtype = gtypebyte & 0xf;  // first nibble is in the LSBs
				} else {
					g.gtype = (gtypebyte >> 4) & 0xf; // second nibble in the MSBs
				}

				// evaluate g
				uint64_t vin1, vin2, vout;
				vin1 = ((pArgs.vm)[(g.in1) / 8] >> ((g.in1) % 8)) & 1;
				vin2 = ((pArgs.vm)[(g.in2) / 8] >> ((g.in2) % 8)) & 1;
				if( g.gtype & 0x1 ) { // TODO: handle more than two gate-types
					vout = ( !(vin1 & vin2) ) & 1;  // 2-nand
				} else {
					vout = ( !(vin1 | vin2) ) & 1;  // 2-nor
				}
				vmchar = (vout << 7) + (vmchar >> 1);

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

			pArgs.vm[ (obase+igate)>>3 ] = vmchar;

		} // end of level

		level++;

	} // end of circuit

	return obase;

}

void createCirc8244() { // creates a bpw file in g8244 format

	long level = 0;
	long ngate = 0;
	long ibase;
	pgate8244_t pg8244;
	uint8_t gtypebyte = 0;

	if( pArgs.lgw > 30 ) {
		printf( "Excessive circuit width %lu for a file in g8244 format.\n", fHead.w );
		exit( EXIT_FAILURE );
	}

	while( ngate < fHead.n ) {

		ibase = (2 + ((level + 1) % 2)) * fHead.w; // vm index of first gate-output on prior level

		for( long igate = 0; igate < fHead.w; igate = igate+8 ) { // index gates on this level

			pg8244 = reserveGate8244( ); // prepare to write 8 gates

			for( int i=0; i<8; i++ ) {

				gate_t g;
				randGate( &g, level, ibase ); // generates a gate with randomly-chosen inputs
				(pg8244->gid)[ 2*i ] = g.in1;
				(pg8244->gid)[ 2*i + 1 ] = g.in2;

				if( i%2 ) {
					gtypebyte = (g.gtype & 0xf); // first nibble in LSBs
				} else {
					gtypebyte |= ((g.gtype & 0xf) << 4); // second nibble in MSBs
					(pg8244->gtype)[ i/2 ] = gtypebyte;
				}

				ngate++;

			} // end of 8-gate loop

			releaseGate8xxx();  // advances curr_fb->fbi

		} // end of level

		level++;

	} // end of circuit

}
