
/*
���ļ�����Ҫ
����SIFT���������ʵ��
*/
#include "stdafx.h"
#include "dsift.h"
#include "imgfeatures.h"
#include "utils.h"

#include "Queue.h"

#include <cxcore.h>
#include <cv.h>
/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
/************************* Local Function Prototypes *************************/

//��ԭͼת��Ϊ32λ�Ҷ�ͼ����һ����Ȼ�����һ�θ�˹ƽ���������ݲ���img_dbl�����Ƿ�ͼ��ߴ�Ŵ�Ϊԭͼ��2��
static IplImage* create_init_img(IplImage*, int, double);
//������ͼ��ת��Ϊ32λ�Ҷ�ͼ,�����й�һ��
static IplImage* convert_to_gray32(IplImage*);
//�����������������˹������
static IplImage*** build_gauss_pyr(IplImage*, int, int, double);
//������ͼ�����²����������ķ�֮һ��С��ͼ��(ÿ��ά���ϼ���)��ʹ������ڲ�ֵ����
static IplImage* downsample(IplImage*);
//ͨ���Ը�˹��������ÿ��������ͼ�������������˹��ֽ�����
static IplImage*** build_dog_pyr(IplImage***, int, int);
//�ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
static CvSeq* scale_space_extrema(IplImage***, int, int, double, int, CvMemStorage*);
//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ3*3*3�����ڵĵ�Ƚ��������˵��Ƿ�ֵ��(����ֵ��С����)
static int is_extremum(IplImage***, int, int, int, int);




//��ӻ������ڣ��ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
static CvSeq* scale_space_discrete_extrema(IplImage***, int, int, double, int, CvMemStorage*, int, int);
//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ�������������ڵĵ�Ƚ��������˵��Ƿ񼫴�ֵ
static int is_intvls_max(IplImage***, int, int, int, int);
//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ�������������ڵĵ�Ƚ��������˵��Ƿ�Сֵ
static int is_intvs_min(IplImage***, int, int, int, int);






//ͨ�������ؼ���ֵ���м�ֵ�㾫ȷ��λ(������ֵ������)����ȥ���ͶԱȶȵļ�ֵ�㣬������������������feature�ṹ����
static struct feature* interp_extremum(IplImage***, int, int, int, int, int, double);
//����һ�μ�ֵ���ֵ������x��y���ҷ���(�㷽��)�ϵ�������ƫ����(����)
static void interp_step(IplImage***, int, int, int, int, double*, double*, double*);
//��DoG�������м���ĳ���x����y�����Լ��߶ȷ����ϵ�ƫ����
static CvMat* deriv_3D(IplImage***, int, int, int, int);
//��DoG�������м���ĳ���3*3��ɭ����
static CvMat* hessian_3D(IplImage***, int, int, int, int);
//���㱻��ֵ��ĶԱȶȣ�D + 0.5 * dD^T * X
static double interp_contr(IplImage***, int, int, int, int, double, double, double);
//Ϊһ��feature�ṹ����ռ䲢��ʼ��
static struct feature* new_feature(void);
//ȥ����Ե��Ӧ����ͨ�����������ʱ�ֵ�ж�ĳ���Ƿ��Ե��
static int is_too_edge_like(IplImage*, int, int, int);
//����������������ÿ��������ĳ߶�
static void calc_feature_scales(CvSeq*, double, int);
//��������������ÿ����������������(�������˽�ͼ��Ŵ�Ϊԭͼ��2��ʱ������������֮�����)
static void adjust_for_img_dbl(CvSeq*);
//����ÿ����������ݶ�ֱ��ͼ���ҳ�����������һ���������в�ֹһ�������򣬽����Ϊ����������
static void calc_feature_oris(CvSeq*, IplImage***);
//����ָ�����ص���ݶȷ���ֱ��ͼ�����ش��ֱ��ͼ������
static double* ori_hist(IplImage*, int, int, int, int, double);
//����ָ������ݶȵķ�ֵmagnitude�ͷ���orientation
static int calc_grad_mag_ori(IplImage*, int, int, double*, double*);
//���ݶȷ���ֱ��ͼ���и�˹ƽ�����ֲ���û�з��䲻���Զ������������㲻�ȶ�������
static void smooth_ori_hist(double*, int);
//�����ݶ�ֱ��ͼ����������ݶȷ�ֵ��������ֱ��ͼ�����bin��ֵ
static double dominant_ori(double*, int);
//����ǰ�������ֱ��ͼ��ĳ��bin��ֵ���ڸ�������ֵ����������һ�������㲢��ӵ�����������ĩβ
static void add_good_ori_features(CvSeq*, double*, int, double, struct feature*);
//�������feature�ṹ����������������ؿ�¡���ɵ��������ָ��
static struct feature* clone_feature(struct feature*);
//����������������ÿ�����������������������
static void compute_descriptors(CvSeq*, IplImage***, int, int);
//���������㸽������ķ���ֱ��ͼ����ֱ��ͼ�ڼ���������������Ҫ�õ�������ֵ��һ��d*d*n����ά����
static double*** descr_hist(IplImage*, int, int, double, double, int, int);
static void interp_hist_entry(double***, double, double, double, double, int, int);
//��ĳ������ķ���ֱ��ͼת��Ϊ���������������������������ӹ�һ����������Ԫ��ת��Ϊ���ͣ�����ָ����������
static void hist_to_descr(double***, int, int, struct feature*);
//��һ������������������ӣ���������������������ÿ��Ԫ�س������������ӵ�ģ
static void normalize_descr(struct feature*);
//�ȽϺ������������㰴�߶ȵĽ������У���������������CvSeqSort��
static int feature_cmp(void*, void*, void*);
//�ͷż������������ӹ������õ��ķ���ֱ��ͼ���ڴ�ռ�
static void release_descr_hist(double****, int);
//�ͷŽ�����ͼ����Ĵ洢�ռ�
static void release_pyr(IplImage****, int, int);


/*********************** Functions prototyped in sift.h **********************/

/*ʹ��Ĭ�ϲ�����ͼ������ȡSIFT������
������
img��ͼ��ָ��
feat�������洢�������feature�����ָ��
��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*feat)
����ֵ����ȡ�������������������-1������ȡʧ��
*/
int sift_features(IplImage* img, struct feature** feat)///
{
	//����_sift_features()����������������
	return _sift_features(img, feat, SIFT_INTVLS, SIFT_SIGMA, SIFT_CONTR_THR,
		SIFT_CURV_THR, SIFT_IMG_DBL, SIFT_DESCR_WIDTH,
		SIFT_DESCR_HIST_BINS);
}

int dsift_features(IplImage* img, struct feature** feat, int windowHeight, int windowWidth)
{
	//����_sift_features()����������������
	return _dsift_features(img, feat, SIFT_INTVLS, SIFT_SIGMA, SIFT_CONTR_THR,
		SIFT_CURV_THR, SIFT_IMG_DBL, SIFT_DESCR_WIDTH,
		SIFT_DESCR_HIST_BINS, windowHeight, windowWidth);
}



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
int _sift_features(IplImage* img, struct feature** feat, int intvls,
	double sigma, double contr_thr, int curv_thr,
	int img_dbl, int descr_width, int descr_hist_bins)///
{
	IplImage* init_img;//ԭͼ����ʼ�����ͼ�񣬹�һ����32λ�Ҷ�ͼ
	IplImage*** gauss_pyr, *** dog_pyr;//����ָ�룬��˹������ͼ���飬DoG������ͼ����
	CvMemStorage* storage;//�洢��
	CvSeq* features;//�洢����������У������д�ŵ���struct feature���͵�ָ��
	int octvs, i, n = 0;

	//����������
	/* check arguments */
	if (!img)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	if (!feat)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	/* build scale space pyramid; smallest dimension of top level is ~4 pixels */

	//��������һ�������������߶ȿռ䣬��������˹���(DoG)������dog_pyr
	//��ԭͼת��Ϊ32λ�Ҷ�ͼ����һ����Ȼ�����һ�θ�˹ƽ���������ݲ���img_dbl�����Ƿ�ͼ��ߴ�Ŵ�Ϊԭͼ��2��
	init_img = create_init_img(img, img_dbl, sigma);
	//�����˹������������octvs
	octvs = log(MIN(init_img->width, init_img->height)) / log(2) - 2;
	//Ϊ�˱�֤�����ԣ���ÿһ��Ķ�������ø�˹ģ������3��ͼ�����Ը�˹������ÿ����intvls+3�㣬DOG������ÿ����intvls+2��
	//������˹������gauss_pyr����һ��octvs*(intvls+3)��ͼ������
	gauss_pyr = build_gauss_pyr(init_img, octvs, intvls, sigma);
	//������˹���(DoG)������dog_pyr����һ��octvs*(intvls+2)��ͼ������
	dog_pyr = build_dog_pyr(gauss_pyr, octvs, intvls);

	//������������������ڳ߶ȿռ��м�⼫ֵ�㣬�����о�ȷ��λ��ɸѡ
	//����Ĭ�ϴ�С���ڴ�洢��
	storage = cvCreateMemStorage(0);
	//�ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
	features = scale_space_extrema(dog_pyr, octvs, intvls, contr_thr, curv_thr, storage);
	//��������������features��ÿ��������ĳ߶�
	calc_feature_scales(features, sigma, intvls);
	//�������˽�ͼ��Ŵ�Ϊԭͼ��2��
	if (img_dbl)//��������������ÿ����������������(�������˽�ͼ��Ŵ�Ϊԭͼ��2��ʱ������������֮�����)
		adjust_for_img_dbl(features);

	//�����������������������㷽��ֵ����ɴ˲����ÿ����������������Ϣ��λ�á��߶ȡ�����
	//����ÿ����������ݶ�ֱ��ͼ���ҳ�����������һ���������в�ֹһ�������򣬽����Ϊ����������
	calc_feature_oris(features, gauss_pyr);

	//���������ģ���������������������
	//����������������ÿ�����������������������
	compute_descriptors(features, gauss_pyr, descr_width, descr_hist_bins);

	/* sort features by decreasing scale and move from CvSeq to array */
	//��������߶ȵĽ�������������Ԫ�ص�˳��feature_cmp���Զ���ıȽϺ���
	cvSeqSort(features, (CvCmpFunc)feature_cmp, NULL);

	//��CvSeq���͵�����������featuresת��Ϊͨ�õ�struct feature���͵�����feat
	n = features->total;//���������
	*feat = (struct feature*)calloc(n, sizeof(struct feature));//����ؼ�
	//������features�е�Ԫ�ؿ���������feat�У���������ָ���feat
	*feat = (struct feature*)cvCvtSeqToArray(features, *feat, CV_WHOLE_SEQ);

	//�ͷ�����������feat�������������feature_data��Ա����Ϊ�˳�Ա�е������ڼ������������û����
	for (i = 0; i < n; i++)
	{
		free((*feat)[i].feature_data);
		(*feat)[i].feature_data = NULL;
	}

	//�ͷŸ�����ʱ���ݵĴ洢�ռ�
	cvReleaseMemStorage(&storage);//�ͷ��ڴ�洢��
	cvReleaseImage(&init_img);//�ͷų�ʼ�����ͼ��
	release_pyr(&gauss_pyr, octvs, intvls + 3);//�ͷŸ�˹������ͼ����
	release_pyr(&dog_pyr, octvs, intvls + 2);//�ͷŸ�˹��ֽ�����ͼ����

	return n;//���ؼ�⵽��������ĸ���
}


