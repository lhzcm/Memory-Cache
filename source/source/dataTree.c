#include<stdio.h>
#include "../include/dataTree.h"
/*int main()
{*/
	/*****TEST1*****/
	/*
	int i;
	int arry[10]={12,34,45,23,3,675,13,9,100,66};
	dataTree* tree = create_node(1,NULL,NULL);
	for(i=0;i<10;i++)
	{
		tree=insert_by_key(tree,arry[i]);
	}
	traverse(tree);
	delete_by_key(tree,45);
	delete_by_key(tree,3);
	delete_by_key(tree,9);
	delete_by_key(tree,100);
	delete_by_key(tree,13);
	delete_by_key(tree,66);
	traverse(tree);
	*/
	/****TEST2****/
	/*
	int i;
	int arry[6]={2,6,1,3,7,4};
	dataTree* tree = create_node(5,NULL,NULL);
	for(i=0;i<6;i++)
	{
		tree=insert_by_key(tree,arry[i]);
	}
	traverse(tree);
	tree=delete_by_key(tree,7);
	traverse(tree);
	*/
	/****TEST3****/
	/*
	int i;
	int arry[12]={8,5,11,2,7,10,12,1,3,6,9,4};
	dataTree* tree = create_node(5,NULL,NULL);
	for(i=0;i<12;i++)
	{
		tree=insert_by_key(tree,arry[i]);
	}
	traverse(tree);
	tree=delete_by_key(tree,8);
	traverse(tree);
	*/
	/****TEST4****/
	/*
	int length=100,i;
	clock_t start,end;
	time_t thistime;
	dataTree* tree = NULL;
	start=clock();
	for(i=1;i<=length;i++)
	{
		tree=insert_by_key(tree,i);
	}
	end=clock();
	printf("%d\n",end-start);

	start=clock();
	for(i=99;i>3;i--)
	{
		tree=delete_by_key(tree,i);
	}
	end=clock();
	printf("%d\n",end-start);
	for(i=99;i>3;i--)
	{
		tree=insert_by_key(tree,i);
	}
	traverse(tree);
	time(&thistime);
	printf("%ld",thistime);
	*//*
	char* testdata;
	int length=100;
	unsigned i;
	clock_t start,end;
	time_t thistime;
	dataTree* tree = NULL;
	start=clock();
	for(i=1;i<=10;i++)
	{
		testdata=(char*)malloc(sizeof(char));
		scanf("%c",testdata);
		tree=insert_by_node(tree,create_node(i,NULL,NULL,testdata,19951009));
	}
	end=clock();
	printf("%d\n",end-start);
	traverse(tree);

	start=clock();
	for(i=10;i>0;i--)
	{
		tree=delete_by_key(tree,i);
	}
	end=clock();
	printf("%d\n",end-start);
	traverse(tree);
	time(&thistime);
	printf("%ld",thistime);

}*/

/*
**右右型左旋
**param:node 需要旋转的节点
**return 节点
*/
dataTree* rr_rotation(dataTree* node)
{
	dataTree* tnode=node->right;
	node->right=tnode->left;
	tnode->left=node;
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	tnode->height=max_value(get_height(tnode->left),get_height(tnode->right))+1;
	return tnode;
}
/*
**左左型右旋
**param:node 需要旋转的节点
**return 节点
*/
dataTree* ll_rotation(dataTree* node)
{
	dataTree* tnode=node->left;
	node->left=tnode->right;
	tnode->right=node;
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	tnode->height=max_value(get_height(tnode->left),get_height(tnode->right))+1;
	return tnode;
}
/*
**左右型先左旋后右旋
**param:node 需要旋转的节点
**return 节点
*/
dataTree* lr_rotation(dataTree* node)
{
	node->left=rr_rotation(node->left);
	return ll_rotation(node);
}
/*
**右左型先右旋后左旋
**param:node 需要旋转的节点
**return 节点
*/
dataTree* rl_rotation(dataTree* node)
{
	node->right=ll_rotation(node->right);
	return rr_rotation(node);
}

dataTree* create_tree(unsigned int key)
{	
	dataTree* tree=(dataTree*)malloc(sizeof(dataTree));
	if(tree==NULL)
	{
		printf("Memory allocation failed!");
		return NULL;
	}
	tree->key=key;
	tree->height=0;
	return tree;
}

