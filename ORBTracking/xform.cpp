#include "stdafx.h"
#include "xform.h"
#include "imgfeatures.h"
#include "utils.h"

#include <cxcore.h>

#include <stdlib.h>
#include <time.h>

/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
/************************* Local Function Prototypes *************************/

//���ݸ�����ƥ�����ͣ���������������feat��ƥ���
static __inline struct feature* get_match(struct feature*, int);
//�ҵ����о���mtype����ƥ���������㣬�����ǵ�ָ��洢��matched������
static int get_matched_features(struct feature*, int, int, struct feature***);
//���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
static int calc_min_inliers(int, int, double, double);
//����n�Ľ׳˵���Ȼ����
static __inline double log_factorial(int);
//�Ӹ����������㼯�������ѡһ��RANSAC����(��һ��4�������������)
static struct feature** draw_ransac_sample(struct feature**, int, int);
//�������������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
static void extract_corresp_pts(struct feature**, int, int, CvPoint2D64f**, CvPoint2D64f**, int);
//���ڸ�����ģ�ͺʹ�������������������㼯�����ҳ�һ�¼�
static int find_consensus(struct feature**, int, int, CvMat*, ransac_err_fn, double, struct feature***, int);
//�ͷ���������Ĵ洢�ռ�
static __inline void release_mem(CvPoint2D64f*, CvPoint2D64f*, struct feature**);

/********************** Functions prototyped in xform.h **********************/


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
inliers�����������ָ�����飬ָ�����������յ��ڵ㼯�ϣ���Ϊ�գ���ʾû���������Ҫ���һ�¼�
��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*inliers)
n_in��������������ռ�������ڵ����Ŀ
downSampleTime : ����������
����ֵ��RANSAC�㷨������ı任������Ϊ�գ���ʾ������޷�������ɽ��ܾ���
*/
CvMat* ransac_xform(struct feature* features, int n, int mtype,
	ransac_xform_fn xform_fn, int m, double p_badxform,
	ransac_err_fn err_fn, double err_tol,
struct feature*** inliers, int* n_in, int downSampleTime)///
{
	//matched�����о���mtype����ƥ��������������飬Ҳ����������
	//sample��������������4�������������
	//consensus����ǰһ�¼���
	//consensus_max����ǰ���һ�¼�(����ǰ����ý����һ�¼�)
	struct feature** matched, ** sample, ** consensus, ** consensus_max = NULL;
	struct ransac_data* rdata;//ÿ���������feature_data���ransac����ָ��
	CvPoint2D64f* pts, *mpts;//ÿ��������Ӧ�������������飺��������������pts��ƥ�����������mpts
	CvMat* M = NULL;//��ǰ�任����
	//p����ǰ�������ģ�͵Ĵ�����ʣ���pС��p_badxformʱ����ֹͣ
	//in_frac���ڵ���Ŀռ��������Ŀ�İٷֱ�
	double p, in_frac = RANSAC_INLIER_FRAC_EST;
	//nm������������������о���mtype����ƥ�������������
	//in����ǰһ�¼���Ԫ�ظ���
	//in_min��һ�¼���Ԫ�ظ����������Сֵ����֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
	//in_max����ǰ����һ�¼�(���һ�¼�)��Ԫ�صĸ���
	//k����������������㵱ǰģ�͵Ĵ�������й�
	int i, nm, in, in_min, in_max = 0, k = 0;

	//�ҵ�����������features�����о���mtype����ƥ���������㣬�ŵ�matched����(������)�У�����ֵnm��matched�����Ԫ�ظ���
	nm = get_matched_features(features, n, mtype, &matched);
	//���ҵ��ľ���ƥ�������������С�ڼ���任������Ҫ����С������Ը���������
	if (nm < m)
	{   //������������Ը�������
		fprintf(stderr, "Warning: not enough matches to compute xform, %s" \
			" line %d\n", __FILE__, __LINE__);
		goto end;
	}

	/* initialize random number generator */
	srand(time(NULL));//��ʼ�������������

