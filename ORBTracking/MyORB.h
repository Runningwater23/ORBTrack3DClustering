#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <windows.h>

using namespace cv;
#include <vector>
using namespace std;

	struct FeatureMatch
	{
		Point2f pnt;
		Point2f matchPoint;
	};

class CMyORB
{
public:

public:
	ORB m_orb;
	cv::Mat m_descriptors;
	vector<KeyPoint> m_keyPoint;
public:
	void ORBDescriptorExtract(cv::Mat image);
	void ORBDescriptorMatch(vector<KeyPoint> keyPoint2, cv::Mat descriptors2, vector<FeatureMatch> &bestMatches);
};