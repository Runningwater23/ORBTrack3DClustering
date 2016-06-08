/*
  ���ļ��ж����˴洢������Ľṹ��feature���Լ���������ԭ�͵�������
1��������ĵ��뵼��
2�����������
*/


#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

#include "cxcore.h"

/*����������ͣ�
FEATURE_OXFD��ʾ��ţ���ѧVGG�ṩ��Դ���е��������ʽ��
FEATURE_LOWE��ʾ��David.Lowe�ṩ��Դ���е��������ʽ
*/
/** FEATURE_OXFD <BR> FEATURE_LOWE */
enum feature_type
{
	FEATURE_OXFD,
	FEATURE_LOWE,
};

/*������ƥ�����ͣ�
FEATURE_FWD_MATCH������feature�ṹ�е�fwd_match���Ƕ�Ӧ��ƥ���
FEATURE_BCK_MATCH������feature�ṹ�е�bck_match���Ƕ�Ӧ��ƥ���
FEATURE_MDL_MATCH������feature�ṹ�е�mdl_match���Ƕ�Ӧ��ƥ���
*/
/** FEATURE_FWD_MATCH <BR> FEATURE_BCK_MATCH <BR> FEATURE_MDL_MATCH */
enum feature_match_type
{
	FEATURE_FWD_MATCH,
	FEATURE_BCK_MATCH,
	FEATURE_MDL_MATCH,
};

/*���������������ɫ*/
/* colors in which to display different feature types */
#define FEATURE_OXFD_COLOR CV_RGB(255,255,0)
#define FEATURE_LOWE_COLOR CV_RGB(255,0,255)

/*������������ӳ��ȣ���Ϊ128*/
/** max feature descriptor length */
#define FEATURE_MAX_D 128

/*������ṹ��
�˽ṹ��ɴ洢2�����͵������㣺
FEATURE_OXFD��ʾ��ţ���ѧVGG�ṩ��Դ���е��������ʽ��
FEATURE_LOWE��ʾ��David.Lowe�ṩ��Դ���е��������ʽ��
�����OXFD���͵������㣬�ṹ���е�a,b,c��Ա��������������Χ�ķ�������(��Բ�Ĳ���)��������
�����LOWE���͵������㣬�ṹ���е�scl��ori��Ա������������Ĵ�С�ͷ���
fwd_match��bck_match��mdl_matchһ��ͬʱֻ��һ�������ã�����ָ�����������Ӧ��ƥ���
*/
/**
Structure to represent an affine invariant image feature.  The fields
x, y, a, b, c represent the affine region around the feature:
a(x-u)(x-u) + 2b(x-u)(y-v) + c(y-v)(y-v) = 1
*/
struct feature
{
    double x;                      /**< x coord */ //�������x����
    double y;                      /**< y coord */ //�������y����
    double a;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double b;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double c;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double scl;                    /**< scale of a Lowe-style feature *///LOWE������ĳ߶�
    double ori;                    /**< orientation of a Lowe-style feature */ //LOWE������ķ���
    int d;                         /**< descriptor length */ //���������ӵĳ��ȣ���ά����һ����128
    double descr[FEATURE_MAX_D];   /**< descriptor */ //128ά�����������ӣ���һ��double����
    int type;                      /**< feature type, OXFD or LOWE */ //����������
	int category;                  /**< all-purpose feature category */
    struct feature* fwd_match;     /**< matching feature from forward image */   //ָ�����������Ӧ��ƥ���
    struct feature* bck_match;     /**< matching feature from backmward image */ //ָ�����������Ӧ��ƥ���
    struct feature* mdl_match;     /**< matching feature from model */           //ָ�����������Ӧ��ƥ���
    CvPoint2D64f img_pt;           /**< location in image */ //�����������,����(x,y)
    CvPoint2D64f mdl_pt;           /**< location in model */ //��ƥ��������mdl_matchʱ�õ�
    void* feature_data;            /**< user-definable data */ //�û����������:
                                                               //��SIFT��ֵ�����У���detection_data�ṹ��ָ��
                                                               //��k-d�������У���bbf_data�ṹ��ָ��
                                                               //��RANSAC�㷨�У���ransac_data�ṹ��ָ��
};


/*���ļ��ж���ͼ������
�ļ��е��������ʽ������FEATURE_OXFD��FEATURE_LOWE��ʽ
������
filename���ļ���
type������������
feat�������洢�������feature�����ָ��
����ֵ����������������
*/
extern int import_features( char* filename, int type, struct feature** feat );


/*����feature���鵽�ļ�
������
filename���ļ���
feat����������
n�����������
����ֵ��0���ɹ���1��ʧ��
*/
extern int export_features( char* filename, struct feature* feat, int n );


/*��ͼƬ�ϻ���������
������
img��ͼ��
feat������������
n�����������
*/
extern void draw_features( IplImage* img, struct feature* feat, int n );


/*�����������������Ӽ��ŷ�Ͼ����ƽ��
������
f1:��һ��������
f2:�ڶ���������
����ֵ��ŷ�Ͼ����ƽ��
*/
extern double descr_dist_sq( struct feature* f1, struct feature* f2 );