	//���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
	in_min = calc_min_inliers(nm, m, RANSAC_PROB_BAD_SUPP, p_badxform);
	//��ǰ�������ģ�͵Ĵ������,�ڵ���ռ����in_fracԽ�󣬴������ԽС����������kԽ�󣬴������ԽС
	p = pow(1.0 - pow(in_frac, m), k);
	i = 0;

	//��ǰ������ʴ������������������p_badxform����������
	while (p > p_badxform)
	{
		//��������matched�������ѡһ��RANSAC����(��һ��4�������������)���ŵ���������sample��
		sample = draw_ransac_sample(matched, nm, m);
		//�������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
		extract_corresp_pts(sample, m, mtype, &pts, &mpts, downSampleTime);
		//���ò����д���ĺ���xform_fn�����㽫m���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
		M = xform_fn(pts, mpts, m);//һ�㴫��lsq_homog()����
		if (!M)//�����ж�
			goto iteration_end;
		//���������㼯���任�����������������ǰһ�¼�consensus������һ�¼���Ԫ�ظ�����in
		in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus, downSampleTime);

		//����ǰһ�¼�������ʷ����һ�¼�������ǰһ�¼�Ϊ���ţ����������һ�¼�consensus_max
		if (in > in_max)
		{
			if (consensus_max)//��֮ǰ������ֵ���ͷ���ռ�
				free(consensus_max);
			consensus_max = consensus;//��������һ�¼�
			in_max = in;//��������һ�¼���Ԫ�ظ���
			in_frac = (double)in_max / nm;//����һ�¼���Ԫ�ظ���ռ�����ܸ����İٷֱ�
		}
		else//����ǰһ�¼�С����ʷ����һ�¼����ͷŵ�ǰһ�¼�
			free(consensus);
		cvReleaseMat(&M);

	iteration_end:
		release_mem(pts, mpts, sample);
		p = pow(1.0 - pow(in_frac, m), ++k);//���µ�ǰ�������
	}

	//��������һ�¼��������յı任����
	/* calculate final transform based on best consensus set */
	//������һ�¼���Ԫ�ظ���������ͱ�׼��������Ҫ��
	if (in_max >= in_min)
	{
		//������һ�¼��л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
		extract_corresp_pts(consensus_max, in_max, mtype, &pts, &mpts, downSampleTime);
		//���ò����д���ĺ���xform_fn�����㽫in_max���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
		M = xform_fn(pts, mpts, in_max);
		/***********������ٽ���һ�ε���**********/
		//���ݱ任����M��������matched�м����һ�¼�consensus������һ�¼���Ԫ�ظ�����in
		in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus, downSampleTime);
		cvReleaseMat(&M);
		release_mem(pts, mpts, consensus_max);
		//��һ�¼��л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
		extract_corresp_pts(consensus, in, mtype, &pts, &mpts, downSampleTime);
		//���ò����д���ĺ���xform_fn�����㽫in���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
		M = xform_fn(pts, mpts, in);
		if (inliers)
		{
			*inliers = consensus;//������һ�¼���ֵ�����������inliers�����ڵ㼯��
			consensus = NULL;
		}
		if (n_in)
			*n_in = in;//������һ�¼���Ԫ�ظ�����ֵ�����������n_in�����ڵ����
		release_mem(pts, mpts, consensus);
	}
	else if (consensus_max)
	{   //û�м��������Ҫ���һ�¼�
		if (inliers)
			*inliers = NULL;
		if (n_in)
			*n_in = 0;
		free(consensus_max);
	}

	//RANSAC�㷨�������ָ��������б����ĵ�������feature_data�������ر任����M
end:
	for (i = 0; i < nm; i++)
	{
		//���ú�feat_ransac_data����ȡmatched[i]�е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
		rdata = feat_ransac_data(matched[i]);
		//�ָ�feature_data��Ա����ǰ��ֵ
		matched[i]->feature_data = rdata->orig_feat_data;
		free(rdata);//�ͷ��ڴ�
	}
	free(matched);

	return M;//��������ı任����M
}

