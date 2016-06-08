/*
  ���ļ��а���K-D���Ľ�������������������
*/

#pragma once

#include "cxcore.h"


/********************************* Structures ********************************/

struct feature;

/*K-D���еĽ��ṹ*/
/** a node in a k-d tree */
struct kd_node
{
    int ki;                      //�ָ�λ��(����)��ά������(��һά�Ƿָ�λ��)��ȡֵΪ1-128
    double kv;                     //�����ֵ(����������������������ά���ϵķ�������ֵ)
    int leaf;                     //�Ƿ�Ҷ�ӽ��ı�־
    struct feature* features;      //�˽���Ӧ�������㼯��(����)
    int n;                       //������ĸ���
    struct kd_node* kd_left;       //������
    struct kd_node* kd_right;      //������
};


/*************************** Function Prototypes *****************************/
/*���ݸ����������㼯�Ͻ���k-d��
������
features�����������飬ע�⣺�˺�������ı�features������Ԫ�ص�����˳��
n�����������
����ֵ�������õ�k-d��������ָ��
*/
extern struct kd_node* kdtree_build( struct feature* features, int n );


/*��BBF�㷨��k-d���в���ָ���������k�������������
������
kd_root��ͼ��������k-d��������
feat��Ŀ��������
k�����ڸ���
nbrs��k�������������ָ�����飬����Ŀ��������ľ�����������
      ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*nbrs)
max_nn_chks����������������������ֵ��������
����ֵ���洢��nbrs�еĽ��ڸ���������-1��ʾʧ��
*/
extern int kdtree_bbf_knn( struct kd_node* kd_root, struct feature* feat,
						  int k, struct feature*** nbrs, int max_nn_chks );


extern int kdtree_bbf_spatial_knn( struct kd_node* kd_root,
								struct feature* feat, int k,
								struct feature*** nbrs, int max_nn_chks,
								CvRect rect, int model );


/*�ͷ�k-d��ռ�õĴ洢�ռ�
*/
extern void kdtree_release( struct kd_node* kd_root );
