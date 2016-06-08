#include "stdafx.h"
#include "MyORB.h"

void CMyORB::ORBDescriptorExtract(cv::Mat image)
{
	m_orb.detect(image, m_keyPoint);
	m_orb.compute(image, m_keyPoint, m_descriptors);
}
void CMyORB::ORBDescriptorMatch(vector<KeyPoint> keyPoint2, cv::Mat descriptors2, vector<FeatureMatch> &orbMatchList)
{
	BFMatcher matcher(NORM_HAMMING);
	vector<Mat> descriptors;
	descriptors.push_back(m_descriptors);
	matcher.add(descriptors);
	vector<vector<DMatch>> matches, goodmatches;
	DMatch bestMatch, betterMatch;
	
	orbMatchList.clear();
	FeatureMatch tempORBFeat;
	matcher.knnMatch(descriptors2, matches, 2);

	int n = 0;
	Point p1, p2;
	for (int i = 0; i < (int)matches.size(); i++)
	{
		bestMatch = matches[i][0];
		betterMatch = matches[i][1];
		p1 = m_keyPoint[bestMatch.trainIdx].pt;
		p2 = keyPoint2[bestMatch.queryIdx].pt;

		double distance = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
		float distanceRatio = bestMatch.distance / betterMatch.distance;
	
		if (distanceRatio< 0.8 && distance<100)
		{
			tempORBFeat.pnt = p1;
			tempORBFeat.matchPoint = p2;
			orbMatchList.push_back(tempORBFeat);
		}
	}
}