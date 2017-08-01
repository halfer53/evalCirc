/* EvalCircuit.h
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */

#ifndef EVALCIRCUIT_H_
#define EVALCIRCUIT_H_

#include <stdbool.h>
#include <stdint.h>
#include "xtrapbits.h" // for UByteP

struct progArgs {
	uint64_t lgw; //TODO: float lgw, so that w need only be a multiple of 8 and not a power of 2
	uint64_t lgn; //TODO: float lgn, so that n need only be a multiple of 8 and not a power of 2
	uint64_t a; // number of inputs
	uint64_t b; // number of outputs
	uint64_t lgbs; // lg(blockSize): bpw files are blocked and 0-padded, gates don't span blocks
	uint32_t gbasis; // bitmap specifying basis of circuit, 0x08 is 2-nand
	uint64_t gformat; // gate-format e.g. 8211 for 8-gate blocks, 2 inputs/gate, 1 B/input, 1 b/gtype
	int gBlockSize;
	char *input; // input to circuit (in hex)
	char *circuitFileName; // name of BPW file describing a circuit
	bool createFile; // if True, then create a BPW file and don't evaluate its circuit
	int zippy; // if negative, add spurious CPU load; if positive, shed CPU load (compute garbage)
	bool quiet; // default is verbose (echo args, list circuit input and output)
	UByteP vm; // virtual memory of circuit evaluator
	bool ospace; // allocate filebuffers in win32 address space
	int ginSize; // bit-width of a gate input-specifier
	int headerSize; // number of bytes in the header, and in the first block of the file
};

#define BPW_VERSION 0 // the highest BPW file version supported by this program
#define HEADER_LENGTH 64 // BPW files have 64B of header, if they are version BPW_VERSION

typedef struct _fileHeader { //TODO: update header definition in http://arxiv.org/abs/1501.02885
	uint8_t magic[4]; // "BPW" (with trailing 0-byte, little-endian)
	uint32_t version; // == BPW_VERSION, if a BPW file is written by this program
	uint32_t offsetToData; // == HEADER_LENGTH, if written by this program
	uint32_t lgbs; // gates are written to file in blocks of (1<<lgbs) bytes; header in its own block
	uint64_t w; // width of circuit w, w==(1<<lgw) in this incomplete implementation
	uint64_t n; // number of gates n, with n==(1<<lgn) in this incomplete implementation
	uint64_t a; // number of inputs
	uint64_t b; // number of outputs
	uint64_t gformat; // interpreted as 5-digit decimal integer, e.g. 08221 for g8221 format
	uint32_t gbasis; // bitmask restricting the types of gates representable in this file
	//TODO: handle gbasis != 0x8.   Currently only 2-nands are implemented.
	uint32_t headerCRC; //TODO: compute crc32().  Currently writing 0 in headerCRC
	// http://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/zlib-crc32-1.html
} fileHeader_t, *pFileHeader_t;

extern struct progArgs pArgs;
//TODO: multithread, with a copy of this struct provided to each worker-thread

extern fileHeader_t fHead;

#endif /* EVALCIRCUIT_H_ */
