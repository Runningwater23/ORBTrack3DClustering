#include "stdafx.h"
#include "MySift.h"

void CMySift::SiftFeatureExtract(IplImage *image)
{
	IplImage *img;
	img = cvCreateImage(cvSize(image->width / pow(2.0, 0/*dlg->m_DownSampleTime*/), image->height / pow(2.0, 0/*dlg->m_DownSampleTime*/)), image->depth, image->nChannels);
	cvResize(image, img, CV_INTER_NN);//尺寸变换

	m_featNum = sift_features(img, &m_feat);//检测图1中的SIFT特征点,n1是图1的特征点个数 
	cvReleaseImage(&img);
}
void CMySift::DrawSiftFeature(IplImage *imgFeat)
{
	draw_features(imgFeat, m_feat, m_featNum);//画出特征点
}
void CMySift::SiftFeatureMatch(feature *feat1, feature *feat2, int nFeat1Num, int nFeat2Num, vector<FeatureMatch> &matchFeat)
{
	struct feature *feat;//每个特征点
	struct kd_node *kd_root;//k-d树的树根
	struct feature **nbrs;//当前特征点的最近邻点数组
	kd_root = kdtree_build(feat1, nFeat1Num);
	FeatureMatch featMatch;

	double d0, d1;//feat2中每个特征点到最近邻和次近邻的距离
	matchFeat.clear();
	for (int i = 0; i < nFeat2Num; i++)
	{
		feat = feat2 + i;//第i个特征点的指针
		//在kd_root中搜索目标点feat的2个最近邻点，存放在nbrs中，返回实际找到的近邻点个数
		int k = kdtree_bbf_knn(kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS);
		if (k == 2)
		{
			d0 = descr_dist_sq(feat, nbrs[0]);//feat与最近邻点的距离的平方
			d1 = descr_dist_sq(feat, nbrs[1]);//feat与次近邻点的距离的平方
			//若d0和d1的比值小于阈值NN_SQ_DIST_RATIO_THR，则接受此匹配，否则剔除
			if (d0 < d1 * NN_SQ_DIST_RATIO_THR)
			{   //将目标点feat和最近邻点作为匹配点对
				feat2[i].fwd_match = nbrs[0];//使点feat的fwd_match域指向其对应的匹配点
				featMatch.pnt.x = feat2[i].fwd_match->x;
				featMatch.pnt.y = feat2[i].fwd_match->y;
				featMatch.matchPoint.x = feat2[i].x;
				featMatch.matchPoint.y = feat2[i].y;
				matchFeat.push_back(featMatch);
			}
		}
		free(nbrs);//释放近邻数组
	}
}