int _dsift_features(IplImage* img, struct feature** feat, int intvls,
	double sigma, double contr_thr, int curv_thr,
	int img_dbl, int descr_width, int descr_hist_bins, int windowHeight, int windowWidth)
{
	IplImage* init_img;//ԭͼ����ʼ�����ͼ�񣬹�һ����32λ�Ҷ�ͼ
	IplImage*** gauss_pyr, *** dog_pyr;//����ָ�룬��˹������ͼ���飬DoG������ͼ����
	CvMemStorage* storage;//�洢��
	CvSeq* features;//�洢����������У������д�ŵ���struct feature���͵�ָ��
	int octvs, i, n = 0;

	//����������
	/* check arguments */
	if (!img)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	if (!feat)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	/* build scale space pyramid; smallest dimension of top level is ~4 pixels */

	//��������һ�������������߶ȿռ䣬��������˹���(DoG)������dog_pyr
	//��ԭͼת��Ϊ32λ�Ҷ�ͼ����һ����Ȼ�����һ�θ�˹ƽ���������ݲ���img_dbl�����Ƿ�ͼ��ߴ�Ŵ�Ϊԭͼ��2��
	init_img = create_init_img(img, img_dbl, sigma);
	//�����˹������������octvs
	octvs = log(MIN(init_img->width, init_img->height)) / log(2) - 2;
	//Ϊ�˱�֤�����ԣ���ÿһ��Ķ�������ø�˹ģ������3��ͼ�����Ը�˹������ÿ����intvls+3�㣬DOG������ÿ����intvls+2��
	//������˹������gauss_pyr����һ��octvs*(intvls+3)��ͼ������
	gauss_pyr = build_gauss_pyr(init_img, octvs, intvls, sigma);
	//������˹���(DoG)������dog_pyr����һ��octvs*(intvls+2)��ͼ������
	dog_pyr = build_dog_pyr(gauss_pyr, octvs, intvls);

	//������������������ڳ߶ȿռ��м�⼫ֵ�㣬�����о�ȷ��λ��ɸѡ
	//����Ĭ�ϴ�С���ڴ�洢��
	storage = cvCreateMemStorage(0);
	//�ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
	features = scale_space_discrete_extrema(dog_pyr, octvs, intvls, contr_thr, curv_thr, storage, windowHeight, windowWidth);
	//��������������features��ÿ��������ĳ߶�
	calc_feature_scales(features, sigma, intvls);
	//�������˽�ͼ��Ŵ�Ϊԭͼ��2��
	if (img_dbl)//��������������ÿ����������������(�������˽�ͼ��Ŵ�Ϊԭͼ��2��ʱ������������֮�����)
		adjust_for_img_dbl(features);

	//�����������������������㷽��ֵ����ɴ˲����ÿ����������������Ϣ��λ�á��߶ȡ�����
	//����ÿ����������ݶ�ֱ��ͼ���ҳ�����������һ���������в�ֹһ�������򣬽����Ϊ����������
	calc_feature_oris(features, gauss_pyr);

	//���������ģ���������������������
	//����������������ÿ�����������������������
	compute_descriptors(features, gauss_pyr, descr_width, descr_hist_bins);

	/* sort features by decreasing scale and move from CvSeq to array */
	//��������߶ȵĽ�������������Ԫ�ص�˳��feature_cmp���Զ���ıȽϺ���
	cvSeqSort(features, (CvCmpFunc)feature_cmp, NULL);

	//��CvSeq���͵�����������featuresת��Ϊͨ�õ�struct feature���͵�����feat
	n = features->total;//���������
	*feat = (struct feature*)calloc(n, sizeof(struct feature));//����ؼ�
	//������features�е�Ԫ�ؿ���������feat�У���������ָ���feat
	*feat = (struct feature*)cvCvtSeqToArray(features, *feat, CV_WHOLE_SEQ);

	//�ͷ�����������feat�������������feature_data��Ա����Ϊ�˳�Ա�е������ڼ������������û����
	for (i = 0; i < n; i++)
	{
		free((*feat)[i].feature_data);
		(*feat)[i].feature_data = NULL;
	}

	//�ͷŸ�����ʱ���ݵĴ洢�ռ�
	cvReleaseMemStorage(&storage);//�ͷ��ڴ�洢��
	cvReleaseImage(&init_img);//�ͷų�ʼ�����ͼ��
	release_pyr(&gauss_pyr, octvs, intvls + 3);//�ͷŸ�˹������ͼ����
	release_pyr(&dog_pyr, octvs, intvls + 2);//�ͷŸ�˹��ֽ�����ͼ����

	return n;//���ؼ�⵽��������ĸ���
}


/******************************* ���غ�����ʵ�� ********************************/
/************************ Functions prototyped here **************************/

/*��ԭͼת��Ϊ32λ�Ҷ�ͼ����һ����Ȼ�����һ�θ�˹ƽ���������ݲ���img_dbl�����Ƿ�ͼ��ߴ�Ŵ�Ϊԭͼ��2��
������
img�������ԭͼ��
img_dbl���Ƿ�ͼ��Ŵ�Ϊ֮ǰ������
sigma����ʼ��˹ƽ��������
����ֵ����ʼ����ɵ�ͼ��
*/
static IplImage* create_init_img(IplImage* img, int img_dbl, double sigma)///
{
	IplImage* gray, *dbl;
	float sig_diff;

	//���ú�����������ͼ��ת��Ϊ32λ�Ҷ�ͼ������һ��
	gray = convert_to_gray32(img);

	//�������˽�ͼ��Ŵ�Ϊԭͼ��2��
	if (img_dbl)
	{
		//��ͼ�񳤿���չһ��ʱ�������˵�-1�㣬�ò�߶�Ϊ��
		sig_diff = sqrt(sigma * sigma - SIFT_INIT_SIGMA * SIFT_INIT_SIGMA * 4);
		dbl = cvCreateImage(cvSize(img->width * 2, img->height * 2), IPL_DEPTH_32F, 1);//�����Ŵ�ͼ��
		cvResize(gray, dbl, CV_INTER_CUBIC);//�Ŵ�ԭͼ�ĳߴ�
		//��˹ƽ������˹����x,y�����ϵı�׼���sig_diff
		cvSmooth(dbl, dbl, CV_GAUSSIAN, 0, 0, sig_diff, sig_diff);
		cvReleaseImage(&gray);
		return dbl;
	}
	else//���÷Ŵ�Ϊԭͼ��2��
	{
		//�����0��ĳ߶�
		sig_diff = sqrt(sigma * sigma - SIFT_INIT_SIGMA * SIFT_INIT_SIGMA);
		//��˹ƽ������˹����x,y�����ϵı�׼���sig_diff
		cvSmooth(gray, gray, CV_GAUSSIAN, 0, 0, sig_diff, sig_diff);
		return gray;
	}
}


/*������ͼ��ת��Ϊ32λ�Ҷ�ͼ,�����й�һ��
������
img������ͼ��3ͨ��8λ��ɫͼ(BGR)��8λ�Ҷ�ͼ
����ֵ��32λ�Ҷ�ͼ
*/
static IplImage* convert_to_gray32(IplImage* img)
{
	IplImage* gray8, *gray32;

	gray32 = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);//����32λ��ͨ��ͼ��

	//���Ƚ�ԭͼת��Ϊ8λ��ͨ��ͼ��
	if (img->nChannels == 1)//��ԭͼ������ǵ�ͨ����ֱ�ӿ�¡ԭͼ
		gray8 = (IplImage*)cvClone(img);
	else//��ԭͼ��3ͨ��ͼ��
	{
		gray8 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);//����8λ��ͨ��ͼ��
		cvCvtColor(img, gray8, CV_BGR2GRAY);//��ԭͼת��Ϊ8Ϊ��ͨ��ͼ��
	}

	//Ȼ��8Ϊ��ͨ��ͼ��gray8ת��Ϊ32λ��ͨ��ͼ�񣬲����й�һ������(����255)
	cvConvertScale(gray8, gray32, 1.0 / 255.0, 0);

	cvReleaseImage(&gray8);//�ͷ���ʱͼ��

	return gray32;//����32λ��ͨ��ͼ��
}


