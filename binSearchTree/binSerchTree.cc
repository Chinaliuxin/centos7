#include "Search.h"

BSTreeNode* BuyBSTreeNode(BSTDataType data)
{
	BSTreeNode* pNewNode = (BSTreeNode*)malloc(sizeof(BSTreeNode));
	if (pNewNode == NULL)
	{
		assert(0);
		printf("节点创建失败！\n");
		return NULL;
	}
	pNewNode->_data = data;
	pNewNode->_left = NULL;
	pNewNode->_right = NULL;
	return pNewNode;
}
//插入
int BSTreeInsert(BSTreeNode** tree, BSTDataType data)
{
	BSTreeNode* pCur = NULL;
	BSTreeNode* pParent = NULL;
	if (NULL == *tree)
	{
		*tree = BuyBSTreeNode(data);
		return 1;
	}
	pCur = *tree;
	while (pCur)
	{
		if (pCur->_data > data)
		{
			pParent = pCur;
			pCur = pCur->_left;
		}
		else if (pCur->_data < data)
		{
			pParent = pCur;
			pCur = pCur->_right;
		}
		else
		{
			return 0;
		}
	}
	if (pParent->_data < data)
	{
		pParent->_right = BuyBSTreeNode(data);
	}
	else
	{
		pParent->_left = BuyBSTreeNode(data);
	}
	return 1;
}
//删除
int BSTreeRemove(BSTreeNode** tree, BSTDataType data)  //改变指针时需要传入指针的指针
{
	BSTreeNode* pCur = NULL;
	BSTreeNode* pParent = NULL;
	pCur = *tree;
	while (pCur)
	{
		if (pCur->_data > data)
		{
			pParent = pCur;
			pCur = pCur->_left;
		}
		else if (pCur->_data < data)
		{
			pParent = pCur;
			pCur = pCur->_right;
		}
		else
		{
			//找到了开始删除
			//1.左/右为空
			if (pCur->_left == NULL)
			{
				if (NULL == pParent)
				{
					*tree = pCur->_right;
				}
				else
				{
					if (pParent->_left == pCur)
					{
						pParent->_left = pCur->_right;
					}
					else
					{
						pParent->_right = pCur->_right;
					}

				}
			}
			else if (pCur->_right == NULL)
			{
				if (NULL == pParent)
				{
					*tree = pCur->_left;
				}
				else
				{
					if (pParent->_left == pCur)
					{
						pParent->_left = pCur->_left;
					}
					else
					{
						pParent->_right = pCur->_left;
					}
				}			
			}
			//2.左右都不空
			else
			{
				BSTreeNode* replace = pCur->_right;
				while (replace->_left)
				{
					replace = replace->_left;
				}
				pCur->_data = replace->_data;
				return BSTreeRemove(&pCur->_right, replace->_data);
			}
			free(tree);
			return 1;
		}
	}
	return 0;
}
//查找
BSTreeNode* BSTreeFind(BSTreeNode** tree, BSTDataType data)
{
	BSTreeNode* pCur = *tree;
	while (pCur)
	{
		if (pCur->_data > data)
		{
			pCur = pCur->_left;
		}
		else if (pCur->_data < data)
		{
			pCur = pCur->_right;
		}
		else
		{
			return pCur;
		}
	}
	return NULL;
}

//递归实现
int BSTreeInsertR(BSTreeNode** tree, BSTDataType data)
{
	if (NULL == *tree)
	{
		*tree = BuyBSTreeNode(data);
		return 1;
	}
	if ((*tree)->_data > data)
	{
		return BSTreeInsertR(&(*tree)->_left, data);
	}
	else if ((*tree)->_data < data)
	{
		return  BSTreeInsertR(&(*tree)->_right, data);
	}
	else
	{
		return 0;
	}
}

int BSTreeRemoveR(BSTreeNode** tree, BSTDataType data)
{
	if (NULL == *tree)
	{
		return -1;
	}
	if ((*tree)->_data > data)
	{
		return BSTreeRemoveR(&(*tree)->_left, data);
	}
	else if ((*tree)->_data < data)
	{
		return BSTreeRemoveR(&(*tree)->_right, data);
	}
	//走到这里说明找到了要删除的节点
	else
	{
		BSTreeNode* del = *tree;
		//左为空
		if (NULL == (*tree)->_left)
		{
			*tree = (*tree)->_right;
			free(del);
		}
		//右为空
		else if (NULL == (*tree)->_right)
		{
			*tree = (*tree)->_left;
		}
		//左右都不空
		else
		{
			BSTreeNode* replace = (*tree)->_right;
			while (replace->_left)
			{
				replace = replace->_left;
			}
			(*tree)->_data = replace->_data;
			return BSTreeRemoveR(&(*tree)->_right, replace->_data);
		}

	}
	return 1;
}

BSTreeNode* BSTreeFindR(BSTreeNode** tree, BSTDataType data)
{
	if (NULL == *tree)
	{
		return NULL;
	}
	if ((*tree)->_data > data)
	{
		return BSTreeFindR(&(*tree)->_left, data);
	}
	else if ((*tree)->_data < data)
	{
		return BSTreeFindR(&(*tree)->_right, data);
	}
	else
	{
		return *tree;
	}
}

void BStreeDestroy(BSTreeNode** tree)
{
	assert(*tree);
	if (*tree)
	{
		BStreeDestroy(&(*tree)->_left);
		BStreeDestroy(&(*tree)->_right);
		free(*tree);
		*tree = NULL;
	}
}
void BSTreeInOrder(BSTreeNode** tree)
{
	if (NULL == *tree)
	{
		return;
	}
	BSTreeInOrder(&((*tree)->_left));
	printf("%d ", (*tree)->_data);
	BSTreeInOrder(&((*tree)->_right));
}
