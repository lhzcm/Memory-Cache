#include <time.h>
#include "code.h"
#include "dataTree.h"

#ifndef _CORE_H_
#define _CORE_H_
#define revData_len 128
typedef struct RevData
{
	char data[revData_len];
	struct RevData* next;
}RevData;
int get_RevData_len(RevData* data,int position);
int RevDataToChar(RevData* data,char** chardata,int positio);
void free_RevData(RevData* data);
Code insert_data(RevData* data);
Code get_data(RevData* commdata);
Code delete_data(RevData* commdata);
Code updata_time(RevData* commdata);
Code updata_data(RevData* commdata);
Code updata_all(RevData* commdata);
#endif