/*�����������������˹������
������
base������ͼ����Ϊ��˹�������Ļ�ͼ��
octvs����˹������������
intvls��ÿ��Ĳ���
sigma����ʼ�߶�
����ֵ����˹����������һ��octvs*(intvls+3)��ͼ������
*/
static IplImage*** build_gauss_pyr(IplImage* base, int octvs,
	int intvls, double sigma)
{
	IplImage*** gauss_pyr;
	//Ϊ�˱�֤�����ԣ���ÿһ��Ķ�������ø�˹ģ������3��ͼ�����Ը�˹������ÿ����intvls+3�㣬DOG������ÿ����intvls+2��
	double* sig = (double*)calloc(intvls + 3, sizeof(double));//ÿ���sigma��������
	double sig_total, sig_prev, k;
	int i, o;

	//Ϊ��˹������gauss_pyr����ռ䣬��octvs��Ԫ�أ�ÿ��Ԫ����һ��ͼ�����ָ��
	gauss_pyr = (IplImage***)calloc(octvs, sizeof(IplImage**));
	//Ϊ��i��ͼ��gauss_pyr[i]����ռ䣬��intvls+3��Ԫ�أ�ÿ��Ԫ����һ��ͼ��ָ��
	for (i = 0; i < octvs; i++)
		gauss_pyr[i] = (IplImage**)calloc(intvls + 3, sizeof(IplImage*));

	/*	precompute Gaussian sigmas using the following formula:
	sigma_{total}^2 = sigma_{i}^2 + sigma_{i-1}^2   */
	//����ÿ�θ�˹ģ����sigma����
	sig[0] = sigma;//��ʼ�߶�
	k = pow(2.0, 1.0 / intvls);
	for (i = 1; i < intvls + 3; i++)
	{
		sig_prev = pow(k, i - 1) * sigma;//i-1��ĳ߶�
		sig_total = sig_prev * k;//i��ĳ߶�
		sig[i] = sqrt(sig_total * sig_total - sig_prev * sig_prev);//��������Ϊʲô��
	}

	//����������ɸ�˹������
	for (o = 0; o < octvs; o++)//������
	for (i = 0; i < intvls + 3; i++)//������
	{
		if (o == 0 && i == 0)//��0�飬��0�㣬����ԭͼ��
			gauss_pyr[o][i] = cvCloneImage(base);
		else if (i == 0)//�µ�һ����ײ�ͼ��������һ�����һ��ͼ���²����õ�
			gauss_pyr[o][i] = downsample(gauss_pyr[o - 1][intvls]);
		else//����һ��ͼ����и�˹ƽ���õ���ǰ��ͼ��
		{   //����ͼ��
			gauss_pyr[o][i] = cvCreateImage(cvGetSize(gauss_pyr[o][i - 1]), IPL_DEPTH_32F, 1);
			//����һ��ͼ��gauss_pyr[o][i-1]���в���Ϊsig[i]�ĸ�˹ƽ�����õ���ǰ��ͼ��gauss_pyr[o][i]
			cvSmooth(gauss_pyr[o][i - 1], gauss_pyr[o][i], CV_GAUSSIAN, 0, 0, sig[i], sig[i]);
		}
	}

	free(sig);//�ͷ�sigma��������

	return gauss_pyr;//���ظ�˹������
}


/*������ͼ�����²����������ķ�֮һ��С��ͼ��(ÿ��ά���ϼ���)��ʹ������ڲ�ֵ����
������
img������ͼ��
����ֵ���²������ͼ��
*/
static IplImage* downsample(IplImage* img)
{
	//�²���ͼ��
	IplImage* smaller = cvCreateImage(cvSize(img->width / 2, img->height / 2), img->depth, img->nChannels);
	cvResize(img, smaller, CV_INTER_NN);//�ߴ�任

	return smaller;
}


/*ͨ���Ը�˹��������ÿ��������ͼ�������������˹��ֽ�����
������
gauss_pyr����˹������
octvs������
intvls��ÿ��Ĳ���
����ֵ����˹��ֽ���������һ��octvs*(intvls+2)��ͼ������
*/
static IplImage*** build_dog_pyr(IplImage*** gauss_pyr, int octvs, int intvls)
{
	IplImage*** dog_pyr;
	int i, o;

	//Ϊ��˹��ֽ���������ռ䣬��octvs��Ԫ�أ�ÿ��Ԫ����һ��ͼ�����ָ��
	dog_pyr = (IplImage***)calloc(octvs, sizeof(IplImage**));
	//Ϊ��i��ͼ��dog_pyr[i]����ռ䣬��(intvls+2)��Ԫ�أ�ÿ��Ԫ����һ��ͼ��ָ��
	for (i = 0; i < octvs; i++)
		dog_pyr[i] = (IplImage**)calloc(intvls + 2, sizeof(IplImage*));

	//������������ͼ��
	for (o = 0; o < octvs; o++)//������
	for (i = 0; i < intvls + 2; i++)//������
	{   //����DoG�������ĵ�o���i��Ĳ��ͼ��
		dog_pyr[o][i] = cvCreateImage(cvGetSize(gauss_pyr[o][i]), IPL_DEPTH_32F, 1);
		//����˹�������ĵ�o���i+1��ͼ��͵�i��ͼ��������õ�DoG�������ĵ�o���i��ͼ��
		cvSub(gauss_pyr[o][i + 1], gauss_pyr[o][i], dog_pyr[o][i], NULL);
	}

	return dog_pyr;//���ظ�˹��ֽ�����
}


/*�ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
������
dog_pyr����˹��ֽ�����
octvs����˹��ֽ�����������
intvls��ÿ��Ĳ���
contr_thr���Աȶ���ֵ����Թ�һ�����ͼ������ȥ�����ȶ�����
cur_thr�������ʱ�ֵ����ֵ������ȥ����Ե����
storage���洢��
����ֵ�����ؼ�⵽�������������
*/
static CvSeq* scale_space_extrema(IplImage*** dog_pyr, int octvs, int intvls,
	double contr_thr, int curv_thr, CvMemStorage* storage)
{
	CvSeq* features;//����������
	double prelim_contr_thr = 0.5 * contr_thr / intvls;//���صĶԱȶ���ֵ
	struct feature* feat;
	struct detection_data* ddata;
	int o, i, r, c;

	//�ڴ洢��storage�ϴ����洢��ֵ������У����д洢feature�ṹ���͵�����
	features = cvCreateSeq(0, sizeof(CvSeq), sizeof(struct feature), storage);

	/*������˹��ֽ���������⼫ֵ��*/
	//SIFT_IMG_BORDERָ���߽��ȣ�ֻ���߽������ڵļ�ֵ��
	for (o = 0; o < octvs; o++)//��o��
	for (i = 1; i <= intvls; i++)//��i��
	for (r = SIFT_IMG_BORDER; r < dog_pyr[o][0]->height - SIFT_IMG_BORDER; r++)//��r��
	for (c = SIFT_IMG_BORDER; c < dog_pyr[o][0]->width - SIFT_IMG_BORDER; c++)//��c��
		//���г����ĶԱȶȼ�飬ֻ�е���һ���������ֵ���ڶԱȶ���ֵprelim_contr_thrʱ�ż����������ص��Ƿ�����Ǽ�ֵ
		//���ú���pixval32f��ȡͼ��dog_pyr[o][i]�ĵ�r�е�c�еĵ������ֵ��Ȼ�����ABS���������ֵ
	if (ABS(pixval32f(dog_pyr[o][i], r, c)) > prelim_contr_thr)
		//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ3*3*3�����ڵĵ�Ƚ��������˵��Ƿ�ֵ��(����ֵ��С����)
	if (is_extremum(dog_pyr, o, i, r, c))//���Ǽ�ֵ��
	{
		//���ڼ�ֵ��ļ��������ɢ�ռ��н��еģ����Լ�⵽�ļ�ֵ�㲢��һ�������������ϵļ�ֵ��
		//��Ϊ�����ļ�ֵ�����λ����������֮�䣬������ɢ�ռ���ֻ�ܾ�ȷ������㾫����
		//ͨ�������ؼ���ֵ���м�ֵ�㾫ȷ��λ(������ֵ������)����ȥ���ͶԱȶȵļ�ֵ�㣬������������������feature�ṹ����
		feat = interp_extremum(dog_pyr, o, i, r, c, intvls, contr_thr);
		//����ֵ�ǿգ������˵��ѱ��ɹ�����
		if (feat)
		{
			//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
			ddata = feat_detection_data(feat);
			//ȥ����Ե��Ӧ����ͨ�����������ʱ�ֵ�ж�ĳ���Ƿ��Ե��,����ֵΪ0��ʾ���Ǳ�Ե�㣬����������
			if (!is_too_edge_like(dog_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, curv_thr))
			{
				cvSeqPush(features, feat);//������������featuresĩβ�����¼�⵽��������feat
			}
			else
				free(ddata);
			free(feat);
		}
	}

	return features;//��������������
}


/*ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ3*3*3�����ڵĵ�Ƚ��������˵��Ƿ�ֵ��(����ֵ��С����)
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
����ֵ����ָ�������ص��Ǽ�ֵ��(����ֵ��Сֵ)������1�����򷵻�0
*/
static int is_extremum(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
	//���ú���pixval32f��ȡͼ��dog_pyr[octv][intvl]�ĵ�r�е�c�еĵ������ֵ
	float val = pixval32f(dog_pyr[octv][intvl], r, c);
	int i, j, k;
	int w = 1;

	//����Ƿ����ֵ
	if (val > 0)
	{
		for (i = -1; i <= 1; i++)//��
		for (j = -w; j <= w; j++)//��
		for (k = -w; k <= w; k++)//��
		if (val < pixval32f(dog_pyr[octv][intvl + i], r + j, c + k))
			return 0;
	}
	//����Ƿ���Сֵ
	else
	{
		for (i = -1; i <= 1; i++)//��
		for (j = -w; j <= w; j++)//��
		for (k = -w; k <= w; k++)//��
		if (val > pixval32f(dog_pyr[octv][intvl + i], r + j, c + k))
			return 0;
	}

	return 1;
}
/*ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ�������������ڵĵ�Ƚ��������˵��Ƿ񼫴�ֵ
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
����ֵ����ָ�������ص��Ǽ�ֵ��(����ֵ��Сֵ)������1�����򷵻�0
*/
static int is_intvls_max(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
	//���ú���pixval32f��ȡͼ��dog_pyr[octv][intvl]�ĵ�r�е�c�еĵ������ֵ
	float val = pixval32f(dog_pyr[octv][intvl], r, c);
	int j, k;

	//����Ƿ����ֵ
	if (val > 0)
	{
		for (j = -1; j <= 1; j++)//��
		for (k = -1; k <= 1; k++)//��
		if (val < pixval32f(dog_pyr[octv][intvl], r + j, c + k))
			return 0;
	}
	return 1;
}
//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ�������������ڵĵ�Ƚ��������˵��Ƿ�Сֵ
static int is_intvls_min(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
	//���ú���pixval32f��ȡͼ��dog_pyr[octv][intvl]�ĵ�r�е�c�еĵ������ֵ
	float val = pixval32f(dog_pyr[octv][intvl], r, c);
	int j, k;

	//����Ƿ���Сֵ
	if (val <= 0)
	{
		for (j = -1; j <= 1; j++)//��
		for (k = -1; k <= 1; k++)//��
		if (val > pixval32f(dog_pyr[octv][intvl], r + j, c + k))
			return 0;
	}
	return 1;
}


