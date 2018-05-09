#include "bitstream.h"


void bitstreamInit(bitstream * bitstream, FILE * stream)
{
	bitstream->stream = stream;
	bitstream->buffer = 0;
	bitstream->buffer_remain = 0;
}

void bitstreamPutNBits(bitstream * bitstream, uint32_t output, size_t count)
{
	if (!count)
		return;

	count = count > 32 ? 32 : count;

	for (size_t i = 0; i < count; i++)
	{
		bitstream->buffer <<= 1;
		bitstream->buffer |= (output & (1 << (count - i - 1))) != 0;

		if (++bitstream->buffer_remain == BUFF_SIZE)
		{ 
			fwrite(&bitstream->buffer, 1, 1, bitstream->stream);
			bitstream->buffer_remain = 0;
			bitstream->buffer = 0;
		}
	}
}

void bitstreamPutBit(bitstream * bitstream, bool bit)
{
	bitstreamPutNBits(bitstream, bit, 1);
}

void bitstreamPutChar(bitstream * bitstream, uint8_t ch)
{
	bitstreamPutNBits(bitstream, ch, 8);
}

void bitstreamPutBinstr(bitstream * bitstream, uint8_t * str)
{
	size_t len = strlen((const char*)str);
	for (size_t i = 0; i < len; i++)
		bitstreamPutBit(bitstream, (str[i] == '0' ? 0 : 1));
}

bool bitstreamGetBit(bitstream * bitstream, bool * bit)
{
	uint32_t buff;
	bool result = bitstreamGetNBits(bitstream, &buff, 1);
	*bit = buff % 2;


	return result;
}

bool bitstreamGetByte(bitstream * bitstream, uint8_t * ch)
{
	uint32_t buff;
	bool result = bitstreamGetNBits(bitstream, &buff, 8);
	*ch = buff & 255;


	return result;
}

bool bitstreamGetNBits(bitstream * bitstream, uint32_t * input, size_t count)
{
	if (count > 32)
		return false;

	for (size_t i = 0; i < count; i++)
	{
		if (!bitstream->buffer_remain)
		{ 
			if (!fread(&bitstream->buffer, 1, 1, bitstream->stream))
				return false;
			bitstream->buffer_remain = 8;
		}

		bitstream->buffer_remain--;
		(*input) <<= 1;
		(*input) |= (bitstream->buffer & (1 << bitstream->buffer_remain)) != 0;
	}


	return true;
}

void bitstreamDump(bitstream* bitstream)
{
	if (bitstream->buffer_remain)	
	{ 
		bitstream->buffer <<= BUFF_SIZE - bitstream->buffer_remain;
		fwrite(&bitstream->buffer, 1, 1, bitstream->stream);
	}
}