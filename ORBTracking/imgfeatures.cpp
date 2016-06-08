/*
���ļ����м���������ʵ�֣�������ĵ��뵼����������Ļ���
*/
#include "stdafx.h"
#include "utils.h"
#include "imgfeatures.h"

#include <cxcore.h>

#include <math.h>

int g_FeatureDisplayMode = 0;

/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
static int import_oxfd_features(char*, struct feature**);//����OXFD��ʽ������
static int export_oxfd_features(char*, struct feature*, int);//����OXFD��ʽ������
static void draw_oxfd_features(IplImage*, struct feature*, int);//��OXFD��ʽ������
static void draw_oxfd_feature(IplImage*, struct feature*, CvScalar);//��������

static int import_lowe_features(char*, struct feature**);//����LOWE��ʽ������
static int export_lowe_features(char*, struct feature*, int);//����LOWE��ʽ������
static void draw_lowe_features(IplImage*, struct feature*, int);//��LOWE��ʽ������
static void draw_lowe_feature(IplImage*, struct feature*, CvScalar);//��������


/*���ļ��ж���ͼ������
�ļ��е��������ʽ������FEATURE_OXFD��FEATURE_LOWE��ʽ
������
filename���ļ���
type������������
feat�������洢�������feature�����ָ��
����ֵ����������������
*/
int import_features(char* filename, int type, struct feature** feat)
{
	int n;

	//�������������ͣ����ò�ͬ�ĺ�����ɵ��빦��
	switch (type)
	{
	case FEATURE_OXFD:
		n = import_oxfd_features(filename, feat);//���ú���������OXFD��ʽ������
		break;
	case FEATURE_LOWE:
		n = import_lowe_features(filename, feat);//���ú���������LOWE��ʽ������
		break;
	default: //�������ʽ�޷�ʶ��
		fprintf(stderr, "Warning: import_features(): unrecognized feature" \
			"type, %s, line %d\n", __FILE__, __LINE__);
		return -1;
	}

	//����ʧ��
	if (n == -1)
		fprintf(stderr, "Warning: unable to import features from %s,"	\
		" %s, line %d\n", filename, __FILE__, __LINE__);
	return n;
}


/*����feature���鵽�ļ�
������
filename���ļ���
feat����������
n�����������
����ֵ��0���ɹ���1��ʧ��
*/
int export_features(char* filename, struct feature* feat, int n)///
{
	int r, type;

	//�����Ϸ��Լ��
	if (n <= 0 || !feat)
	{
		fprintf(stderr, "Warning: no features to export, %s line %d\n",
			__FILE__, __LINE__);
		return 1;
	}
	type = feat[0].type;//����������͡�

	//�������������ͣ����ò�ͬ�ĺ�����ɵ�������
	switch (type)
	{
	case FEATURE_OXFD:
		r = export_oxfd_features(filename, feat, n);//���ú���������OXFD��ʽ������
		break;
	case FEATURE_LOWE:
		r = export_lowe_features(filename, feat, n);//���ú���������LOWE��ʽ������
		break;
	default:
		fprintf(stderr, "Warning: export_features(): unrecognized feature" \
			"type, %s, line %d\n", __FILE__, __LINE__);
		return -1;
	}

	if (r) //������������ֵ��0����ʾ����ʧ��
		fprintf(stderr, "Warning: unable to export features to %s,"	\
		" %s, line %d\n", filename, __FILE__, __LINE__);
	return r;
}


/*��ͼƬ�ϻ���������
������
img��ͼ��
feat������������
n�����������
*/
void draw_features(IplImage* img, struct feature* feat, int n)///
{
	int type;

	//�����Ϸ��Լ��
	if (n <= 0 || !feat)
	{
		fprintf(stderr, "Warning: no features to draw, %s line %d\n",
			__FILE__, __LINE__);
		return;
	}
	type = feat[0].type;//�����������

	//�������������ͣ����ò�ͬ�ĺ�����ɻ�ͼ����
	switch (type)
	{
	case FEATURE_OXFD:
		draw_oxfd_features(img, feat, n);//���ú�������ͼ���ϻ�OXFD��ʽ������
		break;
	case FEATURE_LOWE:
		draw_lowe_features(img, feat, n);//���ú�������ͼ���ϻ�LOWE��ʽ������
		break;
	default:
		fprintf(stderr, "Warning: draw_features(): unrecognized feature" \
			" type, %s, line %d\n", __FILE__, __LINE__);
		break;
	}
}