/*��ӻ������ڣ��ڳ߶ȿռ��м�⼫ֵ�㣬ͨ����ֵ��ȷ��λ��ȥ���ͶԱȶȵĵ㣬ȥ����Ե�㣬���ؼ�⵽������������
������
dog_pyr����˹��ֽ�����
octvs����˹��ֽ�����������
intvls��ÿ��Ĳ���
contr_thr���Աȶ���ֵ����Թ�һ�����ͼ������ȥ�����ȶ�����
cur_thr�������ʱ�ֵ����ֵ������ȥ����Ե����
storage���洢��
����ֵ�����ؼ�⵽�������������*/
static CvSeq* scale_space_discrete_extrema(IplImage*** dog_pyr, int octvs, int intvls,
	double contr_thr, int curv_thr, CvMemStorage* storage, int windowHeight, int windowWidth)
{
	CvSeq* features;//����������
	double prelim_contr_thr = 0.5 * contr_thr / intvls;//���صĶԱȶ���ֵ
	struct feature* feat;
	struct detection_data* ddata;
	int o, i, r, c, winr, winc;

	//�ڴ洢��storage�ϴ����洢��ֵ������У����д洢feature�ṹ���͵�����
	features = cvCreateSeq(0, sizeof(CvSeq), sizeof(struct feature), storage);

	/*������˹��ֽ���������⼫ֵ��*/
	//SIFT_IMG_BORDERָ���߽��ȣ�ֻ���߽������ڵļ�ֵ��


	for (o = 0; o < 1; o++)//��o��
	for (i = 1; i <= intvls; i++)//��i��
	for (r = SIFT_IMG_BORDER; r < dog_pyr[o][0]->height - SIFT_IMG_BORDER; r++)//��r��
	for (c = SIFT_IMG_BORDER; c < dog_pyr[o][0]->width - SIFT_IMG_BORDER; c++)//��c��
		//���г����ĶԱȶȼ�飬ֻ�е���һ���������ֵ���ڶԱȶ���ֵprelim_contr_thrʱ�ż����������ص��Ƿ�����Ǽ�ֵ
		//���ú���pixval32f��ȡͼ��dog_pyr[o][i]�ĵ�r�е�c�еĵ������ֵ��Ȼ�����ABS���������ֵ
	if (ABS(pixval32f(dog_pyr[o][i], r, c)) > prelim_contr_thr)
		//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ3*3*3�����ڵĵ�Ƚ��������˵��Ƿ�ֵ��(����ֵ��С����)
	if (is_extremum(dog_pyr, o, i, r, c))//���Ǽ�ֵ��
	{
		//���ڼ�ֵ��ļ��������ɢ�ռ��н��еģ����Լ�⵽�ļ�ֵ�㲢��һ�������������ϵļ�ֵ��
		//��Ϊ�����ļ�ֵ�����λ����������֮�䣬������ɢ�ռ���ֻ�ܾ�ȷ������㾫����
		//ͨ�������ؼ���ֵ���м�ֵ�㾫ȷ��λ(������ֵ������)����ȥ���ͶԱȶȵļ�ֵ�㣬������������������feature�ṹ����
		feat = interp_extremum(dog_pyr, o, i, r, c, intvls, contr_thr);
		//����ֵ�ǿգ������˵��ѱ��ɹ�����
		if (feat)
		{
			//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
			ddata = feat_detection_data(feat);
			//ȥ����Ե��Ӧ����ͨ�����������ʱ�ֵ�ж�ĳ���Ƿ��Ե��,����ֵΪ0��ʾ���Ǳ�Ե�㣬����������
			if (!is_too_edge_like(dog_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, curv_thr))
			{
				cvSeqPush(features, feat);//������������featuresĩβ�����¼�⵽��������feat
			}
			else
				free(ddata);
			free(feat);
		}
	}


	for (o = 1; o < octvs; o++)//��o��
	for (i = 1; i <= intvls; i++)//��i��
	for (winr = 0; winr < (dog_pyr[o][0]->height - 2 * SIFT_IMG_BORDER) / windowHeight; winr++)//�����������ڵ���
	for (winc = 0; winc < (dog_pyr[o][0]->width - 2 * SIFT_IMG_BORDER) / windowWidth; winc++)//�����������ڵ���
	{
		int leftTop = SIFT_IMG_BORDER + winr*windowHeight;//�����ڵ�һ�����ͼ�񶥲�����
		int topLeft = SIFT_IMG_BORDER + winc*windowWidth;//�����ڵ�һ�����ͼ����˾���
		LinkQueue maxQ;//����һ���ն��У�����������ֵ
		int initmax = InitQueue(&maxQ);
		LinkQueue minQ;//����һ���ն��У����������Сֵ
		int initmin = InitQueue(&minQ);


		for (r = leftTop; r < leftTop + windowHeight; r++)
		for (c = topLeft; c < topLeft + windowWidth; c++)
		if (ABS(pixval32f(dog_pyr[o][i], r, c)) > prelim_contr_thr)
		if (is_intvls_max(dog_pyr, o, i, r, c))
		{
			QElemType max;
			max.c = c;
			max.r = r;
			max.value = pixval32f(dog_pyr[o][i], r, c);
			EnMaxQueue(&maxQ, max);
		}
		else if (is_intvls_min(dog_pyr, o, i, r, c))
		{
			QElemType min;
			min.c = c;
			min.r = r;
			min.value = pixval32f(dog_pyr[o][i], r, c);
			EnMinQueue(&minQ, min);
		}


		if (!QueueEmpty(maxQ))
		{
			QElemType max;
			DeQueue(&maxQ, &max);
			if (is_extremum(dog_pyr, o, i, max.r, max.c))//���Ǽ�ֵ��
			{
				feat = interp_extremum(dog_pyr, o, i, r, c, intvls, contr_thr);
				if (feat)
				{
					ddata = feat_detection_data(feat);
					if (!is_too_edge_like(dog_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, curv_thr))
					{
						cvSeqPush(features, feat);//������������featuresĩβ�����¼�⵽��������feat
					}
					else
						free(ddata);
					free(feat);
				}
			}
		}


		if (!QueueEmpty(minQ))
		{
			QElemType min;
			DeQueue(&minQ, &min);
			if (is_extremum(dog_pyr, o, i, min.r, min.c))//���Ǽ�ֵ��
			{
				feat = interp_extremum(dog_pyr, o, i, r, c, intvls, contr_thr);
				if (feat)
				{
					ddata = feat_detection_data(feat);
					if (!is_too_edge_like(dog_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, curv_thr))
					{
						cvSeqPush(features, feat);//������������featuresĩβ�����¼�⵽��������feat
					}
					else
						free(ddata);
					free(feat);
				}
			}
		}

		ClearQueue(&maxQ);
		ClearQueue(&minQ);
		DestroyQueue(&maxQ);
		DestroyQueue(&minQ);
	}


	/*	for( o = octvs/2; o < octvs; o++ )//��o��
	for( i = 1; i <= intvls; i++ )//��i��
	for(r = SIFT_IMG_BORDER; r < dog_pyr[o][0]->height-SIFT_IMG_BORDER; r++)//��r��
	for(c = SIFT_IMG_BORDER; c < dog_pyr[o][0]->width-SIFT_IMG_BORDER; c++)//��c��
	//���г����ĶԱȶȼ�飬ֻ�е���һ���������ֵ���ڶԱȶ���ֵprelim_contr_thrʱ�ż����������ص��Ƿ�����Ǽ�ֵ
	//���ú���pixval32f��ȡͼ��dog_pyr[o][i]�ĵ�r�е�c�еĵ������ֵ��Ȼ�����ABS���������ֵ
	if( ABS( pixval32f( dog_pyr[o][i], r, c ) ) > prelim_contr_thr )
	//ͨ���ڳ߶ȿռ��н�һ�����ص��ֵ������Χ3*3*3�����ڵĵ�Ƚ��������˵��Ƿ�ֵ��(����ֵ��С����)
	if( is_extremum( dog_pyr, o, i, r, c ) )//���Ǽ�ֵ��
	{
	//���ڼ�ֵ��ļ��������ɢ�ռ��н��еģ����Լ�⵽�ļ�ֵ�㲢��һ�������������ϵļ�ֵ��
	//��Ϊ�����ļ�ֵ�����λ����������֮�䣬������ɢ�ռ���ֻ�ܾ�ȷ������㾫����
	//ͨ�������ؼ���ֵ���м�ֵ�㾫ȷ��λ(������ֵ������)����ȥ���ͶԱȶȵļ�ֵ�㣬������������������feature�ṹ����
	feat = interp_extremum(dog_pyr, o, i, r, c, intvls, contr_thr);
	//����ֵ�ǿգ������˵��ѱ��ɹ�����
	if( feat )
	{
	//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
	ddata = feat_detection_data( feat );
	//ȥ����Ե��Ӧ����ͨ�����������ʱ�ֵ�ж�ĳ���Ƿ��Ե��,����ֵΪ0��ʾ���Ǳ�Ե�㣬����������
	if( ! is_too_edge_like( dog_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, curv_thr ) )
	{
	cvSeqPush( features, feat );//������������featuresĩβ�����¼�⵽��������feat
	}
	else
	free( ddata );
	free( feat );
	}
	}
	*/

	return features;//��������������
}





























