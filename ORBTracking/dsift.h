/*
  此文件中包括DSIFT特征点检测函数的声明，以及一些实现DSIFT算法中一些默认参数的定义
*/

#pragma once

#include "cxcore.h"

/******************************** Structures *********************************/

//极值点检测中用到的结构
//在SIFT特征提取过程中，此类型数据会被赋值给feature结构的feature_data成员
/** holds feature data relevant to detection */
struct detection_data
{
    int r;      //特征点所在的行
    int c;      //特征点所在的列
    int octv;   //高斯差分金字塔中，特征点所在的组
    int intvl;  //高斯差分金字塔中，特征点所在的组中的层
    double subintvl;  //特征点在层方向(σ方向,intvl方向)上的亚像素偏移量
    double scl_octv;  //特征点所在的组的尺度
};

struct feature;

/******************************* 一些默认参数 *****************************/

//高斯金字塔每组内的层数
#define SIFT_INTVLS 3

//第0层的初始尺度，即第0层高斯模糊所使用的参数
#define SIFT_SIGMA 1.6

//对比度阈值，针对归一化后的图像，用来去除不稳定特征
#define SIFT_CONTR_THR 0.04

//主曲率比值的阈值，用来去除边缘特征
#define SIFT_CURV_THR 10

//是否将图像放大为之前的两倍
#define SIFT_IMG_DBL 1

//输入图像的尺度为0.5
#define SIFT_INIT_SIGMA 0.5

//边界的像素宽度，检测过程中将忽略边界线中的极值点，即只检测边界线以内是否存在极值点
#define SIFT_IMG_BORDER 5

//通过插值进行极值点精确定位时，最大差值次数，即关键点修正次数
#define SIFT_MAX_INTERP_STEPS 5

//特征点方向赋值过程中，梯度方向直方图中柱子(bin)的个数
#define SIFT_ORI_HIST_BINS 36

//特征点方向赋值过程中，搜索邻域的半径为：3 * 1.5 * σ
#define SIFT_ORI_SIG_FCTR 1.5

//特征点方向赋值过程中，搜索邻域的半径为：3 * 1.5 * σ
#define SIFT_ORI_RADIUS 3.0 * SIFT_ORI_SIG_FCTR

//特征点方向赋值过程中，梯度方向直方图的平滑次数，计算出梯度直方图后还要进行高斯平滑
#define SIFT_ORI_SMOOTH_PASSES 2

//特征点方向赋值过程中，梯度幅值达到最大值的80%则分裂为两个特征点
#define SIFT_ORI_PEAK_RATIO 0.8

//计算特征描述子过程中，计算方向直方图时，将特征点附近划分为d*d个区域，每个区域生成一个直方图，SIFT_DESCR_WIDTH即d的默认值
#define SIFT_DESCR_WIDTH 4

//计算特征描述子过程中，每个方向直方图的bin个数
#define SIFT_DESCR_HIST_BINS 8

//计算特征描述子过程中，特征点周围的d*d个区域中，每个区域的宽度为m*σ个像素，SIFT_DESCR_SCL_FCTR即m的默认值，σ为特征点的尺度
#define SIFT_DESCR_SCL_FCTR 3.0

//计算特征描述子过程中，特征描述子向量中元素的阈值(最大值，并且是针对归一化后的特征描述子)，超过此阈值的元素被强行赋值为此阈值
#define SIFT_DESCR_MAG_THR 0.2

//计算特征描述子过程中，将浮点型的特征描述子变为整型时乘以的系数
#define SIFT_INT_DESCR_FCTR 512.0

//定义了一个带参数的函数宏，用来提取参数f中的feature_data成员并转换为detection_data格式的指针
#define feat_detection_data(f) ( (struct detection_data*)(f->feature_data) )


/*使用默认参数在图像中提取SIFT特征点
参数：
img：图像指针
feat：用来存储特征点的feature数组的指针
      此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*feat)
返回值：提取的特征点个数，若返回-1表明提取失败
*/
extern int sift_features( IplImage* img, struct feature** feat );
extern int dsift_features( IplImage* img, struct feature** feat, int windowHeight, int windowWidth );


/*使用用户指定的参数在图像中提取SIFT特征点
参数：
img：输入图像
feat：存储特征点的数组的指针
      此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*feat)
intvls：每组的层数
sigma：初始高斯平滑参数σ
contr_thr：对比度阈值，针对归一化后的图像，用来去除不稳定特征
curv_thr：去除边缘的特征的主曲率阈值
img_dbl：是否将图像放大为之前的两倍
descr_width：特征描述过程中，计算方向直方图时，将特征点附近划分为descr_width*descr_width个区域，每个区域生成一个直方图
descr_hist_bins：特征描述过程中，每个直方图中bin的个数
返回值：提取的特征点个数，若返回-1表明提取失败
*/
extern int _sift_features( IplImage* img, struct feature** feat, int intvls,
						  double sigma, double contr_thr, int curv_thr,
						  int img_dbl, int descr_width, int descr_hist_bins );

extern int _dsift_features( IplImage* img, struct feature** feat, int intvls,
						  double sigma, double contr_thr, int curv_thr,
						  int img_dbl, int descr_width, int descr_hist_bins, int windowHeight, int windowWidth);
double calc_s_value(struct feature* feat, int nn);

