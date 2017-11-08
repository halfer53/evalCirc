/*
 * bpw_io.c
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 *  Based on: 18/03/2016 version authored by Rafik Nasraoui, https://github.com/rafikn/bpw
 *
 */

#include "evalcircuit.h"
#include "bpw_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#ifdef __linux__
#include <aio.h>
#endif
#include <math.h> // for log2()
#include <sys/stat.h>
#ifdef __CYGWIN__
#include <windows.h>
#endif
#include <assert.h>
// #include <zlib.h>  //TODO: use crc32() to compute crc for header
#include "bpw_linux.h"
#define FLOORLOG2(X) ((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((X)) - 1))

static int resSize= 0; // number of buffer bytes currently "reserved" for reading or writing

static char* magicStr = "BPW"; // "magic" (filetype) chars at the beginning of a .bpw file

// We allocate two structs of this type, referenced by curr_fb and next_fb
typedef struct {
#ifdef __linux__ 
	int fd;
	struct aiocb* cb;
#elif __CYGWIN__
	HANDLE hFile; // Windows file handle (using windows type convention)
	LPOVERLAPPED pOverlap; // this struct is reserved for use by the win32 runtime
#endif
	uint8_t *fbuffer;
	uint64_t fbsize; // number of bytes of data in this buffer
	uint64_t fbi; // index into fbuffer: next char
	uint64_t offset; // offset into file, for our reference (copied into pOverlap->Pointer)
} FileBuffer_t, *pFileBuffer_t;

static FileBuffer_t fb0 = {0}; // for header
static FileBuffer_t fb1 = {0}; // for data blocks
static FileBuffer_t fb2 = {0}; // for data blocks (double-buffered)

static pFileBuffer_t curr_fb; // current working area
static pFileBuffer_t next_fb; // buffer for read-ahead (in -f option) or write-behind (in -c option)
//TODO: each worker thread will need its own buffer

void ErrorExit(char *str);
void read_fbuffer(pFileBuffer_t pfb);

uint8_t * pCfbI( ) {
	return (uint8_t *) &( curr_fb->fbuffer[curr_fb->fbi] );
}

#ifdef __linux__ 
#define IOPENDING	EINPROGRESS
#define IOTIMEOUT	EAGAIN
#define IOCOMPLETE	0
#elif __CYGWIN__
#define IOPENDING	ERROR_IO_PENDING
#define IOTIMEOUT	WAIT_TIMEOUT
#define IOCOMPLETE	WAIT_OBJECT_0
#endif

DWORD get_err(){
#ifdef __linux__ 
	int val = aio_error( curr_fb->cb );
	return (DWORD)(val == -1 ? errno : val);
#elif __CYGWIN__
	return GetLastError();
#endif
}

int64_t get_file_size( ) {

#ifdef __linux__ 
	struct stat st;
	sys_chk(fstat(fb0.cb->aio_fildes, &st));
	return (int64_t)st.st_size;

#elif __CYGWIN__
	LARGE_INTEGER fs;
	BOOL bCompleted = GetFileSizeEx(fb0.hFile, &fs);
	if( !bCompleted ) {
		DWORD lastErr = get_err( );
		printf( "GetFileSizeEx() reported failure, with error code %d and filesize %lld",
				lastErr, fs.QuadPart );
		exit( EXIT_FAILURE );
	}
	return fs.QuadPart;
#endif
    
}

#ifdef __CYGWIN__

HANDLE get_file_handle() {
	HANDLE h;

	DWORD desired_access = (pArgs.createFile ? GENERIC_WRITE : GENERIC_READ);
	DWORD desired_share = (pArgs.createFile ? FILE_SHARE_WRITE : FILE_SHARE_READ);
	DWORD required_existing = (pArgs.createFile ? CREATE_ALWAYS : OPEN_EXISTING);

	h = CreateFile(pArgs.circuitFileName,
			desired_access,
			desired_share,
			NULL, // no security attributes
			required_existing,
			FILE_FLAG_OVERLAPPED, // async access mode
			NULL // no attributes copied from another file
		);
	if (h == INVALID_HANDLE_VALUE) {
		DWORD last_err = get_err();
		printf("Error code %d: unable to create handle for file \"%s\".\n",
				last_err, pArgs.circuitFileName);
		exit(last_err);
	}
	return (h);
}
#endif