/*ͨ�������ؼ���ֵ���м�ֵ�㾫ȷ��λ(������ֵ������)����ȥ���ͶԱȶȵļ�ֵ�㣬������������������feature�ṹ����
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
intvls��ÿ��Ĳ���
contr_thr���Աȶ���ֵ����Թ�һ�����ͼ������ȥ�����ȶ�����
����ֵ�����ؾ���ֵ�������������(feature����)���������޴β�ֵ��Ȼ�޷���ȷ������������߸õ�Աȶȹ��ͣ�����NULL
*/
static struct feature* interp_extremum(IplImage*** dog_pyr, int octv, int intvl,
	int r, int c, int intvls, double contr_thr)
{
	struct feature* feat;//�������������
	struct detection_data* ddata;//����������йصĽṹ������feature�ṹ��feature_data��Ա��
	double xi, xr, xc, contr;//xi,xr,xc�ֱ�Ϊ�����ص�intvl(��),row(y),col(x)�����ϵ�����(ƫ����)
	int i = 0;//��ֵ����

	//SIFT_MAX_INTERP_STEPSָ���˹ؼ��������ֵ������������������ٴΣ�Ĭ����5
	while (i < SIFT_MAX_INTERP_STEPS)
	{
		//����һ�μ�ֵ���ֵ�������(�㷽��,intvl����)��y��x�����ϵ�������ƫ����(����)
		interp_step(dog_pyr, octv, intvl, r, c, &xi, &xr, &xc);
		//�������ⷽ���ϵ�ƫ��������0.5ʱ����ζ�Ų�ֵ�����Ѿ�ƫ�Ƶ������ٽ����ϣ����Ա���ı䵱ǰ�ؼ����λ������
		if (ABS(xi) < 0.5  &&  ABS(xr) < 0.5  &&  ABS(xc) < 0.5)//����������ƫ������С��0.5����ʾ�Ѿ�����ȷ�����ü�����ֵ
			break;

		//�����ؼ�������꣬x,y,���������ϵ�ԭ�������ƫ����ȡ��(��������)
		c += cvRound(xc);//x��������
		r += cvRound(xr);//y��������
		intvl += cvRound(xi);//�ҷ��򣬼��㷽��

		//�����������󳬳���Χ���������ֵ������NULL
		if (intvl < 1 ||           //�������֮��Խ��
			intvl > intvls ||
			c < SIFT_IMG_BORDER ||   //���������֮�󵽱߽�����
			r < SIFT_IMG_BORDER ||
			c >= dog_pyr[octv][0]->width - SIFT_IMG_BORDER ||
			r >= dog_pyr[octv][0]->height - SIFT_IMG_BORDER)
		{
			return NULL;
		}

		i++;
	}

	//������SIFT_MAX_INTERP_STEPS�β�ֵ��û������������ľ�ȷλ�ã��򷵻�NULL���������˼�ֵ��
	if (i >= SIFT_MAX_INTERP_STEPS)
		return NULL;

	//���㱻��ֵ��ĶԱȶȣ�D + 0.5 * dD^T * X
	contr = interp_contr(dog_pyr, octv, intvl, r, c, xi, xr, xc);
	if (ABS(contr) < contr_thr / intvls)//���õ�Աȶȹ�С������������NULL
		return NULL;

	//Ϊһ��������feature�ṹ����ռ䲢��ʼ��������������ָ��
	feat = new_feature();
	//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
	ddata = feat_detection_data(feat);

	//������������긳ֵ��������feat
	//ԭͼ���������x���꣬��Ϊ��octv���е�ͼ�ĳߴ��ԭͼС2^octv������������ֵҪ����2^octv
	feat->img_pt.x = feat->x = (c + xc) * pow(2.0, octv);
	//ԭͼ���������y���꣬��Ϊ��octv���е�ͼ�ĳߴ��ԭͼС2^octv������������ֵҪ����2^octv
	feat->img_pt.y = feat->y = (r + xr) * pow(2.0, octv);

	ddata->r = r;//���������ڵ���
	ddata->c = c;//���������ڵ���
	ddata->octv = octv;//��˹��ֽ������У����������ڵ���
	ddata->intvl = intvl;//��˹��ֽ������У����������ڵ����еĲ�
	ddata->subintvl = xi;//�������ڲ㷽��(�ҷ���,intvl����)�ϵ�������ƫ����

	return feat;//����������ָ��
}


/*����һ�μ�ֵ���ֵ������x��y���ҷ���(�㷽��)�ϵ�������ƫ����(����)
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
xi������������㷽���ϵ�����������(ƫ��)
xr�����������y�����ϵ�����������(ƫ��)
xc�����������x�����ϵ�����������(ƫ��)
*/
static void interp_step(IplImage*** dog_pyr, int octv, int intvl, int r, int c,
	double* xi, double* xr, double* xc)
{
	CvMat* dD, *H, *H_inv, X;
	double x[3] = { 0 };

	//��DoG�������м���ĳ���x����y�����Լ��߶ȷ����ϵ�ƫ��������������������dD��
	dD = deriv_3D(dog_pyr, octv, intvl, r, c);
	//��DoG�������м���ĳ���3*3��ɭ����
	H = hessian_3D(dog_pyr, octv, intvl, r, c);
	H_inv = cvCreateMat(3, 3, CV_64FC1);//��ɭ���������
	cvInvert(H, H_inv, CV_SVD);
	cvInitMatHeader(&X, 3, 1, CV_64FC1, x, CV_AUTOSTEP);
	//X = - H^(-1) * dD��H������Ԫ�طֱ���x,y,�ҷ����ϵ�ƫ����(�����SIFT�㷨˵��)
	cvGEMM(H_inv, dD, -1, NULL, 0, &X, 0);

	cvReleaseMat(&dD);
	cvReleaseMat(&H);
	cvReleaseMat(&H_inv);

	*xi = x[2];//�ҷ���(�㷽��)ƫ����
	*xr = x[1];//y������ƫ����
	*xc = x[0];//x������ƫ����
}


/*��DoG�������м���ĳ���x����y�����Լ��߶ȷ����ϵ�ƫ����
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
����ֵ������3��ƫ������ɵ�������{ dI/dx, dI/dy, dI/ds }^T
*/
static CvMat* deriv_3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
	CvMat* dI;
	double dx, dy, ds;

	//����������ƫ�����������õĸ������ȡ��ֵ���ݶȼ��㷽��
	//��x�����ϵĲ�������ƴ���ƫ����
	dx = (pixval32f(dog_pyr[octv][intvl], r, c + 1) -
		pixval32f(dog_pyr[octv][intvl], r, c - 1)) / 2.0;
	//��y�����ϵĲ�������ƴ���ƫ����
	dy = (pixval32f(dog_pyr[octv][intvl], r + 1, c) -
		pixval32f(dog_pyr[octv][intvl], r - 1, c)) / 2.0;
	//����Ĳ�������ƴ���߶ȷ����ϵ�ƫ����
	ds = (pixval32f(dog_pyr[octv][intvl + 1], r, c) -
		pixval32f(dog_pyr[octv][intvl - 1], r, c)) / 2.0;

	//���������
	dI = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(dI, 0, 0, dx);
	cvmSet(dI, 1, 0, dy);
	cvmSet(dI, 2, 0, ds);

	return dI;
}


/*��DoG�������м���ĳ���3*3��ɭ����
/ Ixx  Ixy  Ixs \
| Ixy  Iyy  Iys |
\ Ixs  Iys  Iss /
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
����ֵ������3*3�ĺ�ɭ����
*/
static CvMat* hessian_3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
	CvMat* H;
	double v, dxx, dyy, dss, dxy, dxs, dys;

	v = pixval32f(dog_pyr[octv][intvl], r, c);//�õ������ֵ

	//�ò�ֽ��ƴ��浹��(���幫ʽ�������ݶȵ���)
	//dxx = f(i+1,j) - 2f(i,j) + f(i-1,j)
	//dyy = f(i,j+1) - 2f(i,j) + f(i,j-1)
	dxx = (pixval32f(dog_pyr[octv][intvl], r, c + 1) +
		pixval32f(dog_pyr[octv][intvl], r, c - 1) - 2 * v);
	dyy = (pixval32f(dog_pyr[octv][intvl], r + 1, c) +
		pixval32f(dog_pyr[octv][intvl], r - 1, c) - 2 * v);
	dss = (pixval32f(dog_pyr[octv][intvl + 1], r, c) +
		pixval32f(dog_pyr[octv][intvl - 1], r, c) - 2 * v);
	dxy = (pixval32f(dog_pyr[octv][intvl], r + 1, c + 1) -
		pixval32f(dog_pyr[octv][intvl], r + 1, c - 1) -
		pixval32f(dog_pyr[octv][intvl], r - 1, c + 1) +
		pixval32f(dog_pyr[octv][intvl], r - 1, c - 1)) / 4.0;
	dxs = (pixval32f(dog_pyr[octv][intvl + 1], r, c + 1) -
		pixval32f(dog_pyr[octv][intvl + 1], r, c - 1) -
		pixval32f(dog_pyr[octv][intvl - 1], r, c + 1) +
		pixval32f(dog_pyr[octv][intvl - 1], r, c - 1)) / 4.0;
	dys = (pixval32f(dog_pyr[octv][intvl + 1], r + 1, c) -
		pixval32f(dog_pyr[octv][intvl + 1], r - 1, c) -
		pixval32f(dog_pyr[octv][intvl - 1], r + 1, c) +
		pixval32f(dog_pyr[octv][intvl - 1], r - 1, c)) / 4.0;

	//��ɺ�ɭ����
	H = cvCreateMat(3, 3, CV_64FC1);
	cvmSet(H, 0, 0, dxx);
	cvmSet(H, 0, 1, dxy);
	cvmSet(H, 0, 2, dxs);
	cvmSet(H, 1, 0, dxy);
	cvmSet(H, 1, 1, dyy);
	cvmSet(H, 1, 2, dys);
	cvmSet(H, 2, 0, dxs);
	cvmSet(H, 2, 1, dys);
	cvmSet(H, 2, 2, dss);

	return H;
}


