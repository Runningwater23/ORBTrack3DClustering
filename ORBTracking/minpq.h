/*
此文件中是最小优先队列(小顶堆)相关的一些结构和函数声明
*/
#pragma once

#include <stdlib.h>



#define MINPQ_INIT_NALLOCD 512  //初始分配空间个数

/*结点结构*/
struct pq_node
{
	void* data;
	int key;
};

/*最小优先队列结构*/
struct min_pq
{
    struct pq_node* pq_array;     //结点指针
    int nallocd;                 //分配的空间个数
    int n;                        //元素个数
};



/*初始化最小优先级队列
*/

extern struct min_pq* minpq_init();

/*插入元素到优先队列
参数：
min_pq：优先队列
data:要插入的数据
key:与data关联的键值
返回值：0：成功，1：失败
*/

extern int minpq_insert( struct min_pq* min_pq, void* data, int key );


/*返回优先队列中键值最小的元素，但并不删除它
参数：min_pq：优先队列
返回值：最小元素的指针
*/

extern void* minpq_get_min( struct min_pq* min_pq );


/*返回并移除具有最小键值的元素
参数：min_pq：优先级队列
返回值：最小元素的指针
*/

extern void* minpq_extract_min( struct min_pq* min_pq );

/*释放优先队列
*/
extern void minpq_release( struct min_pq** min_pq );
