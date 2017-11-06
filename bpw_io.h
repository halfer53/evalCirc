/*
 * bpw_io.h
 *
 *  Created on: 25/05/2017
 *      Author: ctho065
 */

#ifndef BPW_IO_H_
#define BPW_IO_H_

#include <fileapi.h>

// reserve_fbuffer() must be called to reserve space before any read or write
void reserve_fbuffer( int gateSize );

// explicit release of reservation, for efficient (single-copy) 8-gate I/O in getGate.c
void release_reservation( );

// support for single-copy I/O: returns pointer to the first reserved byte in the current fbuffer
uint8_t * pCfbI( );

void write_64( uint64_t val );  // write on reserved area, releasing the portion written
void write_32( uint32_t val );
void write_24( uint32_t val );
void write_16( uint16_t val );
void write_8( uint8_t val );

uint8_t read_8( ); // read on reserved area, releasing the portion read
uint16_t read_16( );
uint32_t read_24( );
uint32_t read_32( );
uint64_t read_64( );

void alloc_fbuffers( ); // allocate large buffers for file I/O (using blocksize from bpw header)

void create_file( ); // create new file, initialise curr_fb for write, allocate next_fb

void open_file( ); // open existing file, initialise curr_fb for read, initiate read on next_fb (if file has more than one block)

void close_file( ); // closes file (after flushing fbuffers, if writing)

void free_handles( ); // release event handles and free heap-space in OS 32-bit address space

#endif /* BPW_IO_H_ */
