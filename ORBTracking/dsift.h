/*
  ���ļ��а���DSIFT�������⺯�����������Լ�һЩʵ��DSIFT�㷨��һЩĬ�ϲ����Ķ���
*/

#pragma once

#include "cxcore.h"

/******************************** Structures *********************************/

//��ֵ�������õ��Ľṹ
//��SIFT������ȡ�����У����������ݻᱻ��ֵ��feature�ṹ��feature_data��Ա
/** holds feature data relevant to detection */
struct detection_data
{
    int r;      //���������ڵ���
    int c;      //���������ڵ���
    int octv;   //��˹��ֽ������У����������ڵ���
    int intvl;  //��˹��ֽ������У����������ڵ����еĲ�
    double subintvl;  //�������ڲ㷽��(�ҷ���,intvl����)�ϵ�������ƫ����
    double scl_octv;  //���������ڵ���ĳ߶�
};

struct feature;

/******************************* һЩĬ�ϲ��� *****************************/

//��˹������ÿ���ڵĲ���
#define SIFT_INTVLS 3

//��0��ĳ�ʼ�߶ȣ�����0���˹ģ����ʹ�õĲ���
#define SIFT_SIGMA 1.6

//�Աȶ���ֵ����Թ�һ�����ͼ������ȥ�����ȶ�����
#define SIFT_CONTR_THR 0.04

//�����ʱ�ֵ����ֵ������ȥ����Ե����
#define SIFT_CURV_THR 10

//�Ƿ�ͼ��Ŵ�Ϊ֮ǰ������
#define SIFT_IMG_DBL 1

//����ͼ��ĳ߶�Ϊ0.5
#define SIFT_INIT_SIGMA 0.5

//�߽�����ؿ�ȣ��������н����Ա߽����еļ�ֵ�㣬��ֻ���߽��������Ƿ���ڼ�ֵ��
#define SIFT_IMG_BORDER 5

//ͨ����ֵ���м�ֵ�㾫ȷ��λʱ������ֵ���������ؼ�����������
#define SIFT_MAX_INTERP_STEPS 5

//�����㷽��ֵ�����У��ݶȷ���ֱ��ͼ������(bin)�ĸ���
#define SIFT_ORI_HIST_BINS 36

//�����㷽��ֵ�����У���������İ뾶Ϊ��3 * 1.5 * ��
#define SIFT_ORI_SIG_FCTR 1.5

//�����㷽��ֵ�����У���������İ뾶Ϊ��3 * 1.5 * ��
#define SIFT_ORI_RADIUS 3.0 * SIFT_ORI_SIG_FCTR

//�����㷽��ֵ�����У��ݶȷ���ֱ��ͼ��ƽ��������������ݶ�ֱ��ͼ��Ҫ���и�˹ƽ��
#define SIFT_ORI_SMOOTH_PASSES 2

//�����㷽��ֵ�����У��ݶȷ�ֵ�ﵽ���ֵ��80%�����Ϊ����������
#define SIFT_ORI_PEAK_RATIO 0.8

//�������������ӹ����У����㷽��ֱ��ͼʱ���������㸽������Ϊd*d������ÿ����������һ��ֱ��ͼ��SIFT_DESCR_WIDTH��d��Ĭ��ֵ
#define SIFT_DESCR_WIDTH 4

//�������������ӹ����У�ÿ������ֱ��ͼ��bin����
#define SIFT_DESCR_HIST_BINS 8

//�������������ӹ����У���������Χ��d*d�������У�ÿ������Ŀ��Ϊm*�Ҹ����أ�SIFT_DESCR_SCL_FCTR��m��Ĭ��ֵ����Ϊ������ĳ߶�
#define SIFT_DESCR_SCL_FCTR 3.0

//�������������ӹ����У�����������������Ԫ�ص���ֵ(���ֵ����������Թ�һ���������������)����������ֵ��Ԫ�ر�ǿ�и�ֵΪ����ֵ
#define SIFT_DESCR_MAG_THR 0.2

//�������������ӹ����У��������͵����������ӱ�Ϊ����ʱ���Ե�ϵ��
#define SIFT_INT_DESCR_FCTR 512.0

//������һ���������ĺ����꣬������ȡ����f�е�feature_data��Ա��ת��Ϊdetection_data��ʽ��ָ��
#define feat_detection_data(f) ( (struct detection_data*)(f->feature_data) )


/*ʹ��Ĭ�ϲ�����ͼ������ȡSIFT������
������
img��ͼ��ָ��
feat�������洢�������feature�����ָ��
      ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*feat)
����ֵ����ȡ�������������������-1������ȡʧ��
*/
extern int sift_features( IplImage* img, struct feature** feat );
extern int dsift_features( IplImage* img, struct feature** feat, int windowHeight, int windowWidth );


/*ʹ���û�ָ���Ĳ�����ͼ������ȡSIFT������
������
img������ͼ��
feat���洢������������ָ��
      ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*feat)
intvls��ÿ��Ĳ���
sigma����ʼ��˹ƽ��������
contr_thr���Աȶ���ֵ����Թ�һ�����ͼ������ȥ�����ȶ�����
curv_thr��ȥ����Ե����������������ֵ
img_dbl���Ƿ�ͼ��Ŵ�Ϊ֮ǰ������
descr_width���������������У����㷽��ֱ��ͼʱ���������㸽������Ϊdescr_width*descr_width������ÿ����������һ��ֱ��ͼ
descr_hist_bins���������������У�ÿ��ֱ��ͼ��bin�ĸ���
����ֵ����ȡ�������������������-1������ȡʧ��
*/
extern int _sift_features( IplImage* img, struct feature** feat, int intvls,
						  double sigma, double contr_thr, int curv_thr,
						  int img_dbl, int descr_width, int descr_hist_bins );

extern int _dsift_features( IplImage* img, struct feature** feat, int intvls,
						  double sigma, double contr_thr, int curv_thr,
						  int img_dbl, int descr_width, int descr_hist_bins, int windowHeight, int windowWidth);
double calc_s_value(struct feature* feat, int nn);