// initialise both FileBuffers, allocating and reserving a small area for the file header


#ifdef __linux__ 
void init_fbuffers(){
	uint8_t *buf0;
	struct aiocb *cb1, *cb2, *cb3;
	int fd;
	cb1 = calloc(1, sizeof(struct aiocb));
	cb2 = calloc(1, sizeof(struct aiocb));
	cb3 = calloc(1, sizeof(struct aiocb));

	int accessflags = pArgs.createFile ? O_WRONLY : O_RDONLY;
	sys_chk(fd = open(pArgs.circuitFileName, O_CREAT | accessflags, S_IRUSR | S_IWUSR));

	cb1->aio_fildes = fd;
	cb2->aio_fildes = dup(fd);
	cb3->aio_fildes = dup(fd);

	cb2->aio_offset = fHead.offsetToData;

	fb0.cb = cb1;
	fb1.cb = cb2;
	fb2.cb = cb3;

	buf0 = calloc(1, HEADER_LENGTH); // we can't allocate buf1 and buf2 until we know blocksize
	fb0.fbuffer = buf0; // for reading the header
	fb0.fbsize = HEADER_LENGTH; // the first block of a BPW file holds its header

	fb1.fbuffer = 0; // can't read any blocks of data until the blocksize is known
	fb1.fbsize = 0; // data blocks are all the same size.... currently unknown
	fb2.fbuffer = 0; // can't read any blocks of data until the blocksize is known
	fb2.fbsize = 0; // data blocks are all the same size.... currently unknown

	fb0.fbi = 0;
	fb1.fbi = 0;
	fb2.fbi = 0;

	fb0.offset = 0;
	fb1.offset = fHead.offsetToData;
	fb2.offset = 0; // currently unknown...

	curr_fb = &fb0;
	next_fb = &fb1;
}


#elif __CYGWIN__

void init_fbuffers() {
	uint8_t *buf0;
	LPOVERLAPPED ov0;
	LPOVERLAPPED ov1;
	LPOVERLAPPED ov2;

	HANDLE h = get_file_handle();
	fb0.hFile = h;
	fb1.hFile = h;
	fb2.hFile = h;

	buf0 = calloc(1, HEADER_LENGTH); // we can't allocate buf1 and buf2 until we know blocksize

	ov0 = calloc(1, sizeof(OVERLAPPED));
	ov1 = calloc(1, sizeof(OVERLAPPED));
	ov2 = calloc(1, sizeof(OVERLAPPED));

	if ( ov0==0 || ov1==0 || ov2==0 || buf0==0  ) {
		printf("Unable to allocate buffers.\n");
		exit(EXIT_FAILURE);
	}

	if (!pArgs.quiet) {
		printf("ov0 at %p\n", ov0);
		printf("ov1 at %p\n", ov1);
		printf("ov2 at %p\n", ov2);
		printf("buf0 at %p\n", buf0);
	}

	ov0->Pointer = 0; // offset within file -- must be initially 0
	ov1->Pointer = 0; // offset within file -- must be initially 0
	ov2->Pointer = 0;

    HANDLE h_Ev0 = CreateEvent(NULL, FALSE, FALSE, "fb0 event");
    if (h_Ev0 == NULL)
    {
        printf("Unable to create wait event! Error: %u\n", get_err());
        exit(EXIT_FAILURE);
    }
    HANDLE h_Ev1 = CreateEvent(NULL, FALSE, FALSE, "fb1 event");
    if (h_Ev1 == NULL)
    {
        printf("Unable to create wait event! Error: %u\n", get_err());
        exit(EXIT_FAILURE);
    }
    HANDLE h_Ev2 = CreateEvent(NULL, FALSE, FALSE, "fb2 event");
    if (h_Ev2 == NULL)
    {
        printf("Unable to create wait event! Error: %u\n", get_err());
        exit(EXIT_FAILURE);
    }

    ov0->hEvent = h_Ev0;
    ov1->hEvent = h_Ev1;
    ov2->hEvent = h_Ev2;

    ov0->Pointer = (PVOID) 0; // 64-bit file-pointer (offset)
    ov1->Pointer = (PVOID) (long) fHead.offsetToData;
    ov2->Pointer = (PVOID) 0; // currently unknown...

	fb0.pOverlap = ov0;
    fb1.pOverlap = ov1;
	fb2.pOverlap = ov2;


	fb0.fbuffer = buf0; // for reading the header
	fb0.fbsize = HEADER_LENGTH; // the first block of a BPW file holds its header

	fb1.fbuffer = 0; // can't read any blocks of data until the blocksize is known
	fb1.fbsize = 0; // data blocks are all the same size.... currently unknown
	fb2.fbuffer = 0; // can't read any blocks of data until the blocksize is known
	fb2.fbsize = 0; // data blocks are all the same size.... currently unknown

	fb0.fbi = 0;
	fb1.fbi = 0;
	fb2.fbi = 0;

	fb0.offset = 0;
	fb1.offset = fHead.offsetToData;
	fb2.offset = 0; // currently unknown...

	curr_fb = &fb0;
	next_fb = &fb1;

}

