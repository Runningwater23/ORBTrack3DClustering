#include "stdafx.h"
#include "MySift.h"

void CMySift::SiftFeatureExtract(IplImage *image)
{
	IplImage *img;
	img = cvCreateImage(cvSize(image->width / pow(2.0, 0/*dlg->m_DownSampleTime*/), image->height / pow(2.0, 0/*dlg->m_DownSampleTime*/)), image->depth, image->nChannels);
	cvResize(image, img, CV_INTER_NN);//�ߴ�任

	m_featNum = sift_features(img, &m_feat);//���ͼ1�е�SIFT������,n1��ͼ1����������� 
	cvReleaseImage(&img);
}
void CMySift::DrawSiftFeature(IplImage *imgFeat)
{
	draw_features(imgFeat, m_feat, m_featNum);//����������
}
void CMySift::SiftFeatureMatch(feature *feat1, feature *feat2, int nFeat1Num, int nFeat2Num, vector<FeatureMatch> &matchFeat)
{
	struct feature *feat;//ÿ��������
	struct kd_node *kd_root;//k-d��������
	struct feature **nbrs;//��ǰ�����������ڵ�����
	kd_root = kdtree_build(feat1, nFeat1Num);
	FeatureMatch featMatch;

	double d0, d1;//feat2��ÿ�������㵽����ںʹν��ڵľ���
	matchFeat.clear();
	for (int i = 0; i < nFeat2Num; i++)
	{
		feat = feat2 + i;//��i���������ָ��
		//��kd_root������Ŀ���feat��2������ڵ㣬�����nbrs�У�����ʵ���ҵ��Ľ��ڵ����
		int k = kdtree_bbf_knn(kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS);
		if (k == 2)
		{
			d0 = descr_dist_sq(feat, nbrs[0]);//feat������ڵ�ľ����ƽ��
			d1 = descr_dist_sq(feat, nbrs[1]);//feat��ν��ڵ�ľ����ƽ��
			//��d0��d1�ı�ֵС����ֵNN_SQ_DIST_RATIO_THR������ܴ�ƥ�䣬�����޳�
			if (d0 < d1 * NN_SQ_DIST_RATIO_THR)
			{   //��Ŀ���feat������ڵ���Ϊƥ����
				feat2[i].fwd_match = nbrs[0];//ʹ��feat��fwd_match��ָ�����Ӧ��ƥ���
				featMatch.pnt.x = feat2[i].fwd_match->x;
				featMatch.pnt.y = feat2[i].fwd_match->y;
				featMatch.matchPoint.x = feat2[i].x;
				featMatch.matchPoint.y = feat2[i].y;
				matchFeat.push_back(featMatch);
			}
		}
		free(nbrs);//�ͷŽ�������
	}
}