/*���㱻��ֵ��ĶԱȶȣ�D + 0.5 * dD^T * X
������
dog_pyr����˹��ֽ�����
octv�����ص����ڵ���
intvl�����ص����ڵĲ�
r�����ص����ڵ���
c�����ص����ڵ���
xi���㷽���ϵ�����������
xr��y�����ϵ�����������
xc��x�����ϵ�����������
����ֵ����ֵ��ĶԱȶ�
*/
static double interp_contr(IplImage*** dog_pyr, int octv, int intvl, int r,
	int c, double xi, double xr, double xc)
{
	CvMat* dD, X, T;
	double t[1], x[3] = { xc, xr, xi };

	//ƫ������ɵ�������X��������x��y�����������ϵ�ƫ����
	cvInitMatHeader(&X, 3, 1, CV_64FC1, x, CV_AUTOSTEP);
	//����˷��Ľ��T����һ����ֵ
	cvInitMatHeader(&T, 1, 1, CV_64FC1, t, CV_AUTOSTEP);
	//��DoG�������м���ĳ���x����y�����Լ��߶ȷ����ϵ�ƫ��������������������dD��
	dD = deriv_3D(dog_pyr, octv, intvl, r, c);
	//����˷���T = dD^T * X
	cvGEMM(dD, &X, 1, NULL, 0, &T, CV_GEMM_A_T);
	cvReleaseMat(&dD);

	//���ؼ�����ĶԱȶ�ֵ��D + 0.5 * dD^T * X (���幫ʽ�Ƶ���SIFT�㷨˵��)
	return pixval32f(dog_pyr[octv][intvl], r, c) + t[0] * 0.5;
}


/*Ϊһ��feature�ṹ����ռ䲢��ʼ��
����ֵ����ʼ����ɵ�feature�ṹ��ָ��
*/
static struct feature* new_feature(void)
{
	struct feature* feat;//������ָ��
	struct detection_data* ddata;//�����������صĽṹ

	feat = (struct feature*)malloc(sizeof(struct feature));//����ռ�
	memset(feat, 0, sizeof(struct feature));//����
	ddata = (struct detection_data*)malloc(sizeof(struct detection_data));
	memset(ddata, 0, sizeof(struct detection_data));
	feat->feature_data = ddata;//�����������صĽṹָ�븳ֵ���������feature_data��Ա
	feat->type = FEATURE_LOWE;//Ĭ����LOWE���͵�������

	return feat;
}


/*ȥ����Ե��Ӧ����ͨ�����������ʱ�ֵ�ж�ĳ���Ƿ��Ե��
������
dog_img�������������ڵ�DoGͼ��
r�����������ڵ���
c�����������ڵ���
cur_thr�������ʱ�ֵ����ֵ������ȥ����Ե����
����ֵ��0���˵��ǷǱ�Ե�㣻1���˵��Ǳ�Ե��
*/
static int is_too_edge_like(IplImage* dog_img, int r, int c, int curv_thr)
{
	double d, dxx, dyy, dxy, tr, det;

	/*ĳ������������亣ɭ���������ֵ�����ȣ�Ϊ�˱���ֱ�Ӽ�������ֵ������ֻ��������ֵ�ı�ֵ
	��ͨ�����㺣ɭ����ļ�tr(H)������ʽdet(H)����������ֵ�ı�ֵ
	��a�Ǻ�ɭ����Ľϴ�����ֵ��b�ǽ�С������ֵ����a = r*b��r�Ǵ�С����ֵ�ı�ֵ
	tr(H) = a + b; det(H) = a*b;
	tr(H)^2 / det(H) = (a+b)^2 / ab = (r+1)^2/r
	rԽ��Խ�����Ǳ�Ե�㣻����r������(r+1)^2/r ��ֵҲ�������Կ�ͨ��(r+1)^2/r �ж������ʱ�ֵ�Ƿ���������*/
	/* principal curvatures are computed using the trace and det of Hessian */
	d = pixval32f(dog_img, r, c);//���ú���pixval32f��ȡͼ��dog_img�ĵ�r�е�c�еĵ������ֵ

	//�ò�ֽ��ƴ���ƫ���������ɭ����ļ���Ԫ��ֵ
	/*  / dxx  dxy \
	\ dxy  dyy /   */
	dxx = pixval32f(dog_img, r, c + 1) + pixval32f(dog_img, r, c - 1) - 2 * d;
	dyy = pixval32f(dog_img, r + 1, c) + pixval32f(dog_img, r - 1, c) - 2 * d;
	dxy = (pixval32f(dog_img, r + 1, c + 1) - pixval32f(dog_img, r + 1, c - 1) -
		pixval32f(dog_img, r - 1, c + 1) + pixval32f(dog_img, r - 1, c - 1)) / 4.0;
	tr = dxx + dyy;//��ɭ����ļ�
	det = dxx * dyy - dxy * dxy;//��ɭ���������ʽ

	//������ʽΪ�������������в�ͬ�ķ��ţ�ȥ���˵�
	/* negative determinant -> curvatures have different signs; reject feature */
	if (det <= 0)
		return 1;//����1�����˵��Ǳ�Ե��

	//ͨ��ʽ�ӣ�(r+1)^2/r �ж������ʵı�ֵ�Ƿ�������������С����ֵ���������Ǳ�Ե��
	if (tr * tr / det < (curv_thr + 1.0)*(curv_thr + 1.0) / curv_thr)
		return 0;//���Ǳ�Ե��
	return 1;//�Ǳ�Ե��
}


/*����������������ÿ��������ĳ߶�
������
features������������
sigma����ʼ��˹ƽ������������ʼ�߶�
intvls���߶ȿռ���ÿ��Ĳ���
*/
static void calc_feature_scales(CvSeq* features, double sigma, int intvls)
{
	struct feature* feat;
	struct detection_data* ddata;
	double intvl;
	int i, n;

	n = features->total;//�ܵ����������

	//����������
	for (i = 0; i < n; i++)
	{
		//���ú꣬��ȡ����features�еĵ�i��Ԫ�أ���ǿ��ת��Ϊstruct feature����
		feat = CV_GET_SEQ_ELEM(struct feature, features, i);
		//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
		ddata = feat_detection_data(feat);
		//���������ڵĲ���ddata->intvl�����������ڲ㷽���ϵ�������ƫ�������õ�������Ľ�Ϊ��ȷ�Ĳ���
		intvl = ddata->intvl + ddata->subintvl;
		//����������ĳ߶�(��ʽ��SIFT�㷨˵��)������ֵ��scl��Ա
		feat->scl = sigma * pow(2.0, ddata->octv + intvl / intvls);
		//�������������ڵ���ĳ߶ȣ���detection_data��scl_octv��Ա��ֵ
		ddata->scl_octv = sigma * pow(2.0, intvl / intvls);
	}
}


/*��������������ÿ����������������(�������˽�ͼ��Ŵ�Ϊԭͼ��2��ʱ������������֮�����)
������
features������������
*/
static void adjust_for_img_dbl(CvSeq* features)
{
	struct feature* feat;
	int i, n;

	n = features->total;//�ܵ����������

	//����������
	for (i = 0; i < n; i++)
	{
		//���ú꣬��ȡ����features�еĵ�i��Ԫ�أ���ǿ��ת��Ϊstruct feature����
		feat = CV_GET_SEQ_ELEM(struct feature, features, i);
		//���������x,y����ͳ߶ȶ�����
		feat->x /= 2.0;
		feat->y /= 2.0;
		feat->scl /= 2.0;
		feat->img_pt.x /= 2.0;
		feat->img_pt.y /= 2.0;
	}
}


/*����ÿ����������ݶ�ֱ��ͼ���ҳ�����������һ���������в�ֹһ�������򣬽����Ϊ����������
������
features������������
gauss_pyr����˹������
*/
static void calc_feature_oris(CvSeq* features, IplImage*** gauss_pyr)
{
	struct feature* feat;
	struct detection_data* ddata;
	double* hist;//����ݶ�ֱ��ͼ������
	double omax;
	int i, j, n = features->total;//���������

	//��������������
	for (i = 0; i < n; i++)
	{
		//��ÿ�����������feature�ṹ��С���ڴ�
		feat = (struct feature*)malloc(sizeof(struct feature));
		//�Ƴ�����Ԫ�أ��ŵ�feat��
		cvSeqPopFront(features, feat);
		//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
		//detection_data�����д���д����������������ͳ߶ȣ��Լ����ڵĲ����
		ddata = feat_detection_data(feat);

		//����ָ�����ص���ݶȷ���ֱ��ͼ�����ش��ֱ��ͼ�������hist
		hist = ori_hist(gauss_pyr[ddata->octv][ddata->intvl],       //���������ڵ�ͼ��
			ddata->r, ddata->c,                          //���������������
			SIFT_ORI_HIST_BINS,                          //Ĭ�ϵ��ݶ�ֱ��ͼ��bin(����)����
			cvRound(SIFT_ORI_RADIUS * ddata->scl_octv),//�����㷽��ֵ�����У���������İ뾶Ϊ��3 * 1.5 * ��
			SIFT_ORI_SIG_FCTR * ddata->scl_octv);       //����ֱ��ͼʱ�ݶȷ�ֵ�ĸ�˹Ȩ�صĳ�ʼֵ

		//���ݶ�ֱ��ͼ���и�˹ƽ�����ֲ���û�з��䲻���Զ������������㲻�ȶ�������,SIFT_ORI_SMOOTH_PASSESָ����ƽ������
		for (j = 0; j < SIFT_ORI_SMOOTH_PASSES; j++)
			smooth_ori_hist(hist, SIFT_ORI_HIST_BINS);

		//�����ݶ�ֱ��ͼ����������ݶȷ�ֵ��������ֱ��ͼ�����bin��ֵ,���ظ�omax
		omax = dominant_ori(hist, SIFT_ORI_HIST_BINS);
		/*����ǰ�������ֱ��ͼ��ĳ��bin��ֵ���ڸ�������ֵ����������һ�������㲢��ӵ�����������ĩβ
		�����������ָ��feat���Ѿ�������������features���Ƴ��ģ����Լ�ʹ��������û�и�����(�ڶ������ڷ�ֵ��ֵ�ķ���)
		�ں���add_good_ori_features��Ҳ��ִ��һ�ο�¡feat�����䷽����в�ֵ���������������������еĲ���
		��ֵ��ֵһ������Ϊ��ǰ��������ݶ�ֱ��ͼ�����binֵ��80%                   */
		add_good_ori_features(features, hist, SIFT_ORI_HIST_BINS,
			omax * SIFT_ORI_PEAK_RATIO, feat);
		//�ͷ��ڴ�
		free(ddata);
		free(feat);
		free(hist);
	}
}