#endif

void alloc_fbuffers() {
	uint8_t *buf1;
	uint8_t *buf2;
	long blockSize = (1L << pArgs.lgbs);

	// if( pArgs.ospace ) {
	// 	HANDLE heap = GetProcessHeap(); // handle of heap in win32 api
	// 	buf1 = HeapAlloc(heap, 0, blockSize);
	// 	buf2 = HeapAlloc(heap, 0, blockSize);
	// } else {
		buf1 = calloc(blockSize, 1);
		buf2 = calloc(blockSize, 1);
	// }

	if ( buf1==0 || buf2==0 ) {
		printf("Unable to allocate buffers.\n");
		exit(EXIT_FAILURE);
	}

	if (!pArgs.quiet) {
		printf("buf1 at %p\n", buf1);
		printf("buf2 at %p\n", buf2);
	}

	fb1.fbuffer = buf1;
	fb1.fbsize = (1LL << fHead.lgbs );
	fb2.fbuffer = buf2;
	fb2.fbsize = fb1.fbsize;

	if( !pArgs.createFile ) {
		read_fbuffer( &fb1 ); // prime the pump: start first data-read
	}

}

void write_fbuffer() {
	bool bCompleted;
	if( !pArgs.quiet ) {
//		#ifdef __CYGWIN__
//		printf("curr_fb->hFile at %p for WriteFile\n", curr_fb->hFile);
//		printf("curr_fb->pOverlap at %p for WriteFile\n", curr_fb->pOverlap);
//		printf("curr_fb->pOverlap->Pointer at %p for WriteFile\n", curr_fb->pOverlap->Pointer);
//		#endif

		printf("curr_fb->fbuffer at %p for WriteFile\n", curr_fb->fbuffer);
		printf("curr_fb->fbsize is %ld for WriteFile\n", curr_fb->fbsize);
		
	}

#ifdef __linux__ 
	curr_fb->cb->aio_buf = curr_fb->fbuffer;
	curr_fb->cb->aio_offset = curr_fb->fbsize;
	sys_chk(aio_write(curr_fb->cb));
	bCompleted = get_err() == IOCOMPLETE;
#elif __CYGWIN__
	bCompleted = WriteFile(curr_fb->hFile, curr_fb->fbuffer,
			(DWORD) curr_fb->fbsize,
			NULL,
			curr_fb->pOverlap // async write
			);
#endif
	if (!bCompleted) {
		DWORD last_err = get_err();
		if (last_err == IOPENDING) {
			if (!pArgs.quiet) {
				printf("Writing a block...");
			}
		} else {
			printf("\nError code %d when writing file \"%s\" asynchronously.\n", last_err,
				pArgs.circuitFileName);
			exit( EXIT_FAILURE );
		}
	} else {
		// very strange, IO is already complete
		printf("WriteFile completed immediately!\n");
	}

}

