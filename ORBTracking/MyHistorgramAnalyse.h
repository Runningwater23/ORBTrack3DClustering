#pragma once

#include "stdafx.h"
#include <vector>
using namespace std;

typedef DWORD COLOR;

class CMyHistorgram : public CWnd
{
public:

   const float PI = 3.14159;

	CMyHistorgram();
	~CMyHistorgram();

public:

	vector<CPoint> m_pointList;
	vector<CPoint> m_integralPointList;
	float xMinValue;
	float xMaxValue;
	float yMinValue;
	float yMaxValue;

	float xWidth;
	float yHeight;

	int xRectWidth;
	int yRectHeight;

	float xUnit;
	float yUnit;
	float integralYUnit;
	float integralSum;

	int m_15PercentIndex;
	int m_85PercentIndex;
	int m_modeIndex;

	float m_meanValue;
	float m_variance;

	bool m_bShowEXDX;

public:

	bool Create(UINT nID, CWnd *parent);
	void SetLimit(float xMin, float xMax, float yMin, float yMax);
	void DrawCoordSystem();
	void RenderScene();
	void DrawPoint(vector<CPoint> pointList,COLOR color,int nRadius);
	void DrawPoint(CPoint point, COLOR color, int nRadius);
	void DrawLine(vector<CPoint> pointList, int lineStyle, int lineWidth, COLOR color);
	void DrawLine(CPoint point1, CPoint point2, int lineStyle, int lineWidth, COLOR color);
	void DrawIntegralHistogram(vector<CPoint> pointList, int lineStyle, int lineWidth, COLOR color);
	float GetMeanValue(vector<CPoint> pointList);
	float GetVariance(vector<CPoint> pointList, float meanValue);

protected:
	UINT m_nID;
	CWnd *m_mainDlg;
	CRect m_rect;
	CWnd *m_pWnd;
	CDC *m_pDC;
	CFont m_font;
	CPen m_penRed;
	CPen m_penGreen;
	CPen m_penBlue;
	CPen m_penBlack;
};