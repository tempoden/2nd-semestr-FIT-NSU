#include "decoder.h"

uint8_t treeGetChar(bitstream* input, treeNode* tree)
{
	if (tree->is_leaf)
		return tree->value;

	bool bit;
	bitstreamGetBit(input, &bit);


	return bit ? treeGetChar(input, tree->right) : treeGetChar(input, tree->left);
}


void rebuildTree(bitstream* input, treeNode* tree)
{
	bool bit;
READ:;
	if (!bitstreamGetBit(input, &bit))
		return;
	if (bit)
	{
		if (!tree->left)
		{
			tree->left = treeNewInternal(NULL, NULL);
			rebuildTree(input, tree->left);
			goto READ;
		}
		else
		{
			tree->right = treeNewInternal(NULL, NULL);
			rebuildTree(input, tree->right);
		}
	}
	else
	{
		if (!tree->left)
		{
			uint8_t ch;
			if (!bitstreamGetByte(input, &ch))
			{
				printf("Wrong tree layout!\n");
				exit(EXIT_FAILURE);
			}
			tree->left = treeNewLeaf(0, ch);
			goto READ;
		}
		else
		{
			uint8_t ch;
			if (!bitstreamGetByte(input, &ch))
			{
				printf("Wrong tree layout!\n");
				//exit(EXIT_FAILURE);
			}
			tree->right = treeNewLeaf(0, ch);
		}
	}
}


treeNode* readTree(bitstream* input)
{
	treeNode* tree = treeNewInternal(NULL, NULL);
	rebuildTree(input, tree);


	return tree;
}

void decode(const char* input_file_name, const char* output_file_name)
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


	bitstream input;
	bitstreamInit(&input, fin);
	bitstream output;
	bitstreamInit(&output, fout);

	uint32_t fileLen;
	bitstreamGetNBits(&input, &fileLen, 32);

	treeNode * tree = readTree(&input);

	while (fileLen-- > 0)
		bitstreamPutChar(&output, treeGetChar(&input, tree));

	freeTree(tree);

	fclose(fin);
	fclose(fout);
}