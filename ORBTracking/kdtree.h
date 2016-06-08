/*
  此文件中包含K-D树的建立与搜索函数的声明
*/

#pragma once

#include "cxcore.h"


/********************************* Structures ********************************/

struct feature;

/*K-D树中的结点结构*/
/** a node in a k-d tree */
struct kd_node
{
    int ki;                      //分割位置(枢轴)的维数索引(哪一维是分割位置)，取值为1-128
    double kv;                     //枢轴的值(所有特征向量在枢轴索引维数上的分量的中值)
    int leaf;                     //是否叶子结点的标志
    struct feature* features;      //此结点对应的特征点集合(数组)
    int n;                       //特征点的个数
    struct kd_node* kd_left;       //左子树
    struct kd_node* kd_right;      //右子树
};


/*************************** Function Prototypes *****************************/
/*根据给定的特征点集合建立k-d树
参数：
features：特征点数组，注意：此函数将会改变features数组中元素的排列顺序
n：特征点个数
返回值：建立好的k-d树的树根指针
*/
extern struct kd_node* kdtree_build( struct feature* features, int n );


/*用BBF算法在k-d树中查找指定特征点的k个最近邻特征点
参数：
kd_root：图像特征的k-d树的树根
feat：目标特征点
k：近邻个数
nbrs：k个近邻特征点的指针数组，按到目标特征点的距离升序排列
      此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*nbrs)
max_nn_chks：搜索的最大次数，超过此值不再搜索
返回值：存储在nbrs中的近邻个数，返回-1表示失败
*/
extern int kdtree_bbf_knn( struct kd_node* kd_root, struct feature* feat,
						  int k, struct feature*** nbrs, int max_nn_chks );


extern int kdtree_bbf_spatial_knn( struct kd_node* kd_root,
								struct feature* feat, int k,
								struct feature*** nbrs, int max_nn_chks,
								CvRect rect, int model );


/*释放k-d树占用的存储空间
*/
extern void kdtree_release( struct kd_node* kd_root );
