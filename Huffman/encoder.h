#ifndef _ENCODER_
#define _ENCODER_
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bitstream.h"
#include "coding_tree.h"

#define ASCII_MAX_CHAR 256
#define INPUT_BUFF_SIZE 1024


void buildHuffmanCodes(treeNode* tree, uint8_t* huffman_codes[], uint8_t* s, int len);

void buildTreeLayout(treeNode* tree, uint8_t* tree_layout, size_t* pos);

size_t getFreq(const char* input_file_name, size_t chars_freq[]);

void writeTreeLayout(bitstream* stream, uint8_t* tree_layout, size_t bitsViewLen);

void fileCompression(const char* input_file_name, const char* output_file_name,
	uint8_t* huffman_codes[], uint8_t* tree_view, size_t total_len, size_t tree_view_len);

void encode(const char * input_file_name, const char * output_file_name);

#endif