/*�����������������Ӽ��ŷ�Ͼ����ƽ��
������
f1:��һ��������
f2:�ڶ���������
����ֵ��ŷ�Ͼ����ƽ��
*/
double descr_dist_sq(struct feature* f1, struct feature* f2)///
{
	double diff, dsq = 0;
	double* descr1, *descr2;
	int i, d;

	d = f1->d;//f1�����������ӵĳ���
	if (f2->d != d)//��f1��f2�����������ӳ��Ȳ�ͬ������
		return DBL_MAX;
	descr1 = f1->descr;//f1�����������ӣ�һ��double����
	descr2 = f2->descr;//f2�����������ӣ�һ��double����

	//����ŷ�Ͼ����ƽ��������ӦԪ�صĲ��ƽ����
	for (i = 0; i < d; i++)
	{
		diff = descr1[i] - descr2[i];
		dsq += diff*diff;
	}
	return dsq;
}


/***************************** һЩδ��¶�ӿڵ��ڲ����� *******************************/


/*���ļ��ж���OXFD��ʽ��ͼ������
������
filename���ļ���
features�������洢�������feature�����ָ��
����ֵ����������������
*/
static int import_oxfd_features(char* filename, struct feature** features)///
{
	struct feature* f;//��һ���������ָ��
	int i, j, n, d;
	double x, y, a, b, c, dv;
	FILE* file;//�ļ�ָ��

	if (!features)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	//���ļ�
	if (!(file = fopen(filename, "r")))
	{
		fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
			filename, __FILE__, __LINE__);
		return -1;
	}

	//��������������ά�������������
	if (fscanf(file, " %d %d ", &d, &n) != 2)
	{
		fprintf(stderr, "Warning: file read error, %s, line %d\n",
			__FILE__, __LINE__);
		return -1;
	}
	//����������ά�����ڶ�������ά��������
	if (d > FEATURE_MAX_D)
	{
		fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
			__FILE__, __LINE__);
		return -1;
	}

	//�����ڴ棬n��feature�ṹ��С�������׵�ַ��f
	f = (struct feature*)calloc(n, sizeof(struct feature));

	//�����ļ��е�n��������
	for (i = 0; i < n; i++)
	{
		//��������������
		/* read affine region parameters */
		if (fscanf(file, " %lf %lf %lf %lf %lf ", &x, &y, &a, &b, &c) != 5)
		{
			fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
				i + 1, __FILE__, __LINE__);
			free(f);//����������ͷ��ڴ�
			return -1;
		}
		//����i�������㸳ֵ
		f[i].img_pt.x = f[i].x = x;//�������x����
		f[i].img_pt.y = f[i].y = y;//�������y����
		f[i].a = a;
		f[i].b = b;
		f[i].c = c;
		f[i].d = d;
		f[i].type = FEATURE_OXFD;//����������

		//��������������
		/* read descriptor */
		for (j = 0; j < d; j++)
		{
			if (!fscanf(file, " %lf ", &dv))
			{
				fprintf(stderr, "Warning: error reading feature descriptor" \
					" #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
				free(f);//����������ͷ��ڴ�
				return -1;
			}
			f[i].descr[j] = dv;//������i��������ĵ�j������������
		}

		//����һЩûʲô�õĲ���
		f[i].scl = f[i].ori = 0;//OXFD�������޴˲���
		f[i].category = 0;
		f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
		f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
		f[i].feature_data = NULL;
	}

	//�ر��ļ�
	if (fclose(file))
	{
		fprintf(stderr, "Warning: file close error, %s, line %d\n",
			__FILE__, __LINE__);
		free(f);//����������ͷ��ڴ�
		return -1;
	}

	*features = f;//����һ���������ָ�븳��*feature
	return n;//���ض�������������
}


/*����OXFD��ʽ�������㼯���ļ�
������
filename���ļ���
feat����������
n�����������
����ֵ��0���ɹ���1��ʧ��
*/
static int export_oxfd_features(char* filename, struct feature* feat, int n)///
{
	FILE* file;
	int i, j, d;

	if (n <= 0)
	{
		fprintf(stderr, "Warning: feature count %d, %s, line %s\n",
			n, __FILE__, __LINE__);
		return 1;
	}
	//���ļ�
	if (!(file = fopen(filename, "w")))
	{
		fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
			filename, __FILE__, __LINE__);
		return 1;
	}

	d = feat[0].d;//���������ӵ�ά��
	fprintf(file, "%d\n%d\n", d, n);//����д�����������ӵ�ά�������������

	//����д��ÿ�����������Ϣ
	for (i = 0; i < n; i++)
	{
		//д������������
		fprintf(file, "%f %f %f %f %f", feat[i].x, feat[i].y, feat[i].a,
			feat[i].b, feat[i].c);
		//д��d�����������ӵ�Ԫ��
		for (j = 0; j < d; j++)
			fprintf(file, " %f", feat[i].descr[j]);
		fprintf(file, "\n");//����
	}

	//�ر��ļ�
	if (fclose(file))
	{
		fprintf(stderr, "Warning: file close error, %s, line %d\n",
			__FILE__, __LINE__);
		return 1;
	}

	return 0;
}


