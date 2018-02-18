#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "avltree.h"

int main()
{
	struct AvltreeNode *tree = NULL;

	int n = 0;
	int cur = 0;

	scanf("%d", &n);

	while (n--)
	{
		scanf("%d", &cur);
		tree = treeInsert(tree, cur);
	}

	printf("%d\n", treeHeight(tree));

	freeTree(tree);

	return 0;
}