void wait_on_fb( pFileBuffer_t pfb ) {
#ifdef __linux__
	struct timespec timeoutspec = {1, 0};
	// timeoutspec.tv_sec = 1;
	// timeoutspec.tv_nsec = 0;
	const struct aiocb *const alist[1] = {pfb->cb};
	DWORD dwResult = aio_suspend(alist, 1, &timeoutspec);
#elif __CYGWIN__
	DWORD dwResult = WaitForSingleObject(pfb->pOverlap->hEvent, 1000); // timeout 1000 msec
#endif
	DWORD lastErr = get_err();

	if (dwResult == IOCOMPLETE) {
		if( !pArgs.quiet ) {
			printf("File I/O Completed\n");
		}
	} else {
		if (dwResult == IOTIMEOUT) {
			printf("Timeout waiting for file I/O to complete\n");
		} else {
			printf("File I/O error %d\n", lastErr );
		}

		#ifdef __linux__
		aio_cancel(pfb->cb->aio_fildes, pfb->cb);
		#elif __CYGWIN__
		CancelIo(pfb->hFile);
		#endif

		exit(EXIT_FAILURE);
	}
}

void read_fbuffer(pFileBuffer_t pfb) {

	if( !pArgs.quiet ) {
//		#ifdef __CYGWIN__
//		printf("pfb->hFile at %p for ReadFile\n", pfb->hFile);
//		printf("pfb->pOverlap at %p for ReadFile\n", pfb->pOverlap);
//		printf("pfb->pOverlap->Pointer at %p for ReadFile\n", pfb->pOverlap->Pointer);
//		#endif

		printf("pfb->fbuffer at %p for ReadFile\n", pfb->fbuffer);
		printf("pfb->fbsize is %lx for ReadFile\n", pfb->fbsize);
		
	}
#ifdef __linux__
	pfb->cb->aio_buf = pfb->fbuffer;
	pfb->cb->aio_offset = pfb->fbsize;
	sys_chk(aio_read(pfb->cb));
	BOOL bCompleted = get_err() == IOCOMPLETE;
#elif __CYGWIN__
	BOOL bCompleted = ReadFile(pfb->hFile,
			pfb->fbuffer,
			(DWORD) pfb->fbsize,
			NULL, // OVERLAPPED
			pfb->pOverlap // async
			);
#endif
	if (!bCompleted) {
		DWORD last_err = get_err();
		if (last_err == IOPENDING) {
			if( !pArgs.quiet ) {
				printf("Reading a block...");
			}
		} else {
			printf("Error code %d: unable to read file \"%s\" asynchronously.\n", last_err,
				pArgs.circuitFileName);
			exit(last_err);
		}
	} else {
		// very strange, IO is already complete
		if( !pArgs.quiet ) {
			printf("ReadFile completed immediately!\n");
		}
	}

}

void reserve_fbuffer(int gateSize) { // prepare to read or write a field
	// If curr_fb can't accommodate the reservation, then:
	//   zero-pad curr_fb (-c option), or verify its zero-padding (-f option)
	//   initiate an async-read (-f option), or an async-write (-c option), on curr_fb
	//   block until next_fb is free
	//   swap(curr_fb,next_fb)
	//   if writing: initialise fbi, offset, pOverlap->Pointer, and fbsize in curr_fb

	assert(resSize == 0); // all reserved bytes must be read (or written)
	resSize = gateSize;

	if( (curr_fb->fbi + gateSize) > curr_fb->fbsize ) {
		// field won't fit in current buffer
		if( pArgs.createFile ) {
			// 0-pad curr_fb
			for( ; curr_fb->fbi < curr_fb->fbsize; curr_fb->fbi++ ) {
				curr_fb->fbuffer[curr_fb->fbi] = 0;
			}
			// write curr_fb to disk
			write_fbuffer( );
		} else {
			// reading: verify 0-pad, then start next read
			for( ; curr_fb->fbi < curr_fb->fbsize; curr_fb->fbi++ ) {
				if( curr_fb->fbuffer[curr_fb->fbi] != 0 ) { //TODO: add CRC to each datablock
					puts( "Improper format detected in input file" );
					abort( );
				}
			}
			if( curr_fb == &fb0 ) { // special case to set up read of second data block
				curr_fb = &fb2;
				curr_fb->fbsize = (1LL << fHead.lgbs);
			}
			curr_fb->fbi = 0;
			curr_fb->offset = next_fb->offset + next_fb->fbsize;
			#ifdef __CYGWIN__
			curr_fb->pOverlap->Pointer = (PVOID) curr_fb->offset;
			#endif
			read_fbuffer( curr_fb );
		}
		// block until next_fb is available for use (general case)
		// special cases: first two writes
		if( pArgs.createFile ) {
			if( curr_fb == &fb0 ) { // first write
				// no writes are pending
			} else {
				if( curr_fb->offset == HEADER_LENGTH ) { // second write
					wait_on_fb( &fb0 ); // wait on prior write (header)
				} else {
					wait_on_fb( next_fb ); // wait on prior write
				}
			}
		} else {
			wait_on_fb( next_fb ); // wait on prior read
		}

		// swap buffers
		pFileBuffer_t pt;
		pt = ( (curr_fb == &fb0) ? &fb2 : curr_fb );
		curr_fb = next_fb;
		next_fb = pt;
		// TODO: we'll need w+1 buffers if we have w worker-threads

	    // if writing anything other than the header, we must initialise curr_fb
		if( pArgs.createFile && (next_fb->offset > 0) ) {
			curr_fb->fbi = 0;
			curr_fb->offset = next_fb->offset + next_fb->fbsize; // next data block
			#ifdef __CYGWIN__
			curr_fb->pOverlap->Pointer = (PVOID) curr_fb->offset;
			#endif
		}

	}
}

