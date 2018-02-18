#ifndef _AVL_TREE
#define _AVL_TREE

#include <stdlib.h>


typedef struct AVL_TreeNode
{
	int key;
	unsigned char tree_height;
	struct AVL_TreeNode* left;
	struct AVL_TreeNode* right;
} TreeNode;

/*
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
||                           CREATES NEW TREE NODE                              ||
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
*/
TreeNode *newTreeNode(int key);

/*
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
||                         RETURNES HEIGHT OF AVL TREE                          ||
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
*/
unsigned char treeHeight(TreeNode* p);

/*
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
||                         COUNTS TREE BALANCE FACTOR                           ||
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
*/
int treeBalanceFactor(TreeNode* p);

/*
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
|| CORRECTS VALUE OF FIELD "height" IF CHILDREN HAVE CORRECT VALUE OF "height"  ||
||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||==||
*/
void treeFixHeight(TreeNode* p);


TreeNode* treeRotateRight(TreeNode* p);

TreeNode* treeRotateLeft(TreeNode* q);


TreeNode* treeBalance(TreeNode* p);

TreeNode* treeInsert(TreeNode* p, int k);

void freeTree(TreeNode* tree);

#endif