#include "coding_tree.h"
#include "pqueue.h"

#define ASCII_MAX_CHAR 256



static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr)
{
	return (next > curr);
}

static pqueue_pri_t get_pri(void *a)
{
	return ((treeNode *)a)->priority;
}

static void set_pri(void *a, pqueue_pri_t pri)
{
	((treeNode *)a)->priority = pri;
}

static size_t get_pos(void *a)
{
	return ((treeNode *)a)->pos;
}

static void set_pos(void *a, size_t pos)
{
	((treeNode *)a)->pos = pos;
}


treeNode * treeNewNode(bool leaf, uint64_t chars_freq, uint8_t c, treeNode * left, treeNode * right)
{
	treeNode * node = (treeNode *)malloc(sizeof(treeNode));
	node->value = c;
	node->left = left, node->right = right;
	node->is_leaf = leaf;

	if (leaf)
		node->priority = chars_freq;
	else
		node->priority = (left ? left->priority : 0) + (right ? right->priority : 0);

	return node;
}

treeNode* treeNewInternal(treeNode* left, treeNode* right)
{
	return treeNewNode(false, 0, 0, left, right);
}

treeNode* treeNewLeaf(uint64_t priority, uint8_t value)
{
	return treeNewNode(true, priority, value, NULL, NULL);
}

void freeTree(treeNode* tree)
{
	if (!tree)
		return;

	freeTree(tree->left);
	freeTree(tree->right);
	free(tree);
}

treeNode* buildFreqTree(size_t * chars_freq)
{
	pqueue_t * pq = pqueue_init(ASCII_MAX_CHAR, cmp_pri, get_pri, set_pri, get_pos, set_pos);

	for (size_t i = 0; i < ASCII_MAX_CHAR; ++i)
		if (chars_freq[i])
			pqueue_insert(pq, treeNewLeaf(chars_freq[i], (uint8_t)i));


	while (pqueue_size(pq) > 1)
		pqueue_insert(pq, treeNewInternal(pqueue_pop(pq), pqueue_pop(pq)));

	treeNode* tree = (treeNode*)pqueue_pop(pq);
	pqueue_free(pq);


	return tree;
}