/*��ͼ���ϻ���OXFD���͵�������
������
img��ͼ��ָ��
feat����������
n����������
*/
static void draw_oxfd_features(IplImage* img, struct feature* feat, int n)///
{
	CvScalar color = CV_RGB(255, 255, 255);//��ɫ
	int i;

	if (img->nChannels > 1)
		color = FEATURE_OXFD_COLOR;

	//���ú��������λ���ÿ��������
	for (i = 0; i < n; i++)
		draw_oxfd_feature(img, feat + i, color);
}


/*��ͼ���ϻ�����OXFD������
������
img��ͼ��ָ��
feat��Ҫ����������
color����ɫ
*/
static void draw_oxfd_feature(IplImage* img, struct feature* feat, CvScalar color)///
{
	double m[4] = { feat->a, feat->b, feat->b, feat->c };
	double v[4] = { 0 };//��������������
	double e[2] = { 0 };//����ֵ������
	CvMat M, V, E;
	double alpha, l1, l2;

	//������Բ�����ߺͷ���
	/* compute axes and orientation of ellipse surrounding affine region */
	cvInitMatHeader(&M, 2, 2, CV_64FC1, m, CV_AUTOSTEP);//����
	cvInitMatHeader(&V, 2, 2, CV_64FC1, v, CV_AUTOSTEP);//2��2*1������������ɵľ���
	cvInitMatHeader(&E, 2, 1, CV_64FC1, e, CV_AUTOSTEP);//����ֵ
	cvEigenVV(&M, &V, &E, DBL_EPSILON, 0, 0);//��������ֵ����������
	l1 = 1 / sqrt(e[1]);
	l2 = 1 / sqrt(e[0]);
	alpha = -atan2(v[1], v[0]);
	alpha *= 180 / CV_PI;

	//����Բ��ʮ����
	cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
		0, 360, CV_RGB(0, 0, 0), 3, 8, 0);
	cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
		0, 360, color, 1, 8, 0);
	cvLine(img, cvPoint(feat->x + 2, feat->y), cvPoint(feat->x - 2, feat->y),
		color, 1, 8, 0);
	cvLine(img, cvPoint(feat->x, feat->y + 2), cvPoint(feat->x, feat->y - 2),
		color, 1, 8, 0);
}


/*���ļ��ж���LOWE������
������
filename:�ļ���
features���������������������ָ��
����ֵ����������������
*/
static int import_lowe_features(char* filename, struct feature** features)///
{
	struct feature* f;//��һ���������ָ��
	int i, j, n, d;
	double x, y, s, o, dv;
	FILE* file;

	if (!features)
		fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);

	//���ļ�
	if (!(file = fopen(filename, "r")))
	{
		fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
			filename, __FILE__, __LINE__);
		return -1;
	}

	//���ȶ������������������������ά��
	if (fscanf(file, " %d %d ", &n, &d) != 2)
	{
		fprintf(stderr, "Warning: file read error, %s, line %d\n",
			__FILE__, __LINE__);
		return -1;
	}

	//����������ά�����ڶ�������ά��������
	if (d > FEATURE_MAX_D)
	{
		fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
			__FILE__, __LINE__);
		return -1;
	}

	//�����ڴ棬n��feature�ṹ��С�������׵�ַ��f
	f = (struct feature*)calloc(n, sizeof(struct feature));

	//���ζ���n��������
	for (i = 0; i < n; i++)
	{
		//���������������(ע��x,y˳��)���߶Ⱥͷ���
		if (fscanf(file, " %lf %lf %lf %lf ", &y, &x, &s, &o) != 4)
		{
			fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
				i + 1, __FILE__, __LINE__);
			free(f);//������ͷ��ڴ�
			return -1;
		}
		//����i�������㸳ֵ
		f[i].img_pt.x = f[i].x = x;//�������x����
		f[i].img_pt.y = f[i].y = y;//�������y����
		f[i].scl = s;//������Ĵ�С��������������ݶȵ�ģֵ
		f[i].ori = o;//������ķ��򣬼���������
		f[i].d = d;//���������ӵ�ά��
		f[i].type = FEATURE_LOWE;//����

		//��������������
		/* read descriptor */
		for (j = 0; j < d; j++)
		{
			if (!fscanf(file, " %lf ", &dv))
			{
				fprintf(stderr, "Warning: error reading feature descriptor" \
					" #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
				free(f);//������ͷ��ڴ�
				return -1;
			}
			f[i].descr[j] = dv;
		}

		//����һЩûʲô�õĲ���
		f[i].a = f[i].b = f[i].c = 0;
		f[i].category = 0;
		f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
		f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
	}

	//�ر��ļ�
	if (fclose(file))
	{
		fprintf(stderr, "Warning: file close error, %s, line %d\n",
			__FILE__, __LINE__);
		free(f);//������ͷ��ڴ�
		return -1;
	}

	*features = f;//�׵�ַ����*features
	return n;//���ض�������������
}