CvMat* dlt_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n)
{
	CvMat* H, *A, *VT, *D, h, v9;
	double _h[9];
	int i;

	if (n < 4)
		return NULL;

	/* set up matrices so we can unstack homography into h; Ah = 0 */
	A = cvCreateMat(2 * n, 9, CV_64FC1);
	cvZero(A);
	for (i = 0; i < n; i++)
	{
		cvmSet(A, 2 * i, 3, -pts[i].x);
		cvmSet(A, 2 * i, 4, -pts[i].y);
		cvmSet(A, 2 * i, 5, -1.0);
		cvmSet(A, 2 * i, 6, mpts[i].y * pts[i].x);
		cvmSet(A, 2 * i, 7, mpts[i].y * pts[i].y);
		cvmSet(A, 2 * i, 8, mpts[i].y);
		cvmSet(A, 2 * i + 1, 0, pts[i].x);
		cvmSet(A, 2 * i + 1, 1, pts[i].y);
		cvmSet(A, 2 * i + 1, 2, 1.0);
		cvmSet(A, 2 * i + 1, 6, -mpts[i].x * pts[i].x);
		cvmSet(A, 2 * i + 1, 7, -mpts[i].x * pts[i].y);
		cvmSet(A, 2 * i + 1, 8, -mpts[i].x);
	}
	D = cvCreateMat(9, 9, CV_64FC1);
	VT = cvCreateMat(9, 9, CV_64FC1);
	cvSVD(A, D, NULL, VT, CV_SVD_MODIFY_A + CV_SVD_V_T);
	v9 = cvMat(1, 9, CV_64FC1, NULL);
	cvGetRow(VT, &v9, 8);
	h = cvMat(1, 9, CV_64FC1, _h);
	cvCopy(&v9, &h, NULL);
	h = cvMat(3, 3, CV_64FC1, _h);
	H = cvCreateMat(3, 3, CV_64FC1);
	cvConvert(&h, H);

	cvReleaseMat(&A);
	cvReleaseMat(&D);
	cvReleaseMat(&VT);
	return H;
}


/* ����4������������С����ƽ�浥Ӧ�Ա任����
������
pts�����������
mpts����Ӧ�����飬pts[i]��mpts[i]һһ��Ӧ
n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ��һ����4
����ֵ��һ��3*3�ı任���󣬽�pts�е�ÿһ����ת��Ϊmpts�еĶ�Ӧ�㣬����ֵΪ�ձ�ʾʧ��
*/
CvMat* lsq_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n)
{
	CvMat* H, *A, *B, X;
	double x[9];//����x�е�Ԫ�ؾ��Ǳ任����H�е�ֵ
	int i;

	//�����Ը�������4
	if (n < 4)
	{
		fprintf(stderr, "Warning: too few points in lsq_homog(), %s line %d\n",
			__FILE__, __LINE__);
		return NULL;
	}

	//���任����Hչ����һ��8ά������X�У�ʹ��AX=B������ֻ��һ�ν����Է����鼴�����X��Ȼ���ٸ���X�ָ�H
	/* set up matrices so we can unstack homography into X; AX = B */
	A = cvCreateMat(2 * n, 8, CV_64FC1);//����2n*8�ľ���һ����8*8
	B = cvCreateMat(2 * n, 1, CV_64FC1);//����2n*1�ľ���һ����8*1
	X = cvMat(8, 1, CV_64FC1, x);//����8*1�ľ���ָ������Ϊx
	H = cvCreateMat(3, 3, CV_64FC1);//����3*3�ľ���
	cvZero(A);//��A����

	//������չ�����㣬��Ҫ����ԭ���ľ�����㷨�����·������A��B��ֵ������
	for (i = 0; i < n; i++)
	{
		cvmSet(A, i, 0, pts[i].x);//���þ���A��i��0�е�ֵΪpts[i].x
		cvmSet(A, i + n, 3, pts[i].x);
		cvmSet(A, i, 1, pts[i].y);
		cvmSet(A, i + n, 4, pts[i].y);
		cvmSet(A, i, 2, 1.0);
		cvmSet(A, i + n, 5, 1.0);
		cvmSet(A, i, 6, -pts[i].x * mpts[i].x);
		cvmSet(A, i, 7, -pts[i].y * mpts[i].x);
		cvmSet(A, i + n, 6, -pts[i].x * mpts[i].y);
		cvmSet(A, i + n, 7, -pts[i].y * mpts[i].y);
		cvmSet(B, i, 0, mpts[i].x);
		cvmSet(B, i + n, 0, mpts[i].y);
	}

	//����OpenCV�����������Է�����
	cvSolve(A, B, &X, CV_SVD);//��X��ʹ��AX=B
	x[8] = 1.0;//�任�����[3][3]λ�õ�ֵΪ�̶�ֵ1
	X = cvMat(3, 3, CV_64FC1, x);
	cvConvert(&X, H);//������ת��Ϊ����

	cvReleaseMat(&A);
	cvReleaseMat(&B);
	return H;
}


