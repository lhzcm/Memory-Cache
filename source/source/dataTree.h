#include<stdlib.h>
#include<time.h>

#ifndef _DATATREE_H_
#define _DATATREE_H_

typedef struct dataTree
{
	unsigned int key;
	struct dataTree *left;
	struct dataTree *right;
	int height;
	char* data;
	time_t expire;
}dataTree;

dataTree* AVL_TREE_TEMP; //删除时用到的临时变量

dataTree* rr_rotation(dataTree* node);
dataTree* ll_rotation(dataTree* node);
dataTree* lr_rotation(dataTree* node);
dataTree* rl_rotation(dataTree* node);
dataTree* create_tree(unsigned int key);
dataTree* create_node(unsigned int key,dataTree* left,dataTree* right,char* nodeData,time_t timeExpire);
dataTree* delete_by_key(dataTree* node,unsigned int key,int* errorcode);
dataTree* left_to_top(dataTree* node);
dataTree* right_to_top(dataTree* node);
int get_height(dataTree* node);
int max_value(int first,int second);
void traverse(dataTree* node);
dataTree* insert_by_node(dataTree* node,dataTree* insertNode);

dataTree* get_by_key(dataTree* tree,unsigned key);

static unsigned int count_node=1;
static dataTree* data_tree=NULL;
#endif