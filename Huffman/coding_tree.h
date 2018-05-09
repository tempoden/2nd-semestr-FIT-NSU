#ifndef _CODINGTREE_
#define _CODINGTREE_
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct treeNode treeNode;

struct treeNode {
	uint64_t priority;
	uint8_t value;
	size_t pos;
	bool is_leaf;

	treeNode * left;
	treeNode * right;
};

treeNode * treeNewNode(bool leaf, uint64_t chars_freq, uint8_t c, treeNode * left, treeNode * right);

treeNode * treeNewInternal(treeNode * left, treeNode * right);

treeNode * treeNewLeaf(uint64_t priority, uint8_t value);

void freeTree(treeNode * tree);


treeNode * buildFreqTree(size_t * chars_freq);
#endif