/*
此文件中包括最小优先队列的实现
*/
#include "stdafx.h"
#include "minpq.h"
#include "utils.h"

#include <limits.h>


//本地函数

static void restore_minpq_order(struct pq_node*, int, int);
static void decrease_pq_node_key(struct pq_node*, int, int);


//内联函数

//返回i节点的父节点的下标
static __inline int parent(int i)
{
	return (i - 1) / 2;
}

//返回i节点的右孩子的下标
static __inline int right(int i)
{
	return 2 * i + 2;
}

//返回i节点的左孩子的下标
static __inline int left(int i)
{
	return 2 * i + 1;
}



/*初始化最小优先级队列，分配默认大小的空间
*/
struct min_pq* minpq_init()
{
	struct min_pq* min_pq;

	min_pq = (struct min_pq*)malloc(sizeof(struct min_pq));
	//默认分配MINPQ_INIT_NALLOCD个空间
	min_pq->pq_array = (struct pq_node*)calloc(MINPQ_INIT_NALLOCD, sizeof(struct pq_node));
	min_pq->nallocd = MINPQ_INIT_NALLOCD;
	min_pq->n = 0;

	return min_pq;
}


int minpq_insert(struct min_pq* min_pq, void* data, int key)
{
	int n = min_pq->n;

	if (min_pq->nallocd == n)
	{
		min_pq->nallocd = array_double((void**)(&min_pq->pq_array), min_pq->nallocd,
			sizeof(struct pq_node));
		if (!min_pq->nallocd)
		{
			fprintf(stderr, "Warning: unable to allocate memory, %s, line %d\n",
				__FILE__, __LINE__);
			return 1;
		}
	}

	min_pq->pq_array[n].data = data;
	min_pq->pq_array[n].key = INT_MAX;
	decrease_pq_node_key(min_pq->pq_array, min_pq->n, key);
	min_pq->n++;

	return 0;
}


void* minpq_get_min(struct min_pq* min_pq)
{
	if (min_pq->n < 1)
	{
		fprintf(stderr, "Warning: PQ empty, %s line %d\n", __FILE__, __LINE__);
		return NULL;
	}
	return min_pq->pq_array[0].data;
}


void* minpq_extract_min(struct min_pq* min_pq)
{
	void* data;

	if (min_pq->n < 1)
	{
		fprintf(stderr, "Warning: PQ empty, %s line %d\n", __FILE__, __LINE__);
		return NULL;
	}
	data = min_pq->pq_array[0].data;
	min_pq->n--;
	min_pq->pq_array[0] = min_pq->pq_array[min_pq->n];
	restore_minpq_order(min_pq->pq_array, 0, min_pq->n);

	return data;
}


void minpq_release(struct min_pq** min_pq)
{
	if (!min_pq)
	{
		fprintf(stderr, "Warning: NULL pointer error, %s line %d\n", __FILE__,
			__LINE__);
		return;
	}
	if (*min_pq && (*min_pq)->pq_array)
	{
		free((*min_pq)->pq_array);
		free(*min_pq);
		*min_pq = NULL;
	}
}


/************************ 函数原型 **************************/
static void decrease_pq_node_key(struct pq_node* pq_array, int i, int key)
{
	struct pq_node tmp;

	if (key > pq_array[i].key)
		return;

	pq_array[i].key = key;
	while (i > 0 && pq_array[i].key < pq_array[parent(i)].key)
	{
		tmp = pq_array[parent(i)];
		pq_array[parent(i)] = pq_array[i];
		pq_array[i] = tmp;
		i = parent(i);
	}
}


static void restore_minpq_order(struct pq_node* pq_array, int i, int n)
{
	struct pq_node tmp;
	int l, r, min = i;

	l = left(i);
	r = right(i);
	if (l < n)
	if (pq_array[l].key < pq_array[i].key)
		min = l;
	if (r < n)
	if (pq_array[r].key < pq_array[min].key)
		min = r;

	if (min != i)
	{
		tmp = pq_array[min];
		pq_array[min] = pq_array[i];
		pq_array[i] = tmp;
		restore_minpq_order(pq_array, min, n);
	}
}