/*����ָ�����ص���ݶȷ���ֱ��ͼ�����ش��ֱ��ͼ������
������
img��ͼ��ָ��
r�����������ڵ���
c�����������ڵ���
n��ֱ��ͼ����(bin)�ĸ�����Ĭ����36
rad������뾶���ڴ������м����ݶȷ���ֱ��ͼ
sigma������ֱ��ͼʱ�ݶȷ�ֵ�ĸ�˹Ȩ�صĳ�ʼֵ
����ֵ������һ��nԪ���飬�����Ƿ���ֱ��ͼ��ͳ������
*/
static double* ori_hist(IplImage* img, int r, int c, int n, int rad, double sigma)
{
	double* hist;//ֱ��ͼ����
	double mag, ori, w, exp_denom, PI2 = CV_PI * 2.0;
	int bin, i, j;

	//Ϊֱ��ͼ�������ռ䣬��n��Ԫ�أ�n�����ĸ���
	hist = (double*)calloc(n, sizeof(double));
	exp_denom = 2.0 * sigma * sigma;

	//������ָ����Ϊ���ĵ���������
	for (i = -rad; i <= rad; i++)
	for (j = -rad; j <= rad; j++)
		//����ָ������ݶȵķ�ֵmag�ͷ���ori������ֵΪ1��ʾ����ɹ�
	if (calc_grad_mag_ori(img, r + i, c + j, &mag, &ori))
	{
		w = exp(-(i*i + j*j) / exp_denom);//�õ���ݶȷ�ֵȨ��
		bin = cvRound(n * (ori + CV_PI) / PI2);//�����ݶȵķ����Ӧ��ֱ��ͼ�е�bin�±�
		bin = (bin < n) ? bin : 0;
		hist[bin] += w * mag;//��ֱ��ͼ��ĳ��bin���ۼӼ�Ȩ��ķ�ֵ
	}

	return hist;//����ֱ��ͼ����
}


/*����ָ������ݶȵķ�ֵmagnitude�ͷ���orientation
������
img��ͼ��ָ��
r�����������ڵ���
c�����������ڵ���
img������������˵���ݶȷ�ֵ
ori������������˵���ݶȷ���
����ֵ�����ָ���ĵ��ǺϷ��㲢�Ѽ������ֵ�ͷ��򣬷���1�����򷵻�0
*/
static int calc_grad_mag_ori(IplImage* img, int r, int c, double* mag, double* ori)
{
	double dx, dy;

	//�����������ֵ���м��
	if (r > 0 && r < img->height - 1 && c > 0 && c < img->width - 1)
	{
		//�ò�ֽ��ƴ���ƫ���������ݶȵķ�ֵ�ͷ���
		dx = pixval32f(img, r, c + 1) - pixval32f(img, r, c - 1);//x����ƫ��
		dy = pixval32f(img, r - 1, c) - pixval32f(img, r + 1, c);//y����ƫ��
		*mag = sqrt(dx*dx + dy*dy);//�ݶȵķ�ֵ�����ݶȵ�ģ
		*ori = atan2(dy, dx);//�ݶȵķ���
		return 1;
	}
	//��������ֵ���Ϸ�������0
	else
		return 0;
}


/*���ݶȷ���ֱ��ͼ���и�˹ƽ�����ֲ���û�з��䲻���Զ������������㲻�ȶ�������
������
hist������ݶ�ֱ��ͼ������
n���ݶ�ֱ��ͼ��bin�ĸ���
*/
static void smooth_ori_hist(double* hist, int n)
{
	double prev, tmp, h0 = hist[0];
	int i;

	prev = hist[n - 1];
	//���ƾ�ֵƯ�Ƶ�һ������ƽ��������ͻ���Ӱ��
	for (i = 0; i < n; i++)
	{
		tmp = hist[i];
		hist[i] = 0.25 * prev + 0.5 * hist[i] +
			0.25 * ((i + 1 == n) ? h0 : hist[i + 1]);
		prev = tmp;
	}
}


/*�����ݶ�ֱ��ͼ����������ݶȷ�ֵ��������ֱ��ͼ�����bin��ֵ
������
hist�����ֱ��ͼ������
n��ֱ��ͼ��bin�ĸ���
����ֵ������ֱ��ͼ������bin��ֵ
*/
static double dominant_ori(double* hist, int n)
{
	double omax;
	int maxbin, i;

	omax = hist[0];
	maxbin = 0;

	//����ֱ��ͼ���ҵ�����bin
	for (i = 1; i < n; i++)
	if (hist[i] > omax)
	{
		omax = hist[i];
		maxbin = i;
	}
	return omax;//��������bin��ֵ
}


//�������С�������bin��ֵ�Ե�ǰbin����ֱ��ͼ��ֵ������ȡ����ȷ�ķ���Ƕ�ֵ
#define interp_hist_peak( l, c, r ) ( 0.5 * ((l)-(r)) / ((l) - 2.0*(c) + (r)) )


/*����ǰ�������ֱ��ͼ��ĳ��bin��ֵ���ڸ�������ֵ����������һ�������㲢��ӵ�����������ĩβ
�����������ָ��feat���Ѿ�������������features���Ƴ��ģ����Լ�ʹ��������û�и�����(�ڶ������ڷ�ֵ��ֵ�ķ���)
Ҳ��ִ��һ�ο�¡feat�����䷽����в�ֵ���������������������еĲ���
������
features������������
hist���ݶ�ֱ��ͼ
n��ֱ��ͼ��bin�ĸ���
mag_thr����ֵ��ֵ����ֱ��ͼ����bin��ֵ���ڴ���ֵ����������������
feat��һ��������ָ�룬�µ��������¡��feat��������ͬ
*/
static void add_good_ori_features(CvSeq* features, double* hist, int n,
	double mag_thr, struct feature* feat)
{
	struct feature* new_feat;
	double bin, PI2 = CV_PI * 2.0;
	int l, r, i;

	//����ֱ��ͼ
	for (i = 0; i < n; i++)
	{
		l = (i == 0) ? n - 1 : i - 1;//ǰһ��(��ߵ�)bin���±�
		r = (i + 1) % n;//��һ��(�ұߵ�)bin���±�

		//����ǰ��bin�Ǿֲ���ֵ(��ǰһ���ͺ�һ��bin����)������ֵ���ڸ����ķ�ֵ��ֵ����������һ�������㲢��ӵ�����������ĩβ
		if (hist[i] > hist[l] && hist[i] > hist[r] && hist[i] >= mag_thr)
		{
			//�������С�������bin��ֵ�Ե�ǰbin����ֱ��ͼ��ֵ
			bin = i + interp_hist_peak(hist[l], hist[i], hist[r]);
			bin = (bin < 0) ? n + bin : (bin >= n) ? bin - n : bin;//����ֵ����淶��[0,n]��
			new_feat = clone_feature(feat);//��¡��ǰ������Ϊ��������
			new_feat->ori = ((PI2 * bin) / n) - CV_PI;//��������ķ���
			cvSeqPush(features, new_feat);//���뵽����������ĩβ
			free(new_feat);
		}
	}
}


/*�������feature�ṹ����������������ؿ�¡���ɵ��������ָ��
������
feat����Ҫ����¡���������ָ��
����ֵ���������ɵ��������ָ��
*/
static struct feature* clone_feature(struct feature* feat)
{
	struct feature* new_feat;
	struct detection_data* ddata;

	//Ϊһ��feature�ṹ����ռ䲢��ʼ��
	new_feat = new_feature();
	//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
	ddata = feat_detection_data(new_feat);
	//���ڴ�ռ���и�ֵ
	memcpy(new_feat, feat, sizeof(struct feature));
	memcpy(ddata, feat_detection_data(feat), sizeof(struct detection_data));
	new_feat->feature_data = ddata;

	return new_feat;//���ؿ�¡���ɵ��������ָ��
}


/*����������������ÿ�����������������������
������
features������������
gauss_pyr����˹������ͼ����
d�����㷽��ֱ��ͼʱ���������㸽������Ϊd*d������ÿ����������һ��ֱ��ͼ
n��ÿ������ֱ��ͼ��bin����
*/
static void compute_descriptors(CvSeq* features, IplImage*** gauss_pyr, int d, int n)
{
	struct feature* feat;
	struct detection_data* ddata;
	double*** hist;//d*d*n����άֱ��ͼ����
	int i, k = features->total;//������ĸ���

	//���������������е�������
	for (i = 0; i < k; i++)
	{
		//���ú꣬��ȡ����features�еĵ�i��Ԫ�أ���ǿ��ת��Ϊstruct feature����
		feat = CV_GET_SEQ_ELEM(struct feature, features, i);
		//���ú�feat_detection_data����ȡ����feat�е�feature_data��Ա��ת��Ϊdetection_data���͵�ָ��
		ddata = feat_detection_data(feat);
		//���������㸽������ķ���ֱ��ͼ����ֱ��ͼ�ڼ���������������Ҫ�õ�������ֵ��һ��d*d*n����ά����
		hist = descr_hist(gauss_pyr[ddata->octv][ddata->intvl], ddata->r,
			ddata->c, feat->ori, ddata->scl_octv, d, n);
		//��ĳ������ķ���ֱ��ͼת��Ϊ���������������������������ӹ�һ����������Ԫ��ת��Ϊ���ͣ�����������feat��
		hist_to_descr(hist, d, n, feat);
		//�ͷ�������ķ���ֱ��ͼ
		release_descr_hist(&hist, d);
	}
}

double calc_s_value(struct feature* feat, int nn)///
{
	double S = 0.0;
	double Sum_value = 0.0;
	double avg_x = 0.0, avg_y = 0.0, mdx = 0.0, mdy = 0.0;

	size_t n = nn;//������ĸ���

	int ii = 0;
	for (ii = 0; ii < n; ii++)
	{
		avg_x += feat[ii].x;
		avg_y += feat[ii].y;
	}

	avg_x = avg_x / n;
	avg_y = avg_y / n;

	for (ii = 0; ii < n; ii++)
	{
		mdx = (feat[ii].x - avg_x)*(feat[ii].x - avg_x);
		mdy = (feat[ii].y - avg_y)*(feat[ii].y - avg_y);
		Sum_value += sqrt(mdx + mdy);
	}

	S = Sum_value / n;
	return S;
}