/*���ڸ����ĵ�Ӧ�Ծ���H�����������pt��H�任��ĵ�����ƥ���mpt֮������
���磺������x�����Ӧ��x'����Ӧ�Ծ���H�������x'��Hx֮��ľ����ƽ����d(x', Hx)^2
������
pt��һ����
mpt��pt�Ķ�Ӧƥ���
H����Ӧ�Ծ���
����ֵ��ת�����
*/
double homog_xfer_err(CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H)
{
	CvPoint2D64f xpt = persp_xform_pt(pt, H);//pt���任����H�任��ĵ�xpt,��H����x��Ӧ������

	return sqrt(dist_sq_2D(xpt, mpt));//���������ƽ��
}


/*�����pt��͸�ӱ任��ĵ㣬������һ��pt��͸�ӱ任����T������任��ĵ�
������(x,y)���任����M������[x',y',w']^T = M * [x,y,1]^T(^T��ʾת��)��
��任��ĵ���(u,v) = (x'/w', y'/w')
ע�⣺����任��͸�ӱ任������
������
pt��һ����ά��
T��͸�ӱ任����
����ֵ��pt��͸�ӱ任��ĵ�
*/
CvPoint2D64f persp_xform_pt(CvPoint2D64f pt, CvMat* T)
{
	//XY����pt��Ӧ��3*1��������UV��pt�任��ĵ��Ӧ��3*1������
	CvMat XY, UV;
	double xy[3] = { pt.x, pt.y, 1.0 }, uv[3] = { 0 };//��Ӧ������
	CvPoint2D64f rslt;//���

	//��ʼ������ͷ
	cvInitMatHeader(&XY, 3, 1, CV_64FC1, xy, CV_AUTOSTEP);
	cvInitMatHeader(&UV, 3, 1, CV_64FC1, uv, CV_AUTOSTEP);
	cvMatMul(T, &XY, &UV);//�������˷���T*XY,�������UV��
	rslt = cvPoint2D64f(uv[0] / uv[2], uv[1] / uv[2]);//�õ�ת����ĵ�

	return rslt;
}


/************************ ���غ�����ʵ��**************************************/
/************************ Local funciton definitions *************************/

/*���ݸ�����ƥ�����ͣ���������������feat��ƥ���
������
feat������������
mtype��ƥ�����ͣ���FEATURE_FWD_MATCH��FEATURE_BCK_MATCH��FEATURE_MDL_MATCH֮һ
����ֵ��feat��ƥ����ָ�룬��Ϊ�ձ�ʾmtype��������
*/
static __inline struct feature* get_match(struct feature* feat, int mtype)
{
	//FEATURE_MDL_MATCH������feature�ṹ�е�mdl_match���Ƕ�Ӧ��ƥ���
	if (mtype == FEATURE_MDL_MATCH)
		return feat->mdl_match;
	//FEATURE_BCK_MATCH������feature�ṹ�е�bck_match���Ƕ�Ӧ��ƥ���
	if (mtype == FEATURE_BCK_MATCH)
		return feat->bck_match;
	//FEATURE_FWD_MATCH������feature�ṹ�е�fwd_match���Ƕ�Ӧ��ƥ���
	if (mtype == FEATURE_FWD_MATCH)
		return feat->fwd_match;
	return NULL;
}


