#ifndef _DECODER_
#define _DECODER_
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>

#include "bitstream.h"
#include "coding_tree.h"

uint8_t treeGetChar(bitstream* input, treeNode* tree);

void rebuildTree(bitstream* input, treeNode* tree);

treeNode* readTree(bitstream* input);



void decode(const char * input_file_name, const char * output_file_name);

#endif