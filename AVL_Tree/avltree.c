#include "avltree.h"

TreeNode *newTreeNode(int key)
{
	TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));

	n->key = key;
	n->tree_height = 1;
	n->left = n->right = 0;

	return n;
}

unsigned char treeHeight(TreeNode* p)
{
	return (p) ? (p->tree_height) : (0);
}

int treeBalanceFactor(TreeNode* p)
{
	return treeHeight(p->right) - treeHeight(p->left);
}

void treeFixHeight(TreeNode* p)
{
	unsigned char hl = treeHeight(p->left);
	unsigned char hr = treeHeight(p->right);
	p->tree_height = ((hl > hr) ? (hl) : (hr)) + 1;
}

TreeNode* treeRotateRight(TreeNode* p)
{
	TreeNode* q = p->left;
	p->left = q->right;
	q->right = p;
	treeFixHeight(p);
	treeFixHeight(q);

	return q;
}

TreeNode* treeRotateLeft(TreeNode* q)
{
	TreeNode* p = q->right;
	q->right = p->left;
	p->left = q;
	treeFixHeight(q);
	treeFixHeight(p);

	return p;
}

TreeNode* treeBalance(TreeNode* p)
{
	treeFixHeight(p);

	if (treeBalanceFactor(p) == 2)
	{
		if (treeBalanceFactor(p->right) < 0)
			p->right = treeRotateRight(p->right);

		return treeRotateLeft(p);
	}

	if (treeBalanceFactor(p) == -2)
	{
		if (treeBalanceFactor(p->left) > 0)
			p->left = treeRotateLeft(p->left);

		return treeRotateRight(p);
	}

	return p;
}

TreeNode* treeInsert(TreeNode* p, int k)
{
	if (!p)
		return newTreeNode(k);

	if (k < p->key)
		p->left = treeInsert(p->left, k);
	else
		p->right = treeInsert(p->right, k);

	return treeBalance(p);
}

void freeTree(TreeNode* tree)
{
	if (tree == NULL)
		return;

	freeTree(tree->left);
	freeTree(tree->right);

	free(tree);
}