void release_reservation( ) { // explicit release
	curr_fb->fbi += resSize;
	resSize = 0;
}

/**
 * Write 8 bytes (64 bits) to a reserved space in curr_fb (little-endian)
 * Implicitly releases 8 bytes of the current reservation
 */
void write_64(uint64_t val) {
	// hack for speed (on any little-endian architecture with efficient unaligned writes)
	uint64_t *p = (uint64_t *) pCfbI();
	*p = val;
	curr_fb->fbi += 8;
	resSize -= 8;
}

/**
 * Write and release 4 bytes
 */
void write_32(uint32_t val) {
	uint32_t *p = (uint32_t *) pCfbI(); // hack
	*p = val;
	curr_fb->fbi += 4;
	resSize -= 4;
}

/**
 * Write and release 2 bytes
 */
void write_16(uint16_t val) {
	uint16_t *p = (uint16_t *) pCfbI(); // hack
	*p = val;
	curr_fb->fbi += 2;
	resSize -= 2;
}

/**
 * Write and release 1 byte
 */
void write_8(uint8_t val) {
	uint8_t *p = (uint8_t *) pCfbI(); // hack
	*p = val;
	curr_fb->fbi += 1;
	resSize -= 1;
}

/**
 * Write and release 3 bytes
 */
void write_24(uint32_t val) {
	write_16( (uint16_t) (val & 0xffff) );
	write_8( (uint8_t) ((val>>16) & 0xff) );
	resSize -= 3;
}

/**
 * Create a BPW FILE with header
 */
void create_file() {
	init_fbuffers();

	reserve_fbuffer(HEADER_LENGTH); // prepare to write file-header

	uint32_t *p = (uint32_t *) magicStr; // hack assumes a little-endian architecture (e.g. amd64)
	write_32(*p); // these magic bytes are "BPW", note the fourth byte is 0x0

	fHead.version = BPW_VERSION;
	write_32(fHead.version);
	fHead.offsetToData = HEADER_LENGTH;
	write_32(fHead.offsetToData);
	write_32(fHead.lgbs);
	write_64(fHead.w);
	write_64(fHead.n);
	write_64(fHead.a);
	write_64(fHead.b);
	write_64(fHead.gformat);
	write_32(fHead.gbasis);
	write_32(0); //TODO: write CRC32 of header fields here
}

/**
 * Open a BPW file for reading, read its headers
 */
