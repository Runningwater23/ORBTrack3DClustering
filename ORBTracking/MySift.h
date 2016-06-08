#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
#include <vector>
using namespace std;

//SIFT�㷨ͷ�ļ�

#include "imgfeatures.h"
#include "kdtree.h"
#include "minpq.h"
#include "dsift.h"
#include "utils.h"
#include "xform.h"

#include "MyORB.h"

class CMySift
{
public:
	//��k-d���Ͻ���BBF������������
#define KDTREE_BBF_MAX_NN_CHKS 200

	//Ŀ���������ںʹν��ڵľ���ı�ֵ����ֵ�������ڴ���ֵ�����޳���ƥ����
	//ͨ����ֵȡ0.6��ֵԽС�ҵ���ƥ����Խ��ȷ����ƥ����ĿԽ��
#define NN_SQ_DIST_RATIO_THR 0.49
public:
	struct feature *m_feat;
	int m_featNum;
public:
	void SiftFeatureExtract(IplImage *image);
	void DrawSiftFeature(IplImage *imgFeat);
	void SiftFeatureMatch(feature *feat1, feature *feat2, int nFeat1Num, int nFeat2Num, vector<FeatureMatch> &matchFeat);
};