/*�ҵ����о���mtype����ƥ���������㣬�����ǵ�ָ��洢��matched�����У�
����ʼ��matched������ÿ���������feature_data��Ϊransac_data���͵�����ָ��
������
features������������
n�����������
mtype��ƥ������
matched���������������mtype����ƥ�����������ָ������
����ֵ��matched������������ĸ���
*/
static int get_matched_features(struct feature* features, int n, int mtype, struct feature*** matched)
{
	struct feature** _matched;//������飬����mtype����ƥ����������ָ������
	struct ransac_data* rdata;//ransac_data��������ָ��
	int i, m = 0;

	_matched = (struct feature**)calloc(n, sizeof(struct feature*));

	//�������������������
	for (i = 0; i < n; i++)
	{   //�ҵ�i���������mtype����ƥ��㣬������ȷ�ҵ���������������mtype���͵�ƥ��㣬�������_matched����
		if (get_match(features + i, mtype))
		{
			rdata = (struct ransac_data*)malloc(sizeof(struct ransac_data));//Ϊransac_data�ṹ����ռ�
			memset(rdata, 0, sizeof(struct ransac_data));//����
			rdata->orig_feat_data = features[i].feature_data;//�����i���������feature_data��֮ǰ��ֵ
			_matched[m] = features + i;//�ŵ�_matched����
			_matched[m]->feature_data = rdata;//��feature_data��ֵΪransac_data�������ݵ�ָ��
			m++;//_matched������Ԫ�ظ���
		}
	}
	*matched = _matched;//���������ֵ
	return m;//����ֵ��Ԫ�ظ���
}


/*���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
������
n���ƶ���ƥ���Եĸ���
m������ģ���������С��Ը���
p_badsupp�����ʣ�����ģ�ͱ�һ��ƥ����֧�ֵĸ���
p_badxform�����ʣ����ռ������ת�������Ǵ���ĵĸ���
����ֵ����֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
*/
static int calc_min_inliers(int n, int m, double p_badsupp, double p_badxform)
{
	//�������ģ�Chum, O. and Matas, J.  Matching with PROSAC -- Progressive Sample Consensus
	//�е�һ����ʽ���㣬������
	double pi, sum;
	int i, j;

	for (j = m + 1; j <= n; j++)
	{
		sum = 0;
		for (i = j; i <= n; i++)
		{
			pi = (i - m) * log(p_badsupp) + (n - i + m) * log(1.0 - p_badsupp) +
				log_factorial(n - m) - log_factorial(i - m) -
				log_factorial(n - i);
			sum += exp(pi);
		}
		if (sum < p_badxform)
			break;
	}
	return j;
}


//����n�Ľ׳˵���Ȼ����
static __inline double log_factorial(int n)
{
	double f = 0;
	int i;

	for (i = 1; i <= n; i++)
		f += log(i);

	return f;
}


/*�Ӹ����������㼯�������ѡһ��RANSAC����(��һ��4�������������)
������
features����Ϊ������������������
n��features��Ԫ�ظ���
m�����������ĳߴ磬������4(������Ҫ4����������任����)
����ֵ��һ��ָ�����飬��Ԫ��ָ��ѡΪ�����������㣬��ѡΪ�������������feature_data���sampled����Ϊ1
*/
static struct feature** draw_ransac_sample(struct feature** features, int n, int m)
{
	struct feature** sample, *feat;//sample����ѡΪ�����ĵ������
	struct ransac_data* rdata;
	int i, x;

