#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "decoder.h"
#include "encoder.h"


int main(const int argc, const char** argv) 
{
	char * usage = "usage: huffman <-c/-d> <path1> <path2>\n";

	if (argc == 1)
	{	
		char buff[1024];
		const size_t buff_size = 1024;
		size_t cur_size = 0;

		FILE * fin = fopen("in.txt", "rb");
		FILE * temp = fopen("temp", "wb");

		fread(buff, 1, 3, fin);
		char mode = buff[0];
		while ((cur_size = fread(buff, 1, buff_size, fin)) > 0)
			fwrite(buff, cur_size, 1, temp);

		fclose(fin);
		fclose(temp);

		if (mode == 'c')
			encode("temp", "out.txt");
		if (mode == 'd')
			decode("temp", "out.txt");
	}
	else if (argc == 4)
	{
		if (!strcmp(argv[2], argv[3])) {
			printf("input diefferent filenames");
			return 0;
		}
		else if (!strcmp("-c", argv[1])) {
			encode(argv[2], argv[3]);
		}
		else if (!strcmp("-d", argv[1])) {
			decode(argv[2], argv[3]);
		}
		else 
			puts(usage);
	}
	else 
	{
		puts(usage);
	}

	return 0;
}