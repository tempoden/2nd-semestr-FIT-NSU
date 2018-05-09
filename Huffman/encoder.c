
#include "encoder.h"


void buildHuffmanCodes(treeNode* tree, uint8_t* huffman_codes[], uint8_t* s, int len) 
{
	if (!tree)
		return;

	if (tree->is_leaf) 
	{
		s[len] = 0;
		huffman_codes[tree->value] = (uint8_t*)malloc(sizeof(uint8_t) * (len + 1));
		strcpy((char*)huffman_codes[tree->value], (char*)s);
		
		printf("char: %1c code: %s\n", tree->value, huffman_codes[tree->value]);
		
		return;
	}

	s[len] = '0'; buildHuffmanCodes(tree->left, huffman_codes, s, len + 1);
	s[len] = '1'; buildHuffmanCodes(tree->right, huffman_codes, s, len + 1);
}


void buildTreeLayout(treeNode* tree, uint8_t* tree_layout, size_t* pos) 
{
	if (!tree)
		return;

	if (tree->left)
	{ 
		if (tree->left->is_leaf) 
		{
			tree_layout[(*pos)++] = '0';
			tree_layout[(*pos)++] = tree->left->value;
		}
		else
		{ 
			tree_layout[(*pos)++] = '1';
			buildTreeLayout(tree->left, tree_layout, pos);
		}
	}

	if (tree->right) 
	{
		if (tree->right->is_leaf) 
		{
			tree_layout[(*pos)++] = '0';
			tree_layout[(*pos)++] = tree->right->value;
		}
		else 
		{
			tree_layout[(*pos)++] = '1';
			buildTreeLayout(tree->right, tree_layout, pos);
		}
	}
	if (tree->is_leaf) {
		tree_layout[(*pos)++] = '0';
		tree_layout[(*pos)++] = tree->value;
	}
}


size_t getFreq(const char* input_file_name, size_t chars_freq[])
{
	FILE* fin = fopen(input_file_name, "rb");
	if (!fin) 
	{
		printf("Cannot open input file: \"%s\"", input_file_name);
		exit(EXIT_FAILURE);
	}

	uint8_t buff[INPUT_BUFF_SIZE];
	size_t buffSize = INPUT_BUFF_SIZE;
	size_t currSize = 0;
	size_t totalLen = 0;

	while ((currSize = fread(buff, 1, buffSize, fin)) > 0) 
	{
		totalLen += currSize;
		for (size_t i = 0; i < currSize; ++i)
			chars_freq[buff[i]]++;
	}

	fclose(fin);


	return totalLen;
}


void writeTreeLayout(bitstream* stream, uint8_t* tree_layout, size_t bitsViewLen) 
{
	bool isLastEnd = false;
	for (size_t i = 0; i < bitsViewLen; ++i) 
	{
		uint8_t currChar = tree_layout[i];
		if (isLastEnd) 
		{			
			bitstreamPutChar(stream, currChar);
			isLastEnd = false;
		}
		else if (currChar == '0') 
		{
			bitstreamPutBit(stream, false);
			isLastEnd = true;		
		}
		else if (currChar == '1')
			bitstreamPutBit(stream, true);
	}
}


void fileCompression (const char* input_file_name, const char* output_file_name, 
			uint8_t* huffman_codes[], uint8_t* tree_layout, size_t total_len, size_t tree_view_len) 
{
	FILE* fin = fopen(input_file_name, "rb");
	FILE* fout = fopen(output_file_name, "wb");
	if (!fin) 
	{
		printf("Cannot open input file: \"%s\"", input_file_name);
		exit(EXIT_FAILURE);
	}
	if (!fout) 
	{
		printf("Cannot open output file: \"%s\"", output_file_name);
		exit(EXIT_FAILURE);
	}


	uint8_t buff[INPUT_BUFF_SIZE];
	size_t currSize = 0;

	bitstream bs;
	bitstreamInit(&bs, fout);

	bitstreamPutNBits(&bs, (uint32_t)total_len, 32);	
	writeTreeLayout(&bs, tree_layout, tree_view_len);	

	// ????? ?????
	while ((currSize = fread(buff, 1, INPUT_BUFF_SIZE, fin)) > 0)
		for (size_t i = 0; i < currSize; i++) 
		{
			uint8_t* encodedChar = huffman_codes[buff[i]];
			bitstreamPutBinstr(&bs, encodedChar);
		}

	bitstreamDump(&bs);
	fclose(fin);
	fclose(fout);
}

void encode(const char* input_file_name, const char* output_file_name) 
{
	size_t chars_freq[ASCII_MAX_CHAR] = { 0 };
	size_t total_len = getFreq(input_file_name, chars_freq);
	treeNode * coding_tree = buildFreqTree(chars_freq);	
	uint8_t tree_layout[4096] = { 0 };				
	uint8_t* huffman_codes[ASCII_MAX_CHAR] = { 0 };			
	uint8_t s[32] = { 0 };
	size_t tree_layout_len = 0;

	buildHuffmanCodes(coding_tree, huffman_codes, s, 0);
	buildTreeLayout(coding_tree, tree_layout, &tree_layout_len);

	fileCompression(input_file_name, output_file_name, huffman_codes, tree_layout, total_len, tree_layout_len);
	
	for(size_t i = 0; i < ASCII_MAX_CHAR; ++i)
		free(huffman_codes[i]);
}