/*����LOWE��ʽ�����㼯�ϵ��ļ�
������
filename���ļ���
feat������������
n�����������
����ֵ��0���ɹ���1��ʧ��
*/
static int export_lowe_features(char* filename, struct feature* feat, int n)///
{
	FILE* file;
	int i, j, d;

	if (n <= 0)
	{
		fprintf(stderr, "Warning: feature count %d, %s, line %s\n",
			n, __FILE__, __LINE__);
		return 1;
	}

	//���ļ�
	if (!(file = fopen(filename, "w")))
	{
		fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
			filename, __FILE__, __LINE__);
		return 1;
	}

	d = feat[0].d;//����������ά��
	fprintf(file, "%d %d\n", n, d);//����д�����������������������ά��

	//����д��ÿ�����������Ϣ
	for (i = 0; i < n; i++)
	{
		//д������������(ע��x,y˳��),�߶ȣ�����
		fprintf(file, "%f %f %f %f", feat[i].y, feat[i].x,
			feat[i].scl, feat[i].ori);
		//д������������
		for (j = 0; j < d; j++)
		{
			//ÿ��20��Ԫ��
			/* write 20 descriptor values per line */
			if (j % 20 == 0)
				fprintf(file, "\n");
			fprintf(file, " %d", (int)(feat[i].descr[j]));
		}
		fprintf(file, "\n");
	}

	//�ر��ļ�
	if (fclose(file))
	{
		fprintf(stderr, "Warning: file close error, %s, line %d\n",
			__FILE__, __LINE__);
		return 1;
	}

	return 0;
}


/*��ͼ���ϻ�LOWE������
������
img��ͼ��ָ��
feat������������
n�����������
*/
static void draw_lowe_features(IplImage* img, struct feature* feat, int n)///
{
	CvScalar color = CV_RGB(255, 255, 255);//��ɫ
	int i;

	if (img->nChannels > 1)
		color = CV_RGB(255, 255, 0);	//FEATURE_LOWE_COLOR;

	//���ú��������λ�n��������
	for (i = 0; i < n; i++)
		draw_lowe_feature(img, feat + i, color);
}


/*������LOWE������
������
img��ͼ��ָ��
feat��Ҫ����������
color����ɫ
*/
static void draw_lowe_feature(IplImage* img, struct feature* feat, CvScalar color)///
{
	int len, hlen, blen, start_x, start_y, end_x, end_y, h1_x, h1_y, h2_x, h2_y;
	double scl, ori;
	double scale = 5.0;
	double hscale = 0.75;
	CvPoint start, end, h1, h2;

	//��ͷ�˵���������
	start_x = cvRound(feat->x);
	start_y = cvRound(feat->y);

	if (g_FeatureDisplayMode)
	{
		cvCircle(img, cvPoint(start_x, start_y), 3, CV_RGB(255, 255, 255), 1, 8, 0);
		return;
	}

	scl = feat->scl;//������Ĵ�С
	ori = feat->ori;//������ķ��򣬻���
	len = cvRound(scl * scale);//��ͷ�˵ĳ���
	hlen = cvRound(scl * hscale);//��ͷ�ֲ�ĳ���
	blen = len - hlen;
	//��ͷ�˵��յ������
	end_x = cvRound(len *  cos(ori)) + start_x;
	end_y = cvRound(len * -sin(ori)) + start_y;
	//��ͷ���ҷֲ����������
	h1_x = cvRound(blen *  cos(ori + CV_PI / 18.0)) + start_x;
	h1_y = cvRound(blen * -sin(ori + CV_PI / 18.0)) + start_y;
	//��ͷ����ֲ����������
	h2_x = cvRound(blen *  cos(ori - CV_PI / 18.0)) + start_x;
	h2_y = cvRound(blen * -sin(ori - CV_PI / 18.0)) + start_y;
	start = cvPoint(start_x, start_y);//��ͷ�˵����
	end = cvPoint(end_x, end_y);//��ͷ�˵��յ�
	h1 = cvPoint(h1_x, h1_y);//��ͷ���ҷֲ�����
	h2 = cvPoint(h2_x, h2_y);//��ͷ����ֲ�����

	cvLine(img, start, end, color, 1, 8, 0);//����ͷ��
	cvLine(img, end, h1, color, 1, 8, 0);//���ҷֲ�
	cvLine(img, end, h2, color, 1, 8, 0);//����ֲ�
}