	//�������������feature_data���sampledֵ����ʼ��Ϊ0����δ��ѡΪ������
	for (i = 0; i < n; i++)
	{
		//���ú�feat_ransac_data����ȡ�����е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
		rdata = feat_ransac_data(features[i]);
		rdata->sampled = 0;//sampledֵ��Ϊ0
	}

	sample = (struct feature**)calloc(m, sizeof(struct feature*));//Ϊ�����������ռ�

	//�����ȡm����������Ϊһ������������ָ�뱣����sample������
	for (i = 0; i < m; i++)
	{
		do
		{
			x = rand() % n;//����±�
			feat = features[x];
			rdata = feat_ransac_data(feat);//���feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
		} while (rdata->sampled);//����ȡ���������sampledֵΪ1������ѡȡ������ֹͣ��������Ϊ�����е�һ����
		sample[i] = feat;//����sample����
		rdata->sampled = 1;//�õ��feature_data��Ա��sampled��ֵ��Ϊ1
	}

	return sample;//�������ѡȡ������
}


/*�������������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
������
features�����������飬�������г�ȡ��������ƥ��㣬�����������������㶼Ӧ����mtype���͵�ƥ���
n��feantures�����������
mtype��ƥ�����ͣ�����FEATURE_MDL_MATCH����Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
pts�����������������������features�л�ȡ�Ķ�ά��������
mpts�����������������������features�Ķ�Ӧƥ����л�ȡ�Ķ�ά��������
*/
static void extract_corresp_pts(struct feature** features, int n, int mtype,
	CvPoint2D64f** pts, CvPoint2D64f** mpts, int downSampleTime)
{
	struct feature* match;//ÿ���������Ӧ��ƥ���
	CvPoint2D64f* _pts, *_mpts;
	int i;

	_pts = (CvPoint2D64f*)calloc(n, sizeof(CvPoint2D64f));//���������������
	_mpts = (CvPoint2D64f*)calloc(n, sizeof(CvPoint2D64f));//��Ӧƥ������������

	//ƥ��������FEATURE_MDL_MATCH��ƥ����������mdl_pt��
	if (mtype == FEATURE_MDL_MATCH)
	for (i = 0; i < n; i++)
	{
		//���ݸ�����ƥ�����ͣ����������������ƥ���
		match = get_match(features[i], mtype);
		if (!match)
			fatal_error("feature does not have match of type %d, %s line %d",
			mtype, __FILE__, __LINE__);

		_pts[i].x = pow(2, downSampleTime)*(features[i]->img_pt.x);//�����������
		_pts[i].y = pow(2, downSampleTime)*(features[i]->img_pt.y);//�����������
		_mpts[i].x = pow(2, downSampleTime)*(match->mdl_pt.x);//��Ӧƥ��������
		_mpts[i].y = pow(2, downSampleTime)*(match->mdl_pt.y);//��Ӧƥ��������
		//_pts[i] = features[i]->img_pt;//�����������
		//_mpts[i] = match->mdl_pt;//��Ӧƥ��������

	}
	//ƥ�����Ͳ���FEATURE_MDL_MATCH��ƥ����������img_pt��
	else
	for (i = 0; i < n; i++)
	{
		//���ݸ�����ƥ�����ͣ����������������ƥ���
		match = get_match(features[i], mtype);
		if (!match)
			fatal_error("feature does not have match of type %d, %s line %d",
			mtype, __FILE__, __LINE__);

		_pts[i].x = pow(2, downSampleTime)*(features[i]->img_pt.x);//�����������
		_pts[i].y = pow(2, downSampleTime)*(features[i]->img_pt.y);//�����������
		_mpts[i].x = pow(2, downSampleTime)*(match->img_pt.x);//��Ӧƥ��������
		_mpts[i].y = pow(2, downSampleTime)*(match->img_pt.y);//��Ӧƥ��������


		//_pts[i] = features[i]->img_pt;//�����������
		//_mpts[i] = match->img_pt;//��Ӧƥ��������

	}

	*pts = _pts;
	*mpts = _mpts;
}