void open_file() {

	init_fbuffers();
	read_fbuffer(curr_fb); // note: this is a short first block
	wait_on_fb(curr_fb); // block until first read is complete

	uint64_t fsize = get_file_size();
	if( fsize < HEADER_LENGTH ) {
		printf( "Corrupt bpw file of length %ld\n", fsize );
		exit( EXIT_FAILURE );
	}

	reserve_fbuffer(HEADER_LENGTH); // we'll read HEADER_LENGTH bytes from curr_fb

	// confirm correct magic (note: the first char in file is read into LSB of hdr)
	uint32_t hdr1 = read_32();
	if (hdr1 != *((uint32_t *) magicStr)) {
		printf("Incorrect file type: %x\n", hdr1);
		exit( EXIT_FAILURE);
	}
	strcpy((char *)fHead.magic, magicStr);

	uint32_t hdr2 = read_32(); // version
	if (hdr2 > BPW_VERSION) {
		printf("This file is BPW version %d, but this evaluator is BPW version %d\n.", hdr2,
				BPW_VERSION);
		exit( EXIT_FAILURE);
	}
	fHead.version = hdr2;

	uint32_t hdr3 = read_32(); // offsetToData
	if (hdr3 != HEADER_LENGTH) {
		printf("This file has header length %d, unreadable by this circuit evaluator\n.", hdr3);
		exit( EXIT_FAILURE);
	}
	fHead.offsetToData = hdr3;

	uint32_t hdr4 = read_32(); // lg(blockSize)
	if (hdr4 > 30) {
		printf("Excessive blocksize %lx\n.", (1L<<hdr4));
		exit( EXIT_FAILURE);
	}
	double nblocks = (fsize - fHead.offsetToData) / (1LL << hdr4);
	long fracblock = (fsize - fHead.offsetToData) % (1L << hdr4);
	// there must be at least one block of gate-descs, and no incomplete blocks
	if ((fsize < HEADER_LENGTH + (1LL<<hdr4)) || (fracblock != 0)) {
		printf(
			"Corrupt bpw file %s: %lu total size, with %u header and %f.4 blocks of size %lu.\n",
			pArgs.circuitFileName,
			fsize,
			fHead.offsetToData,
			nblocks,
			(1L<<hdr4)
		);
		exit( EXIT_FAILURE);
	}
	fHead.lgbs = hdr4;
	pArgs.lgbs = fHead.lgbs; //TODO: warn if a different blocksize was specified in commandline

	uint64_t hdr5 = read_64();  // width
	fHead.w = hdr5;
	pArgs.lgw = FLOORLOG2(fHead.w);
	//TODO: warn if a different width was specified in commandline
	//TODO: allow more widths; currently we require w to be a power of two
	if (fHead.w != (1L << pArgs.lgw)) {
		printf("Unsupported circuit width %lu\n", hdr5);
		exit( EXIT_FAILURE);
	}

	uint64_t hdr6 = read_64();  // number of gates
	fHead.n = hdr6;
	pArgs.lgn = FLOORLOG2(fHead.n);
	//TODO: warn if a different size was specified in commandline
	//TODO: allow more circuit sizes; currently we require n to be a power of two
	if (fHead.n != (1L << pArgs.lgn)) {
		printf("Unsupported circuit size %lu\n", hdr6);
		exit( EXIT_FAILURE);
	}

	uint64_t hdr7 = read_64();  // number of inputs
	if (hdr7 > (1L<<30)) {
		printf("Unsupported input size %lu\n", hdr7);
		exit( EXIT_FAILURE);
	}
	fHead.a = hdr7;
	pArgs.a = fHead.a; //TODO: warn if a different input-size was specified in commandline

	uint64_t hdr8 = read_64();  // number of outputs
	if (hdr8 > (1L<<30)) {
		printf("Unsupported output size %lu\n", hdr8);
		exit( EXIT_FAILURE );
	}
	fHead.b = hdr8;
	pArgs.b = fHead.b; //TODO: warn if a different output-size was specified in commandline

	uint64_t hdr9 = read_64(); // gate format
	fHead.gformat = hdr9;
	pArgs.gformat = fHead.gformat; // note: we'll update gatesPerBlock in EvalCircuit's main()

	uint32_t hdr10 = read_32(); // bitmask restricting the types of gates representable in this file
	if (hdr10 != 0x18) { 	//TODO: handle gbasis != 0x18.
		printf("Unsupported basis set: %u\n", hdr10);
		exit( EXIT_FAILURE );
	}
	fHead.gbasis = hdr10;

	uint32_t hdr11 = read_32(); // CRC32 of header
	if (hdr11 != 0x0) {
		printf("Corrupt header, CRC32 is %u but should be %u\n", 0, hdr11);
		exit( EXIT_FAILURE );
	}
	//TODO: compute crc32().  Currently all bpw files have 0 in headerCRC field.
	// http://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/zlib-crc32-1.html

	if ((curr_fb->fbi != HEADER_LENGTH) || (curr_fb->fbsize != HEADER_LENGTH)) {
		printf("Implementation error in open_file(), aborting.\n");
		exit( EXIT_FAILURE );
	}

}

