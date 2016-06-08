#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
using namespace cv;
using namespace std;

#include "MyOpenGL.h"

class CKMEANS
{
public:
	void DoKMEANS(vector<POINT3D> pointList,int clusterNum,vector<int> &clusterMarkList);
};
inline void CKMEANS::DoKMEANS(vector<POINT3D> pointList, int clusterNum, vector<int> &clusterMarkList)
{
	int dataNum = pointList.size();
	if (dataNum)
	{
		Mat points(dataNum, 3, CV_32F), labels;
		int clusterCount = MIN(dataNum, clusterNum);
		Mat centers(clusterCount, 3, points.type());
		for (int i = 0; i < dataNum; i++)
		{
			points.at<float>(i,0) = pointList[i].X;
			points.at<float>(i, 1) = pointList[i].Y;
			points.at<float>(i, 2) = pointList[i].Z;
		}
		kmeans(points, clusterCount, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);
	
		clusterMarkList.clear();
		int clusterID;
		for (int i = 0; i < dataNum; i++)
		{
			clusterID = labels.at<int>(i);
			clusterMarkList.push_back(clusterID);
		}
	}

}