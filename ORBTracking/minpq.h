/*
���ļ�������С���ȶ���(С����)��ص�һЩ�ṹ�ͺ�������
*/
#pragma once

#include <stdlib.h>



#define MINPQ_INIT_NALLOCD 512  //��ʼ����ռ����

/*���ṹ*/
struct pq_node
{
	void* data;
	int key;
};

/*��С���ȶ��нṹ*/
struct min_pq
{
    struct pq_node* pq_array;     //���ָ��
    int nallocd;                 //����Ŀռ����
    int n;                        //Ԫ�ظ���
};



/*��ʼ����С���ȼ�����
*/

extern struct min_pq* minpq_init();

/*����Ԫ�ص����ȶ���
������
min_pq�����ȶ���
data:Ҫ���������
key:��data�����ļ�ֵ
����ֵ��0���ɹ���1��ʧ��
*/

extern int minpq_insert( struct min_pq* min_pq, void* data, int key );


/*�������ȶ����м�ֵ��С��Ԫ�أ�������ɾ����
������min_pq�����ȶ���
����ֵ����СԪ�ص�ָ��
*/

extern void* minpq_get_min( struct min_pq* min_pq );


/*���ز��Ƴ�������С��ֵ��Ԫ��
������min_pq�����ȼ�����
����ֵ����СԪ�ص�ָ��
*/

extern void* minpq_extract_min( struct min_pq* min_pq );

/*�ͷ����ȶ���
*/
extern void minpq_release( struct min_pq** min_pq );
