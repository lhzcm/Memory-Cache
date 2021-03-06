#include<stdio.h>
#include "dataTree.h"

pthread_mutex_t malloc_free_mutex=PTHREAD_MUTEX_INITIALIZER; //malloc and free_mutex

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
	dataTree* tree=(dataTree*)malloc_safe(sizeof(dataTree));
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
dataTree* create_node(unsigned int key,dataTree* left,dataTree* right,char* data,time_t timeExpire)
{
	dataTree* node=(dataTree*)malloc_safe(sizeof(dataTree));
	if(node==NULL)
	{
		printf("Memory allocation failed!");
		return NULL;
	}
	node->key=key;
	node->height=0;
	node->left=left;
	node->right=right;
	node->data=data;
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
		printf("has same key!\n");
	}
	node->height=max_value(get_height(node->left),get_height(node->right))+1;
	return node;
}

//通过key来删除节点
dataTree* delete_by_key(dataTree* node,unsigned int key,int* errorcode)
{
	if(node==NULL)
	{
		printf("not found the node which key is:%d\n",key);
		*errorcode=-1;
		return NULL;
	}
	if(node->key>key)
	{
		node->left=delete_by_key(node->left,key,errorcode);
		if(get_height(node->right)-get_height(node->left)==2)
			if(get_height(node->right->right)>=get_height(node->right->left))
				node=rr_rotation(node);
			else
				node=rl_rotation(node);
	}
	else if(node->key<key)
	{
		node->right=delete_by_key(node->right,key,errorcode);
		if(get_height(node->left)-get_height(node->right)==2)
			if(get_height(node->left->left)>=get_height(node->left->right))
				node=ll_rotation(node);
			else
				node=lr_rotation(node);
	}
	else
	{
		if(node->height==0)
		{
			free_safe(node->data);
			free_safe(node);
			*errorcode=1;
			return NULL;
		}
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
		free_safe(node->data);
		free_safe(node);
		node=AVL_TREE_TEMP;
		*errorcode=1;
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

//通过key来获取节点
dataTree* get_by_key(dataTree* tree,unsigned key)
{
	while(tree!=NULL)
	{
		if(tree->key>key)
		{
			tree=tree->left;
			continue;
		}
		else if(tree->key<key)
		{
			tree=tree->right;
			continue;
		}
		else
		{
			return tree;
		}	
	}
	return NULL;
}
//malloc时加上互斥锁来保证malloc时的线程安全
void* malloc_safe(unsigned int size)
{
	void* position;
	pthread_mutex_lock(&malloc_free_mutex);
	position = malloc(size);
	pthread_mutex_unlock(&malloc_free_mutex);
	return position;
}
//free时加上互斥锁来保证free时的线程安全
void free_safe(void* target)
{
	pthread_mutex_lock(&malloc_free_mutex);
	free(target);
	pthread_mutex_unlock(&malloc_free_mutex);
}