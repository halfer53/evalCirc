/*
 ============================================================================
 Name        : EvalCircuit.c
 Author      : Clark Thomborson
 Version     :
 Copyright   :
 Description : evaluate a random circuit, or one in a BPW file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xtrapbits.h"
#include "evalCircuit.h"
#include "randomRD.h"
#include "evalCirc.h"
#include "bpw_io.h"
#include "getGate.h"

// program args and file-headers are global
struct progArgs pArgs;
fileHeader_t fHead;

static const char *optString = "n:w:a:b:i:f:c:s:g:z:o:q?";

void display_usage( void )
{
	puts( "EvalCircuit - evaluate a BPW circuit" );
	puts( "  -n x, evaluate a circuit with n == (1<<x) gates" );
	puts( "  -w x, evaluate a width-(1<<x) circuit" );
	puts( "  -a x, evaluate a circuit with x inputs" );
	puts( "  -b x, evaluate a circuit with x outputs" );
	puts( "  -r x, generate a random circuit using rng #x" );
	puts( "  -i x, use hexstring x as input to the circuit" );
	puts( "  -f x, read a BPW circuit description from file x" );
	puts( "  -c x, create file x with a random BPW circuit" );
	puts( "  -g x, specify gate-format x, e.g. 08211.  Ignored if reading a bpw file.");
	puts( "  -h x, gate-basis bitmap of circuit in hex e.g. -h16 for {2nand, 2nor}" );
	puts( "  -s x, BPW file has blocksize (1<<x)" );
	puts( "  -o 0, allocate file buffers on the program heap");
	puts( "  -o 1, allocate file buffers in the OS (win32) heap");
	puts( "  -z x, zippy: add cpu load if x<0, error-check if x==0, x==1 for fastest correct output" );
	puts( "  -q, quiet: don't echo args, don't print circuit input and output" );
	exit( EXIT_FAILURE );
}

int main( int argc, char **argv ) {

	// default values for program arguments and file-header fields
	pArgs.lgn = 22L;
	fHead.n = (1LL << pArgs.lgn); // number of gates (currently must be a power of 2)

	pArgs.lgw = 8L;
	fHead.w = (1LL << pArgs.lgw); // width of circuit (currently must be a power of 2)

	pArgs.a = 256L; // number of circuit inputs, currently upper-bounded by its width
	fHead.a = pArgs.a;

	pArgs.b = 32L; // number of circuit outputs, currently upper-bounded by its width
	fHead.b = pArgs.b;

	pArgs.lgbs = 18L;  // file-blocks have (1LL << pArgs.lgbs) bytes
	fHead.lgbs = pArgs.lgbs; // gates don't cross file-block boundaries

	pArgs.gformat = 8244LL; // 8 2-input gates, 4 bytes per input desc, 4 bits per gate-type
	fHead.gformat = 8244LL;
	pArgs.gBlockSize = LGATE8244_T; // gate-block length, in bytes, for this format

	pArgs.gbasis = 0x0018; // {2-nand,2-nor}, could be single-bit gate-type field (8xx1 gate-format)
	fHead.gbasis = pArgs.gbasis;

	pArgs.input = NULL; // no input specified (a random input will be generated, if necessary)
	pArgs.createFile = False; // no BPW file will be created
	pArgs.zippy = 1; // no error-checking on data blocks.  Use -z0 when debugging.
	pArgs.quiet = True; // no diagnostic output, but echo args and print circuit I/O
	pArgs.circuitFileName = NULL; // no file specified, so we'll evaluate a random circuit by default
	pArgs.ospace = False; // if True, allocate file buffers in the OS/win32 heap, otherwise in program heap
	fHead.version = 0L; // prototype
	fHead.offsetToData = HEADER_LENGTH; // as defined in EvalCircuit.h

	// read args

	int opt = 0;
	char *ptr;
	opt = getopt( argc, argv, optString );
	while( opt != -1 ) {
		switch( opt ) {

			case 'n':
				pArgs.lgn = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -n must be numeric", optarg );
					return EXIT_FAILURE;
				}
				fHead.n = (1L << pArgs.lgn);
				break;

			case 'w':
				pArgs.lgw = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -w must be numeric", optarg );
					return EXIT_FAILURE;
				}
				fHead.w = (1L << pArgs.lgw);
				break;

			case 'a':
				pArgs.a = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -a must be numeric", optarg );
					return EXIT_FAILURE;
				}
				fHead.a = pArgs.a;
				break;

			case 'b':
				pArgs.b = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -b must be numeric", optarg );
					return EXIT_FAILURE;
				}
				fHead.b = pArgs.b;
				break;

			case 'g':
				pArgs.gformat = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 || pArgs.gformat > 99999 || pArgs.gformat < 1000 ) {
					printf( "Argument %s of -g must be four or five decimal digits", optarg);
					return EXIT_FAILURE;
				}
				fHead.gformat = pArgs.gformat;
				break;

			case 'h':
				pArgs.gbasis = strtoul( optarg, &ptr, 16 ); // hex input
				if( *ptr != 0 || pArgs.gbasis < 1 || pArgs.gbasis >= (1<<16) ) {
					printf( "Argument %s of -h must be one to four hex digits", optarg );
					return EXIT_FAILURE;
				}
				fHead.gbasis = pArgs.gbasis;
				break;

			case 's':
				pArgs.lgbs = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -s must be numeric", optarg );
					return EXIT_FAILURE;
				}
				fHead.lgbs = pArgs.lgbs;
				break;

			case 'i':
				pArgs.input = optarg;
				break;

			case 'f':
				pArgs.circuitFileName = optarg;
				if( optarg == NULL || !strcmp( optarg, "" ) ) {
					pArgs.circuitFileName = "default.bpw"; // default filenm
				}
				break;

			case 'z':
				pArgs.zippy = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 ) {
					printf( "Argument %s of -z must be numeric", optarg );
					return EXIT_FAILURE;
				}
				break;

			case 'q':
				pArgs.quiet = True;
				break;

			case 'o':
				pArgs.ospace = strtoul( optarg, &ptr, 10 );
				if( *ptr != 0 || pArgs.ospace < 0 || pArgs.ospace > 1 ) {
					printf( "Argument %s of -o must be 0 or 1", optarg );
					return EXIT_FAILURE;
				}
				break;

			case 'c':
				pArgs.circuitFileName = optarg;
				pArgs.createFile = True;
				if( optarg == NULL || !strcmp( optarg, "" ) ) {
					pArgs.circuitFileName = "default.bpw"; // default filenm
				}
				break;

			case '?':
				display_usage();
				break;

			default:
				display_usage();
				return EXIT_FAILURE;

		}

		opt = getopt( argc, argv, optString );

	}

	// open file

	if( !pArgs.createFile && pArgs.circuitFileName != NULL ) {
		open_file( ); // read headers; n.b. file params take precedence over command-line args
	}

	// validate parameters

	if( pArgs.lgw > 30 || fHead.w == 0 || (fHead.w % 8) != 0 ||
			fHead.w != (1L << pArgs.lgw) ) {
		printf( "This implementation can't evaluate a width-%lu circuit\n", fHead.w );
		return EXIT_FAILURE;
	}

	if( fHead.n % fHead.w != 0 ) {
		printf( "Irregular width %lu in this %lu-gate circuit\n", fHead.w, fHead.n );
		return EXIT_FAILURE;
	}

	if ( ( !pArgs.createFile || pArgs.circuitFileName == NULL ) &&
			(pArgs.a == 0 || pArgs.b == 0) ) {
		printf( "Can't evaluate a circuit with %lu inputs and %lu outputs\n", pArgs.a, pArgs.b );
		return EXIT_FAILURE;
	}

	uint64_t vmSize = 4 * fHead.w / 8;  // 4 banks of w bits = w/2 bytes in vm
	if( vmSize < 32 ) {
		vmSize = 32; // this simplifies bounds-checking in evalCirc8211() for very small w
	}
	pArgs.vm = (UByteP) calloc( vmSize, 1 );
	if( pArgs.vm == NULL ) {
		puts( "Unable to allocate enough working memory" );
		return EXIT_FAILURE;
	}

	if( pArgs.a > fHead.w ) {
		printf( "Too many inputs %lu for this incomplete implementation\n",	pArgs.a );
		printf( "Evaluating an %lu-input circuit\n", fHead.w );
		pArgs.a = fHead.w;
	} 	//TODO: implement COPY gate, to allow reads of a longer input tape

	if( pArgs.b > fHead.w ) {
		printf( "Too many outputs %lu for this incomplete implementation\n", pArgs.b );
		printf( "Evaluating an %lu-output circuit\n", fHead.w );
		pArgs.b = fHead.w;
		return EXIT_FAILURE;
	} //TODO: implement COPY gate, to allow writes to a longer output tape

	if( pArgs.lgbs > 30 ) {
		printf( "Excessive blocksize %lx\n", (1L<<pArgs.lgbs) );
		return EXIT_FAILURE;
	}

	if( pArgs.gbasis != 0x0018 ) { // { 2-nand, 2-nor }
		printf( "Unimplemented basis set for circuit: %x\n", pArgs.gbasis );
		return EXIT_FAILURE;
	}

	int parseGformat = pArgs.gformat;
	int gatesPerBlock = parseGformat / 1000;
	parseGformat -= 1000 * gatesPerBlock;
	int inputsPerGate = parseGformat / 100;
	parseGformat -= 100 * inputsPerGate;
	int bytesPerInput = parseGformat / 10;
	parseGformat -= 10 * bytesPerInput;
	int bitsPerGateType = parseGformat;
	pArgs.gBlockSize = gatesPerBlock * inputsPerGate * bytesPerInput +
			gatesPerBlock * bitsPerGateType / 8;

	// initialise

	// the circuit-input region of vm is its first pArgs.a bits
	// TODO: handle case of pArgs.a > pArgs.w, after COPY pseudogates are implemented
	if( !pArgs.createFile ) {
		if( pArgs.input == NULL ) {
			for( int ibi=0; ibi < ((pArgs.a - 1) / 8) + 1; ibi++ ) {
				pArgs.vm[ibi] = randomRDk( 8 );
			}
		} else {
			uint64_t ival = strtoul( pArgs.input, &ptr, 16 );
			int errsv = errno;
			if( *ptr != 0 || errsv != 0 ) {
				printf( "Stroul error %d: argument %s of -i must be hex, at most 16 digits",
						errsv, pArgs.input );
				return EXIT_FAILURE;
			}
			for( int ibi=0; ibi < ((pArgs.a - 1) / 8) + 1; ibi++ ) {
				pArgs.vm[ibi] = (unsigned char) (ival & 0xFF);
				ival = ival >> 8;
			}
		}
	}

	if( pArgs.createFile ) {
		if( !pArgs.quiet ) {
			printf( "Creating BPW file %s\n", pArgs.circuitFileName );
		}
		create_file ();  // open file for write, and write its header
	}

	if( pArgs.circuitFileName != NULL ) {
		alloc_fbuffers(); // prepare to read/write gates, now that blocksize is finalised
	}

	if( pArgs.ospace ) {
		printf( "Filebuffers are allocated from the OS heap." );
	}
	printf( "n = %.1f Mgates, w = %.1fMiB (= %.3fKiB), blocksize = %.1f KiB, ",
			(float) fHead.n/1.0e6,
			(float) fHead.w/1024.0/1024.0/8.0,
			(float) fHead.w/1024.0/8.0,
			(float)(1L << pArgs.lgbs)/1024.0
	);
	printf( "format = %lu, file = %s\n", pArgs.gformat, pArgs.circuitFileName );
	if( pArgs.zippy != 1 ) {
		printf( "Zippy = %d\n" , pArgs.zippy );
	}

	if( !pArgs.createFile ) {
		if( !pArgs.quiet ) {
			if( pArgs.circuitFileName != NULL ) {
				printf( "Circuit %s will be evaluated.\n", pArgs.circuitFileName );
			} else {
				puts( "A random circuit will be evaluated." );
			}
		}
		printf( "Circuit input: " ); //TODO: rewrite for long input tapes
		for( uint64_t inb = 0; inb < pArgs.a; inb += 8 ){
			unsigned char ichar = ByteInArray( pArgs.vm, pArgs.a - inb - 1 ); // little-endian
			printf( "%x%x", (ichar>>4) & 0xF, ichar & 0xF );
		}
		printf( "\n" );
	}

	// evaluate

	long obase = 3 * fHead.w; // circuit-output area (as an index into vm[])
	if( pArgs.gformat == 8211 ) {
		obase = evalOrCreateCirc8211( ); // corrects obase, if circuit has an odd number of levels
	} else if( pArgs.gformat == 8214 ) {
		if( pArgs.createFile ) {
			createCirc8214( ); // specialised for file-creation
		} else if( pArgs.circuitFileName != NULL ) {
			obase = evalCirc8214( ); // specialised for file-evals
		} else {
			obase = evalOrCreateCirc8221( ); // random-evals are format-agnostic
		}
 	} else if( pArgs.gformat == 8221 ) {
		obase = evalOrCreateCirc8221( );
	} else if( pArgs.gformat == 8231 ) {
		obase = evalOrCreateCirc8231( );
	} else if( pArgs.gformat == 8241 ) {
		obase = evalOrCreateCirc8241( );
	} else if( pArgs.gformat == 8224 ) {
		if( pArgs.createFile ) {
			createCirc8224( ); // specialised for file-creation
		} else if( pArgs.circuitFileName != NULL ) {
			obase= evalCirc8224( ); // specialised for file-evals
		} else {
			obase = evalOrCreateCirc8221( ); // random-evals are format-agnostic
		}
	} else if( pArgs.gformat == 8234 ) {
		if( pArgs.createFile ) {
			createCirc8234( ); // specialised for file-creation
		} else if( pArgs.circuitFileName != NULL ) {
			obase= evalCirc8234( ); // specialised for file-evals
		} else {
			obase = evalOrCreateCirc8221( ); // random-evals are format-agnostic
		}
	} else if( pArgs.gformat == 8244 ) {
		if( pArgs.createFile ) {
			createCirc8244( ); // specialised for file-creation
		} else if( pArgs.circuitFileName != NULL ) {
			obase= evalCirc8244( ); // specialised for file-evals
		} else {
			obase = evalOrCreateCirc8221( ); // random-evals are format-agnostic
		}
	} else {
		printf( "This implementation can't parse gates in g%lu format.\n", pArgs.gformat );
		exit( EXIT_FAILURE );
	}

	// terminate

	if( !pArgs.createFile ) {
		printf( "Circuit output: " ); //TODO: rewrite for long output tapes
		for( long outi=0; outi<pArgs.b; outi += 8 ){
			unsigned char ochar = ByteInArray(pArgs.vm, obase+outi);
			printf( "%x%x", (ochar>>4) & 0xF, ochar & 0xF ); // little-endian
		}
		printf( "\n" );
	}

	if( pArgs.circuitFileName != NULL ) {
		close_file( );
		free_handles();
		if( !pArgs.quiet ) {
			printf( "Released all win32 resources.\n" );
		}
	}

	return EXIT_SUCCESS;
}
