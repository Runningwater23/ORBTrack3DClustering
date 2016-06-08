/*
���ļ��а����ĺ��������У�
1��RANSAC�㷨����ƥ��ɸѡ
2������һЩ��RANSAC�㷨��غ���
*/

#pragma once

#include <cxcore.h>


struct feature;

//RANSAC�㷨���õ��Ľṹ
//��RANSAC�㷨�����У����������ݻᱻ��ֵ��feature�ṹ��feature_data��Ա
struct ransac_data
{
    void* orig_feat_data; //������������feature_data�����ǰ��ֵ
    int sampled; //��ʶλ��ֵΪ1��ʶ���������Ƿ�ѡΪ����
};

//һЩ�궨��

/*RANSAC�㷨���ݴ��
����ƥ����<pt,mpt>���Լ��任����H��
���pt��H�任��ĵ��mpt֮��ľ����ƽ��С��RANSAC_ERR_TOL����ɽ�����뵱ǰһ�¼�
*/
#define RANSAC_ERR_TOL 3

//�ڵ���Ŀռ��������Ŀ�İٷֱȵ���Сֵ
#define RANSAC_INLIER_FRAC_EST 0.25

//һ��ƥ����֧�ִ���ģ�͵ĸ��ʣ���֪���Ǹ�ʲô�õģ�
#define RANSAC_PROB_BAD_SUPP 0.10

//������һ���������ĺ����꣬������ȡ����feat�е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
#define feat_ransac_data( feat ) ( (struct ransac_data*) (feat)->feature_data )






//����������
#define DOWN_SAMPLE_TIME 0






/*������һ������ָ������ransac_xform_fn���䷵��ֵ��CvMat*���ͣ�����������
֮�������ransac_xform_fn�����庯��ָ��
�����͵ĺ���ָ�뱻����ransac_form()�����Ĳ�����
�����͵ĺ��������ƥ���Լ��ϼ����һ���任������Ϊ����ֵ
������
pts����ļ���
mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ
����ֵ��һ���任���󣬽�pts�е�ÿһ����ת��Ϊmpts�еĶ�Ӧ�㣬����ֵΪ�ձ�ʾʧ��
*/
typedef CvMat* (*ransac_xform_fn)( CvPoint2D64f* pts, CvPoint2D64f* mpts,int n );


/*������һ������ָ������ransac_err_fn���䷵��ֵ��double���ͣ�����������
֮�������ransac_err_fn�����庯��ָ��
�����͵ĺ���ָ�뱻����ransac_form()�����Ĳ�����
�����͵ĺ��������ƥ����(pt,mpt)�ͱ任����M�����һ��double���͵Ĵ������ֵ��Ϊ����ֵ
�˴������ֵ��������"��mpt"��"��pt��M����任��ĵ�"֮���Ƿ���һ��
������
pt��һ����
mpt����pt�Ķ�Ӧƥ���
M���任����
����ֵ��"��mpt"��"��pt��M����任��ĵ�"֮��Ĵ������ֵ
*/
typedef double (*ransac_err_fn)( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* M );


/***************************** Function Prototypes ***************************/

/*����RANSAC�㷨����������ɸѡ����������ƥ��ı任����
������
features�����������飬ֻ�е�mtype���͵�ƥ������ʱ�ű��������е�Ӧ�Լ���
n�����������
mtype������ʹ��ÿ����������ĸ�ƥ������б任����ļ��㣬Ӧ����FEATURE_MDL_MATCH��
    FEATURE_BCK_MATCH��FEATURE_MDL_MATCH�е�һ��������FEATURE_MDL_MATCH��
    ��Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
    ���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
xform_fn������ָ�룬ָ���������ĵ�Խ��б任�������ĺ�����һ�㴫��lsq_homog()����
m���ں���xform_fn�м���任������Ҫ����С������Ը���
p_badxform������Ĵ�����ʣ�������RANSAC�㷨������ı任�������ĸ��ʣ���ǰ�������ģ�͵Ĵ������С��p_badxformʱ����ֹͣ
err_fn������ָ�룬���ڸ����ı任���󣬼����ƶ���ƥ����֮��ı任��һ�㴫��homog_xfer_err()����
err_tol���ݴ�ȣ����ڸ����ı任�����ڴ˷�Χ�ڵĵ�Ա���Ϊ���ڵ�
inliers�����������ָ�����飬ָ�����������յ��ڵ㼯�ϣ���Ϊ�գ���ʾû���������Ҫ���һ�¼���
        ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*inliers)
n_in��������������ռ�������ڵ����Ŀ
downSampleTime: ����������
����ֵ��RANSAC�㷨������ı任������Ϊ�գ���ʾ������޷�������ɽ��ܾ���
*/
extern CvMat* ransac_xform( struct feature* features, int n, int mtype,
						   ransac_xform_fn xform_fn, int m,
						   double p_badxform, ransac_err_fn err_fn,
						   double err_tol, struct feature*** inliers,
						   int* n_in, int downSampleTime );


/*�������Ա任�����е�ƥ�����ƽ�浥Ӧ��
������
pts����ļ���
mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ�����ҵ�Ը�������Ϊ4
����ֵ���任���󣬿ɽ�pts�еĵ�任Ϊmpts�еĵ㣬�����������4�򷵻ؿ�
*/
extern CvMat* dlt_homog( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n );


/*����4������������С����ƽ�浥Ӧ�Ա任����
������
pts����ļ���
mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ�����ҵ�Ը�������Ϊ4
����ֵ���任���󣬿ɽ�pts�еĵ�任Ϊmpts�еĵ㣬�����������4�򷵻ؿ�
*/
extern CvMat* lsq_homog( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n );


/*���ڸ����ĵ�Ӧ�Ծ���H�����������pt��H�任��ĵ�����ƥ���mpt֮������
���磺������x�����Ӧ��x'����Ӧ�Ծ���H�������x'��Hx֮��ľ����ƽ����d(x', Hx)^2
������
pt��һ����
mpt��pt�Ķ�Ӧƥ���
H����Ӧ�Ծ���
����ֵ��ת�����
*/
extern double homog_xfer_err( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H );


/*�����pt��͸�ӱ任��ĵ㣬������һ��pt��͸�ӱ任����T������任��ĵ�
������(x,y)���任����M������[x',y',w']^T = M * [x,y,1]^T(^T��ʾת��)��
��任��ĵ���(u,v) = (x'/w', y'/w')
ע�⣺����任��͸�ӱ任������
������
pt��һ����ά��
T��͸�ӱ任����
����ֵ��pt��͸�ӱ任��ĵ�
*/
extern CvPoint2D64f persp_xform_pt( CvPoint2D64f pt, CvMat* T );