/*���ڸ�����ģ�ͺʹ�������������������㼯�����ҳ�һ�¼�
������
features�������㼯�ϣ����е������㶼����mtype���͵�ƥ���
n��������ĸ���
mtype��ƥ�����ͣ�����FEATURE_MDL_MATCH����Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
M��������ģ�ͣ���һ���任����
err_fn������������������ڸ����ı任���󣬼����ƶ���ƥ����֮��ı任���
err_tol���ݴ�ȣ���������err_fn�ķ���ֵ��С��err_tol�ı�����һ�¼�
consensus�����������һ�¼�����һ�¼��������㹹�ɵ�����
����ֵ��һ�¼���������ĸ���
*/
static int find_consensus(struct feature** features, int n, int mtype,
	CvMat* M, ransac_err_fn err_fn, double err_tol,
struct feature*** consensus, int downSampleTime)
{
	struct feature** _consensus;//һ�¼�
	struct feature* match;//ÿ���������Ӧ��ƥ���
	CvPoint2D64f pt, mpt;//pt:��ǰ����������꣬mpt����ǰ�������ƥ��������
	double err;//�任���
	int i, in = 0;

	_consensus = (struct feature**)calloc(n, sizeof(struct feature*));//��һ�¼�����ռ�

	//ƥ��������FEATURE_MDL_MATCH��ƥ����������mdl_pt��
	if (mtype == FEATURE_MDL_MATCH)
	for (i = 0; i < n; i++)
	{
		//���ݸ�����ƥ�����ͣ����������������ƥ���
		match = get_match(features[i], mtype);
		if (!match)
			fatal_error("feature does not have match of type %d, %s line %d",
			mtype, __FILE__, __LINE__);
		//pt = features[i]->img_pt;//�����������
		// mpt = match->mdl_pt;//��Ӧƥ��������

		pt.x = pow(2, downSampleTime)*(features[i]->img_pt.x);//�����������
		pt.y = pow(2, downSampleTime)*(features[i]->img_pt.y);//�����������
		mpt.x = pow(2, downSampleTime)*(match->mdl_pt.x);//��Ӧƥ��������
		mpt.y = pow(2, downSampleTime)*(match->mdl_pt.y);//��Ӧƥ��������

		err = err_fn(pt, mpt, M);//����"pt��M�任��ĵ�"��mpt֮��ľ����ƽ�������任���
		if (err <= err_tol)//���任���С���ݴ�ȣ��������һ�¼�
			_consensus[in++] = features[i];
	}
	//ƥ�����Ͳ���FEATURE_MDL_MATCH��ƥ����������img_pt��
	else
	for (i = 0; i < n; i++)
	{
		//���ݸ�����ƥ�����ͣ����������������ƥ���
		match = get_match(features[i], mtype);
		if (!match)
			fatal_error("feature does not have match of type %d, %s line %d",
			mtype, __FILE__, __LINE__);
		//pt = features[i]->img_pt;//�����������
		//mpt = match->img_pt;//��Ӧƥ��������

		pt.x = pow(2, downSampleTime)*(features[i]->img_pt.x);//�����������
		pt.y = pow(2, downSampleTime)*(features[i]->img_pt.y);//�����������
		mpt.x = pow(2, downSampleTime)*(match->img_pt.x);//��Ӧƥ��������
		mpt.y = pow(2, downSampleTime)*(match->img_pt.y);//��Ӧƥ��������

		err = err_fn(pt, mpt, M);//����"pt��M�任��ĵ�"��mpt֮��ľ����ƽ�������任���
		if (err <= err_tol)//���任���С���ݴ�ȣ��������һ�¼�
			_consensus[in++] = features[i];
	}
	*consensus = _consensus;
	return in;//����һ�¼���Ԫ�ظ���
}


/*�ͷ���������Ĵ洢�ռ�
*/
static __inline void release_mem(CvPoint2D64f* pts1, CvPoint2D64f* pts2,
struct feature** features)
{
	free(pts1);
	free(pts2);
	if (features)
		free(features);
}
