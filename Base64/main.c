/*								����� ��������
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
||	bites = (input[in] << 16) + (input[in+1] << 8) + input[in+2];				||
||	printf("bites %d", bites);													||
||	for (i = 0; i < 4; i++) {													||
||		output[out++] = base64[((bites & (63 << (6 * (3 - i)))) >> (6* (3-i)))];||
||		printf("  \n%d", ((bites & (63 << (6 * (3 - i)))) >> (6 * (3 - i))));	||
||  }																			||
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||

*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char base64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char getIndex(char c) {
	char i;
	for (i = 0; i < 65; i++) {
		if (c == base64[i]) {
			return i;
		}
	}
	return -1;
}

char* encode(char *input, size_t inputlen) {
	char *output = (char*)malloc((size_t)(1.4*inputlen) * sizeof(char));
	int out = 0,
		in = 0;
	unsigned char chr1, chr2, chr3;
	while (inputlen / 3 > 0) {
		
		chr1 = input[in++];
		chr2 = input[in++];
		chr3 = input[in++];
		

		output[out++] = base64[chr1 >> 2];
		output[out++] = base64[((chr1 & 3) << 4) | (chr2 >> 4)];
		output[out++] = base64[((chr2 & 15) << 2) | (chr3 >> 6)];
		output[out++] = base64[chr3 & 63];
		
		inputlen -= 3;
	}
	
	if (inputlen == 2) {
		
		chr1 = input[in++];
		chr2 = input[in++];

		output[out++] = base64[chr1 >> 2];
		output[out++] = base64[((chr1 & 3) << 4) | (chr2 >> 4)];
		output[out++] = base64[(chr2 & 15) << 2];
		output[out++] = '=';
	}
	else if (inputlen == 1) {
		
		chr1 = input[in++];

		output[out++] = base64[chr1 >> 2];
		output[out++] = base64[(chr1 & 3) << 4];
		output[out++] = '=';
		output[out++] = '=';
	}
	output[out] = EOF;
	return output;
}
char* decode(char* input, size_t inputlen) {
	char *output = (char*)malloc((size_t)(0.8*inputlen) * sizeof(char));
	unsigned char enc1, enc2, enc3, enc4;

	int in = 0,
		out = 0;

	while (inputlen > 0) {

		enc1 = getIndex(input[in++]);
		enc2 = getIndex(input[in++]);
		enc3 = getIndex(input[in++]);
		enc4 = getIndex(input[in++]);

		output[out++] = (enc1 << 2) | (enc2 >> 4);
		if (enc3 != 65) {
			output[out++] = ((enc2 & 15) << 4) | (enc3 >> 2);
			if (enc4 != 65) {
				output[out++] = ((enc3 & 3) << 6) | enc4;
			}
			else {
				output[out++] = (enc3 & 3) << 6;
			}
		}
		else {
			output[out++] = (enc2 & 15) << 4;
		}

		inputlen -= 4;

	}

	output[out] = EOF;

	return output;
}
int main(int argc, char** argv) {
	char *input, *output = "\0", *flag, *inf, *outf;
	size_t os, bs;
	FILE *in, *out;
	
	flag = argv[1];
	inf = argv[2];
	outf = argv[3];

	in = fopen(inf, "r");
	fseek(in, 0, SEEK_END);
	bs = ftell(in);
	printf("%zu", bs);
	input = (char*)malloc(bs);
	fseek(in, 0, SEEK_SET);
	printf("\n%d",fread(input, 1, bs, in));
	fclose(in);

	if (strcmp(flag,"-e") == 0) {
		output = encode(input,bs);
		os = bs * (4.0 / 3) + bs % 3;
	}
	else if (strcmp(flag, "-d") == 0) {
		output = decode(input,bs);
		os = bs * (3.0 / 4);
	}

	out = fopen(outf, "wb");
	fwrite(output, sizeof(char),os,out);
	fclose(out);

	/*free(input);
	free(output);*/
	return 0;
}