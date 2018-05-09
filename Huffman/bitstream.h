#ifndef _BITSTREAM_
#define _BITSTREAM_
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define BUFF_SIZE 8

typedef struct bitstream bitstream;

struct bitstream {
	FILE *	stream;
	uint8_t buffer;
	size_t	buffer_remain;
};


void bitstreamInit(bitstream * bitstream, FILE * stream);


void bitstreamPutNBits(bitstream * bitstream, uint32_t output, size_t count);


void bitstreamPutBit(bitstream * bitstream, bool bit);


void bitstreamPutChar(bitstream * bitstream, uint8_t ch);


void bitstreamPutBinstr(bitstream * bitstream, uint8_t * str);


bool bitstreamGetBit(bitstream * bitstream, bool * bit);


bool bitstreamGetByte(bitstream * bitstream, uint8_t * ch);


bool bitstreamGetNBits(bitstream * bitstream, uint32_t * input, size_t count);


void bitstreamDump(bitstream * bitstream);
#endif