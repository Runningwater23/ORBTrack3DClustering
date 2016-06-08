/*
此文件中包括的函数声明有：
1、RANSAC算法特征匹配筛选
2、其他一些与RANSAC算法相关函数
*/

#pragma once

#include <cxcore.h>


struct feature;

//RANSAC算法中用到的结构
//在RANSAC算法过程中，此类型数据会被赋值给feature结构的feature_data成员
struct ransac_data
{
    void* orig_feat_data; //保存此特征点的feature_data域的以前的值
    int sampled; //标识位，值为1标识此特征点是否被选为样本
};

//一些宏定义

/*RANSAC算法的容错度
对于匹配点对<pt,mpt>，以及变换矩阵H，
如果pt经H变换后的点和mpt之间的距离的平方小于RANSAC_ERR_TOL，则可将其加入当前一致集
*/
#define RANSAC_ERR_TOL 3

//内点数目占样本总数目的百分比的最小值
#define RANSAC_INLIER_FRAC_EST 0.25

//一个匹配点对支持错误模型的概率（不知道是干什么用的）
#define RANSAC_PROB_BAD_SUPP 0.10

//定义了一个带参数的函数宏，用来提取参数feat中的feature_data成员并转换为ransac_data格式的指针
#define feat_ransac_data( feat ) ( (struct ransac_data*) (feat)->feature_data )






//降采样次数
#define DOWN_SAMPLE_TIME 0






/*定义了一个函数指针类型ransac_xform_fn，其返回值是CvMat*类型，有三个参数
之后可以用ransac_xform_fn来定义函数指针
此类型的函数指针被用在ransac_form()函数的参数中
此类型的函数会根据匹配点对集合计算出一个变换矩阵作为返回值
参数：
pts：点的集合
mpts：点的集合，pts[i]与mpts[i]是互相对应的匹配点
n：pts和mpts数组中点的个数，pts和mpts中点的个数必须相同
返回值：一个变换矩阵，将pts中的每一个点转换为mpts中的对应点，返回值为空表示失败
*/
typedef CvMat* (*ransac_xform_fn)( CvPoint2D64f* pts, CvPoint2D64f* mpts,int n );


/*定义了一个函数指针类型ransac_err_fn，其返回值是double类型，有三个参数
之后可以用ransac_err_fn来定义函数指针
此类型的函数指针被用在ransac_form()函数的参数中
此类型的函数会根据匹配点对(pt,mpt)和变换矩阵M计算出一个double类型的错误度量值作为返回值
此错误度量值用来评判"点mpt"和"点pt经M矩阵变换后的点"之间是否相一致
参数：
pt：一个点
mpt：点pt的对应匹配点
M：变换矩阵
返回值："点mpt"和"点pt经M矩阵变换后的点"之间的错误度量值
*/
typedef double (*ransac_err_fn)( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* M );


/***************************** Function Prototypes ***************************/

/*利用RANSAC算法进行特征点筛选，计算出最佳匹配的变换矩阵
参数：
features：特征点数组，只有当mtype类型的匹配点存在时才被用来进行单应性计算
n：特征点个数
mtype：决定使用每个特征点的哪个匹配域进行变换矩阵的计算，应该是FEATURE_MDL_MATCH，
    FEATURE_BCK_MATCH，FEATURE_MDL_MATCH中的一个。若是FEATURE_MDL_MATCH，
    对应的匹配点对坐标是每个特征点的img_pt域和其匹配点的mdl_pt域，
    否则，对应的匹配点对是每个特征点的img_pt域和其匹配点的img_pt域。
xform_fn：函数指针，指向根据输入的点对进行变换矩阵计算的函数，一般传入lsq_homog()函数
m：在函数xform_fn中计算变换矩阵需要的最小特征点对个数
p_badxform：允许的错误概率，即允许RANSAC算法计算出的变换矩阵错误的概率，当前计算出的模型的错误概率小于p_badxform时迭代停止
err_fn：函数指针，对于给定的变换矩阵，计算推定的匹配点对之间的变换误差，一般传入homog_xfer_err()函数
err_tol：容错度，对于给定的变换矩阵，在此范围内的点对被认为是内点
inliers：输出参数，指针数组，指向计算出的最终的内点集合，若为空，表示没计算出符合要求的一致集。
        此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*inliers)
n_in：输出参数，最终计算出的内点的数目
downSampleTime: 降采样次数
返回值：RANSAC算法计算出的变换矩阵，若为空，表示出错或无法计算出可接受矩阵
*/
extern CvMat* ransac_xform( struct feature* features, int n, int mtype,
						   ransac_xform_fn xform_fn, int m,
						   double p_badxform, ransac_err_fn err_fn,
						   double err_tol, struct feature*** inliers,
						   int* n_in, int downSampleTime );


/*运用线性变换，进行点匹配计算平面单应性
参数：
pts：点的集合
mpts：点的集合，pts[i]与mpts[i]是互相对应的匹配点
n：pts和mpts数组中点的个数，pts和mpts中点的个数必须相同，并且点对个数至少为4
返回值：变换矩阵，可将pts中的点变换为mpts中的点，若点个数少于4则返回空
*/
extern CvMat* dlt_homog( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n );


/*根据4对坐标点计算最小二乘平面单应性变换矩阵
参数：
pts：点的集合
mpts：点的集合，pts[i]与mpts[i]是互相对应的匹配点
n：pts和mpts数组中点的个数，pts和mpts中点的个数必须相同，并且点对个数至少为4
返回值：变换矩阵，可将pts中的点变换为mpts中的点，若点个数少于4则返回空
*/
extern CvMat* lsq_homog( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n );


/*对于给定的单应性矩阵H，计算输入点pt精H变换后的点与其匹配点mpt之间的误差
例如：给定点x，其对应点x'，单应性矩阵H，则计算x'与Hx之间的距离的平方，d(x', Hx)^2
参数：
pt：一个点
mpt：pt的对应匹配点
H：单应性矩阵
返回值：转换误差
*/
extern double homog_xfer_err( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H );


/*计算点pt经透视变换后的点，即给定一点pt和透视变换矩阵T，计算变换后的点
给定点(x,y)，变换矩阵M，计算[x',y',w']^T = M * [x,y,1]^T(^T表示转置)，
则变换后的点是(u,v) = (x'/w', y'/w')
注意：仿射变换是透视变换的特例
参数：
pt：一个二维点
T：透视变换矩阵
返回值：pt经透视变换后的点
*/
extern CvPoint2D64f persp_xform_pt( CvPoint2D64f pt, CvMat* T );