/**
 * Read and release a reserved byte
 */
uint8_t read_8() {
	uint8_t val;
	val = *(uint8_t *) pCfbI();
	curr_fb->fbi += 1;
	resSize -= 1;
	return val;
}

/**
 * Read and release a 2B value
 */
uint16_t read_16() {
	uint16_t val;
    // hack for speed (on any little-endian architecture with efficient unaligned reads)
	val = *(uint16_t *) pCfbI();
	curr_fb->fbi += 2;
	resSize -= 2;
	return val;
}

/**
 * Read and release a 3B value
 */
uint32_t read_24() {
	uint32_t val;
	val = *(uint32_t *) pCfbI(); // read 4B (hack for speed)
	val &= 0xffffff; // extract 3B value
	curr_fb->fbi += 3;
	resSize -= 3;
	return val;
}

/**
 * Read and release a 4B value
 */
uint32_t read_32() {
	uint32_t val;
	val = *(uint32_t *) pCfbI(); // hack
	curr_fb->fbi += 4;
	resSize -= 4;
	return val;
}

/**
 * Read and release an 8-byte value
 */
uint64_t read_64() {
	uint64_t val;
	val = *(uint64_t *) pCfbI(); // hack
	curr_fb->fbi += 8;
	resSize -= 8;
	return val;
}

void close_file() {

	if (pArgs.createFile) {
		// flush curr_fb if it's dirty
		if (curr_fb->fbi > 0) {
			// pad curr_fb with 0s, wait on next_fb, then schedule another write
			reserve_fbuffer( (1L<<fHead.lgbs) - curr_fb->fbi + 1 );
		}
		// ensure the last write is completed before we close the file
		wait_on_fb(next_fb);
	}

	if (pArgs.createFile) {
		long nbytes = next_fb->offset + next_fb->fbsize;
		printf("%.2f Mbytes written.\n", (float)nbytes/1.0e6);
	} else {
		long nbytes = curr_fb->offset + curr_fb->fbsize;
		printf("%.2f Mbytes read.\n", (float)nbytes/1.0e6);
	}

	if( pArgs.createFile ) {
		#ifdef __linux__
		fsync(fb1.cb->aio_fildes);
		#elif __CYGWIN__
		FlushFileBuffers(fb1.hFile);
		#endif
		printf("Flushing writes to disk.\n");
	}
	#ifdef __linux__
	sys_chk(close(curr_fb->cb->aio_fildes));
	#elif __CYGWIN__

	BOOL bSuccess = CloseHandle(curr_fb->hFile);
	if (!bSuccess) {
		DWORD last_err = get_err();
		printf("Error code %d when closing \"%s\".\n", last_err,
				pArgs.circuitFileName);
		exit(last_err);
	}
	#endif
}

void free_handles() {
	#ifdef __linux__
	close(fb1.cb->aio_fildes);
	close(fb2.cb->aio_fildes);

	#elif __CYGWIN__
    CloseHandle(fb1.pOverlap->hEvent);
    CloseHandle(fb2.pOverlap->hEvent);
    if( pArgs.ospace ) {
    	HANDLE heap = GetProcessHeap(); // handle of heap in win32 api
        HeapFree(heap, 0, fb1.fbuffer);
        HeapFree(heap, 0, fb2.fbuffer);
        HeapFree(heap, 0, fb1.pOverlap);
        HeapFree(heap, 0, fb2.pOverlap);
	}
	#endif
}

#ifdef __CYGWIN__
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms680582(v=vs.85).aspx, without <strsafe.h>
void ErrorExit(char* str)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = get_err();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)str) + 40) * sizeof(CHAR));
    printf((LPTSTR)lpDisplayBuf,
        "%s failed with error %d: %s",
        str, dw, lpMsgBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf); //TODO: release all other resources!
    ExitProcess(dw);
}
#endif