/*���������㸽������ķ���ֱ��ͼ����ֱ��ͼ�ڼ���������������Ҫ�õ�������ֵ��һ��d*d*n����ά����
������
img��ͼ��ָ��
r�����������ڵ���
c�����������ڵ���
ori���������������
scl��������ĳ߶�
d�����㷽��ֱ��ͼʱ���������㸽������Ϊd*d������ÿ����������һ��ֱ��ͼ��Ĭ��dΪ4
n��ÿ��ֱ��ͼ��bin�ĸ���
����ֵ��double���͵���ά���飬��һ��d*d�Ķ�ά���飬������ÿ��Ԫ����һ����n��bin��ֱ��ͼ����
*/
static double*** descr_hist(IplImage* img, int r, int c, double ori,
	double scl, int d, int n)
{
	double*** hist;//d*d*n����άֱ��ͼ����
	double cos_t, sin_t, hist_width, exp_denom, r_rot, c_rot, grad_mag,
		grad_ori, w, rbin, cbin, obin, bins_per_rad, PI2 = 2.0 * CV_PI;
	int radius, i, j;

	//Ϊֱ��ͼ�������ռ�
	hist = (double***)calloc(d, sizeof(double**));//Ϊ��һά����ռ�
	for (i = 0; i < d; i++)
	{
		hist[i] = (double**)calloc(d, sizeof(double*));//Ϊ�ڶ�ά����ռ�
		for (j = 0; j < d; j++)
			hist[i][j] = (double*)calloc(n, sizeof(double));//Ϊ����ά����ռ�
	}

	//Ϊ�˱�֤���������Ӿ�����ת�����ԣ�Ҫ��������Ϊ���ģ��ڸ�����������ת�Ƚǣ�����תΪ������ķ���
	cos_t = cos(ori);
	sin_t = sin(ori);

	bins_per_rad = n / PI2;
	exp_denom = d * d * 0.5;
	//�������������ӹ����У���������Χ��d*d�������У�ÿ������Ŀ��Ϊm*�Ҹ����أ�SIFT_DESCR_SCL_FCTR��m��Ĭ��ֵ����Ϊ������ĳ߶�
	hist_width = SIFT_DESCR_SCL_FCTR * scl;
	//���ǵ�Ҫ����˫���Բ�ֵ��ÿ������Ŀ��ӦΪ:SIFT_DESCR_SCL_FCTR * scl * ( d + 1.0 )
	//�ڿ��ǵ���ת���أ�ÿ������Ŀ��ӦΪ��SIFT_DESCR_SCL_FCTR * scl * ( d + 1.0 ) * sqrt(2)
	//���������İ뾶�ǣ�SIFT_DESCR_SCL_FCTR * scl * ( d + 1.0 ) * sqrt(2) / 2
	radius = hist_width * sqrt(2) * (d + 1.0) * 0.5 + 0.5;

	//����ÿ�����������
	for (i = -radius; i <= radius; i++)
	for (j = -radius; j <= radius; j++)
	{
		//������תΪ������
		//���濴������
		c_rot = (j * cos_t - i * sin_t) / hist_width;
		r_rot = (j * sin_t + i * cos_t) / hist_width;
		rbin = r_rot + d / 2 - 0.5;
		cbin = c_rot + d / 2 - 0.5;

		if (rbin > -1.0  &&  rbin < d  &&  cbin > -1.0  &&  cbin < d)
		if (calc_grad_mag_ori(img, r + i, c + j, &grad_mag, &grad_ori))
		{
			grad_ori -= ori;
			while (grad_ori < 0.0)
				grad_ori += PI2;
			while (grad_ori >= PI2)
				grad_ori -= PI2;

			obin = grad_ori * bins_per_rad;
			w = exp(-(c_rot * c_rot + r_rot * r_rot) / exp_denom);
			interp_hist_entry(hist, rbin, cbin, obin, grad_mag * w, d, n);
		}
	}

	return hist;
}


//������˫���Բ�ֵ������Ŀ�����
static void interp_hist_entry(double*** hist, double rbin, double cbin,
	double obin, double mag, int d, int n)
{
	double d_r, d_c, d_o, v_r, v_c, v_o;
	double** row, *h;
	int r0, c0, o0, rb, cb, ob, r, c, o;

	r0 = cvFloor(rbin);
	c0 = cvFloor(cbin);
	o0 = cvFloor(obin);
	d_r = rbin - r0;
	d_c = cbin - c0;
	d_o = obin - o0;

	for (r = 0; r <= 1; r++)
	{
		rb = r0 + r;
		if (rb >= 0 && rb < d)
		{
			v_r = mag * ((r == 0) ? 1.0 - d_r : d_r);
			row = hist[rb];
			for (c = 0; c <= 1; c++)
			{
				cb = c0 + c;
				if (cb >= 0 && cb < d)
				{
					v_c = v_r * ((c == 0) ? 1.0 - d_c : d_c);
					h = row[cb];
					for (o = 0; o <= 1; o++)
					{
						ob = (o0 + o) % n;
						v_o = v_c * ((o == 0) ? 1.0 - d_o : d_o);
						h[ob] += v_o;
					}
				}
			}
		}
	}
}


/*��ĳ������ķ���ֱ��ͼת��Ϊ���������������������������ӹ�һ����������Ԫ��ת��Ϊ���ͣ�����ָ����������
������
hist��d*d*n����άֱ��ͼ����
d�����㷽��ֱ��ͼʱ���������㸽������Ϊd*d������ÿ����������һ��ֱ��ͼ
n��ÿ��ֱ��ͼ��bin����
feat��������ָ�룬������õ����������Ӵ�������
*/
static void hist_to_descr(double*** hist, int d, int n, struct feature* feat)
{
	int int_val, i, r, c, o, k = 0;

	//����d*d*n����άֱ��ͼ���飬�����е���������(һ����128��)������feat�ṹ��descr��Ա��
	for (r = 0; r < d; r++)
	for (c = 0; c < d; c++)
	for (o = 0; o < n; o++)
		feat->descr[k++] = hist[r][c][o];

	feat->d = k;//���������ӵ�ά����һ����128
	//��һ������������������ӣ���������������������ÿ��Ԫ�س������������ӵ�ģ
	normalize_descr(feat);

	//����������������������������ֵSIFT_DESCR_MAG_THR��Ԫ��ǿ�и�ֵΪSIFT_DESCR_MAG_THR
	for (i = 0; i < k; i++)
	if (feat->descr[i] > SIFT_DESCR_MAG_THR)
		feat->descr[i] = SIFT_DESCR_MAG_THR;
	//�ٴι�һ����������������
	normalize_descr(feat);

	/* convert floating-point descriptor to integer valued descriptor */
	//��������������������ÿ��Ԫ�س���ϵ��SIFT_INT_DESCR_FCTR����Ϊ���ͣ��������ֵ���ܳ���255
	for (i = 0; i < k; i++)
	{
		int_val = SIFT_INT_DESCR_FCTR * feat->descr[i];
		feat->descr[i] = MIN(255, int_val);
	}
}


/*��һ������������������ӣ���������������������ÿ��Ԫ�س������������ӵ�ģ
*/
static void normalize_descr(struct feature* feat)
{
	double cur, len_inv, len_sq = 0.0;
	int i, d = feat->d;//���������ӵ�ά��

	//�����������ӵ�ģ
	for (i = 0; i < d; i++)
	{
		cur = feat->descr[i];
		len_sq += cur*cur;
	}
	len_inv = 1.0 / sqrt(len_sq);
	//������������ÿ��Ԫ�س������������ӵ�ģ����ɹ�һ��
	for (i = 0; i < d; i++)
		feat->descr[i] *= len_inv;
}


/*�ȽϺ������������㰴�߶ȵĽ������У���������������CvSeqSort��
������
feat1����һ���������ָ��
feat2���ڶ����������ָ��
param���û��Զ�����������ﲻʹ��
����ֵ�����feat1�ĳ߶ȴ���feat2�ĳ߶ȣ�����1�����򷵻�-1������ȷ���0(������)
*/
static int feature_cmp(void* feat1, void* feat2, void* param)
{
	//������Ĳ���ǿ��ת��Ϊstruct feature���͵�ָ��
	struct feature* f1 = (struct feature*) feat1;
	struct feature* f2 = (struct feature*) feat2;

	//�Ƚ�����������ĳ߶�ֵ
	if (f1->scl < f2->scl)
		return 1;
	if (f1->scl > f2->scl)
		return -1;
	return 0;
}


/*�ͷż������������ӹ������õ��ķ���ֱ��ͼ���ڴ�ռ�
������
hist������ֱ��ͼ��ָ�룬��һ��d*d*n����άֱ��ͼ����
d��ֱ��ͼ����ǰ��ά��ά��
*/
static void release_descr_hist(double**** hist, int d)
{
	int i, j;

	for (i = 0; i < d; i++)
	{
		for (j = 0; j < d; j++)
			free((*hist)[i][j]);//�ͷŵ���ά���ڴ�
		free((*hist)[i]);//�ͷŵڶ�ά���ڴ�
	}
	free(*hist);//�ͷŵ�һά���ڴ�
	*hist = NULL;
}

/*�ͷŽ�����ͼ����Ĵ洢�ռ�
����:
pyr��������ͼ�����ָ��
octvs��������������
n��ÿһ���ͼ����
*/
static void release_pyr(IplImage**** pyr, int octvs, int n)
{
	int i, j;
	for (i = 0; i < octvs; i++)
	{
		for (j = 0; j < n; j++)
			cvReleaseImage(&(*pyr)[i][j]);//�ͷ�ÿ��ͼ��
		free((*pyr)[i]);//�ͷ�ÿ����
	}
	free(*pyr);//�ͷŽ�����
	*pyr = NULL;
}