/*
**创建AVL树的节点
**param:key 关键值
**param:left 左子树
**param:right 右子树
**return 节点
*/
dataTree* create_node(unsigned int key,dataTree* left,dataTree* right,RevData* nodeData,time_t timeExpire)
{
	dataTree* node=(dataTree*)malloc(sizeof(dataTree));
	if(node==NULL)
	{
		printf("Memory allocation failed!");
		return NULL;
	}
	node->key=key;
	node->height=0;
	node->left=left;
	node->right=right;
	node->data=nodeData;
	node->expire=timeExpire;
	return node;
}
dataTree* insert_by_node(dataTree* node,dataTree* insertNode)
{
	if(node==NULL)
	{
		return node=insertNode;
	}
	else if(node->key>insertNode->key)
	{
		node->left=insert_by_node(node->left,insertNode);
		if((get_height(node->left)-get_height(node->right))==2)
		{
			if(node->left->key>insertNode->key)
			{
				node=ll_rotation(node);
			}
			else
			{
				node=lr_rotation(node);
			}
		}
	}
	else if(node->key<insertNode->key)
	{
		node->right=insert_by_node(node->right,insertNode);
		if((get_height(node->right)-get_height(node->left))==2)
		{
			if(node->right->key<insertNode->key)
			{
				node=rr_rotation(node);
			}
			else
			{
				node=rl_rotation(node);	
			}
		}
	}
	else
	{
		printf("not allow insert same key");
	}
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	return node;
}
dataTree* delete_by_key(dataTree* node,unsigned int key)
{
	if(node==NULL)
	{
		printf("not found the node which key is:%d\n",key);
		return NULL;
	}
	if(node->key>key)
	{
		node->left=delete_by_key(node->left,key);
		if(get_height(node->right)-get_height(node->left)==2)
			if(get_height(node->right->right)>=get_height(node->right->left))
				node=rr_rotation(node);
			else
				node=rl_rotation(node);
	}
	else if(node->key<key)
	{
		node->right=delete_by_key(node->right,key);
		if(get_height(node->left)-get_height(node->right)==2)
			if(get_height(node->left->left)>=get_height(node->left->right))
				node=ll_rotation(node);
			else
				node=lr_rotation(node);
	}
	else
	{
		if(node->height==0)
			return NULL;
		if(get_height(node->left)>get_height(node->right))
		{
			AVL_TREE_TEMP->left=left_to_top(node->left);
			AVL_TREE_TEMP->right=node->right;
		}
		else
		{
			AVL_TREE_TEMP->right=right_to_top(node->right);
			AVL_TREE_TEMP->left=node->left;
		}
		free(node->data);
		free(node);
		node=AVL_TREE_TEMP;
	}
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	return node;
}
/*
**把要删除的节点最左边的节点填入删除的节点
**param:node 要删除的节点的右子树
**return 节点
*/
dataTree* left_to_top(dataTree* node)
{
	if(node->right==NULL)
	{
		AVL_TREE_TEMP=node;
		return node->left;
	}
	else 
	{
		node->right=left_to_top(node->right);
	}
	if(get_height(node->left)-get_height(node->right)==2)
	{
		if(get_height(node->left->left)>=get_height(node->left->right))
			node=ll_rotation(node);
		else
			node=lr_rotation(node);
	}
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	return node;
}

/*
**把要删除的节点最右边的节点填入删除的节点
**param:node 要删除的节点的左子树
**return 节点
*/
dataTree* right_to_top(dataTree* node)
{
	if(node->left==NULL)
	{
		AVL_TREE_TEMP=node;
		return node->right;
	}
	else 
	{	
		node->left=left_to_top(node->left);
	}
	if(get_height(node->right)-get_height(node->left)==2)
	{
		if(get_height(node->right->right)>=get_height(node->right->left))
			node=rr_rotation(node);
		else
			node=rl_rotation(node);
	}
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	return node;
}

/*
**param:node 要获取高度的节点
**return 节点的高度
*/
int get_height(dataTree *node)
{
	return node==NULL?-1:node->height;
}

int max_value(int first,int second)
{
	return first>second?first:second;
}
void traverse(dataTree* node)
{
	if(node==NULL)
		return;
	traverse(node->left);
	printf("%c\n",*node->data);
	traverse(node->right);
}