#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
#include <vector>
using namespace std;

//SIFT算法头文件

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
	//在k-d树上进行BBF搜索的最大次数
#define KDTREE_BBF_MAX_NN_CHKS 200

	//目标点与最近邻和次近邻的距离的比值的阈值，若大于此阈值，则剔除此匹配点对
	//通常此值取0.6，值越小找到的匹配点对越精确，但匹配数目越少
#define NN_SQ_DIST_RATIO_THR 0.49
public:
	struct feature *m_feat;
	int m_featNum;
public:
	void SiftFeatureExtract(IplImage *image);
	void DrawSiftFeature(IplImage *imgFeat);
	void SiftFeatureMatch(feature *feat1, feature *feat2, int nFeat1Num, int nFeat2Num, vector<FeatureMatch> &matchFeat);
};