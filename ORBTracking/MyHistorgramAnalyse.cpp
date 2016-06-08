#include "stdafx.h"
#include "MyHistorgramAnalyse.h"

CMyHistorgram::CMyHistorgram()
{
	xMinValue = 0;
	xMaxValue = 100;
	yMinValue = 0;
	yMaxValue = 100;

	xWidth = 100;
	yHeight = 100;

	xUnit = 1;
	yUnit = 1;

	m_bShowEXDX = false;
}

CMyHistorgram::~CMyHistorgram()
{

}

bool CMyHistorgram::Create(UINT nID, CWnd *parent)
{
	m_nID = nID;
	m_mainDlg = parent;

	(m_mainDlg->GetDlgItem(m_nID))->GetClientRect(&m_rect);

	m_pWnd = m_mainDlg->GetDlgItem(m_nID);
	m_pDC = m_pWnd->GetDC();

	m_font.CreatePointFont(80, _T("宋体"));
	//PS_SOLID：实线
	//PS_DOT：点线
	//PS_DASH：划线
	//PS_DASHDOT：点划线
	//PS_DASHDOTDOT：一划两点的点划线
	//PS_NULL：透明线
	//PS_INSIDEFRAME：边框内沿线
	m_penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_penGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_penBlue.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_penBlack.CreatePen(PS_DOT, 1, RGB(0, 0, 0));

	xRectWidth = m_rect.Width();
	yRectHeight = m_rect.Height();

	return true;
}

void  CMyHistorgram::SetLimit(float xMin, float xMax, float yMin, float yMax)
{
	xMinValue = xMin;
	xMaxValue = xMax;
	yMinValue = yMin;
	yMaxValue = yMax;

	xWidth = xMax - xMin;
	yHeight = yMax - yMin;

	xUnit = 1.0*xWidth / xRectWidth;
	yUnit = 1.0*yHeight / yRectHeight;
}
void CMyHistorgram::DrawCoordSystem()
{
	m_pDC->FillSolidRect(&m_rect, RGB(255, 255, 255));

	CFont*def_font = m_pDC->SelectObject(&m_font);  //选择该字体进入PDC

	int xSpatal = xRectWidth / 10;
	int ySpatal = yRectHeight / 10;

	m_pDC->SelectObject(&m_penBlack);
	CString tempStr;
	m_pDC->MoveTo(m_rect.left, m_rect.bottom);
	m_pDC->LineTo(m_rect.left, m_rect.top);
	m_pDC->MoveTo(m_rect.left, m_rect.bottom);
	m_pDC->LineTo(m_rect.right, m_rect.bottom);
	m_pDC->TextOut(m_rect.left, m_rect.bottom, _T("Min"));
	for (int i = m_rect.bottom - ySpatal; i > m_rect.top; i -= ySpatal)
	{
		m_pDC->MoveTo(m_rect.left, i);
		m_pDC->LineTo(m_rect.right, i);
		tempStr.Format(_T("%d"), (int)(yMinValue + (m_rect.bottom - i)*yUnit));
		m_pDC->TextOut(m_rect.left, i - 5, tempStr);
	}
	for (int i = m_rect.left + xSpatal; i < m_rect.right; i += xSpatal)
	{
		m_pDC->MoveTo(i, m_rect.bottom);
		m_pDC->LineTo(i, m_rect.top);
		tempStr.Format(_T("%d"), (int)(xMinValue + i*xUnit));
		m_pDC->TextOut(i - 10, m_rect.bottom - 10, tempStr);
	}

	//m_pDC->SelectObject(def_font);//恢复PDC的缺省字体
	//m_font.DeleteObject();//释放font对象
}
void CMyHistorgram::DrawIntegralHistogram(vector<CPoint> pointList, int lineStyle, int lineWidth, COLOR color)
{
	integralSum = 0;
	CPoint tempPoint;
	m_integralPointList.clear();
	for (int i = 0; i < pointList.size(); i++)
	{
		integralSum += pointList[i].y;
		tempPoint.x = pointList[i].x;
		tempPoint.y = integralSum;
		m_integralPointList.push_back(tempPoint);
	}
	CFont*def_font = m_pDC->SelectObject(&m_font);  //选择该字体进入PDC

	int xSpatal = xRectWidth / 10;
	int ySpatal = yRectHeight / 10;

	m_pDC->SelectObject(&m_penBlack);
	CString tempStr;
	integralYUnit = 1.0*integralSum / yRectHeight;
	for (int i = m_rect.bottom - ySpatal; i > m_rect.top; i -= ySpatal)
	{
		tempStr.Format(_T("%d"), (int)(yMinValue + (m_rect.bottom - i)* integralYUnit));
		m_pDC->TextOut(m_rect.right - xSpatal, i - 5, tempStr);
	}
	CPen newPen;
	newPen.CreatePen(lineStyle, lineWidth, color);
	m_pDC->SelectObject(&newPen);
	if (m_integralPointList.size())
	{
		tempPoint.x = (m_integralPointList[0].x - xMinValue) / xUnit;
		tempPoint.y = m_rect.bottom - (m_integralPointList[0].y - yMinValue) / integralYUnit;
		m_pDC->MoveTo(tempPoint);
	}
	for (int i = 1; i < m_integralPointList.size(); i++)
	{
		tempPoint.x = (m_integralPointList[i].x - xMinValue) / xUnit;
		tempPoint.y = m_rect.bottom - (m_integralPointList[i].y - yMinValue) / integralYUnit;
		m_pDC->LineTo(tempPoint);
	}
	float closeDegree = 0;
	for (int i = 0; i < m_integralPointList.size(); i++)
	{
		closeDegree = 1.0*m_integralPointList[i].y / integralSum - 0.15;
		if (closeDegree >= 0)
		{
			m_15PercentIndex = m_integralPointList[i].x;
			break;
		}
	}
	for (int i = m_integralPointList.size() - 1; i; i--)
	{
		closeDegree = 1.0*m_integralPointList[i].y / integralSum - 0.85;
		if (closeDegree <= 0)
		{
			m_85PercentIndex = m_integralPointList[i].x;
			break;
		}
	}
	m_pDC->MoveTo((m_15PercentIndex - xMinValue) / xUnit, m_rect.bottom);
	m_pDC->LineTo((m_15PercentIndex - xMinValue) / xUnit, m_rect.top);
	m_pDC->MoveTo((m_85PercentIndex - xMinValue) / xUnit, m_rect.bottom);
	m_pDC->LineTo((m_85PercentIndex - xMinValue) / xUnit, m_rect.top);
}
void CMyHistorgram::RenderScene()
{
	DrawCoordSystem();

	DrawPoint(m_pointList, RGB(255, 0, 0), 3);
	DrawLine(m_pointList, PS_DOT, 2, RGB(0, 255, 0));
	//DrawIntegralHistogram(m_pointList, PS_DOT, 2, RGB(255, 0, 0));
}
void CMyHistorgram::DrawPoint(vector<CPoint> pointList, COLOR color, int nRadius)
{
	CBrush brush(color);
	m_pDC->SelectObject(brush);
	CPoint point;
	for (int i = 0; i < pointList.size(); i++)
	{
		point.x = (pointList[i].x - xMinValue) / xUnit;
		point.y = m_rect.bottom - (pointList[i].y - yMinValue) / yUnit;
		m_pDC->Ellipse(point.x - nRadius, point.y - nRadius, point.x + nRadius, point.y + nRadius);
	}
}
void CMyHistorgram::DrawPoint(CPoint point, COLOR color, int nRadius)
{
	CBrush brush(color);
	m_pDC->SelectObject(brush);
	CPoint tempPoint;

	tempPoint.x = (point.x - xMinValue) / xUnit;
	tempPoint.y = m_rect.bottom - (point.y - yMinValue) / yUnit;
	m_pDC->Ellipse(tempPoint.x - nRadius, tempPoint.y - nRadius, tempPoint.x + nRadius, tempPoint.y + nRadius);

}
void CMyHistorgram::DrawLine(vector<CPoint> pointList, int lineStyle, int lineWidth, COLOR color)
{
	CPen newPen;
	newPen.CreatePen(lineStyle, lineWidth, color);
	m_pDC->SelectObject(&newPen);
	CPoint point;
	int mode;
	if (pointList.size())
	{
		point.x = (pointList[0].x - xMinValue) / xUnit;
		point.y = m_rect.bottom - (pointList[0].y - yMinValue) / yUnit;
		m_pDC->MoveTo(point);

		mode = pointList[0].y;
		m_modeIndex = pointList[0].x;
	}
	for (int i = 1; i < pointList.size(); i++)
	{
		point.x = (pointList[i].x - xMinValue) / xUnit;
		point.y = m_rect.bottom - (pointList[i].y - yMinValue) / yUnit;
		m_pDC->LineTo(point);

		if (pointList[i].y>mode)
		{
			mode = pointList[i].y;
			m_modeIndex = pointList[i].x;
		}
	}
	if (m_bShowEXDX)
	{
		if (pointList.size())
		{
			//m_pDC->MoveTo((m_modeIndex - xMinValue) / xUnit, m_rect.bottom);
			//m_pDC->LineTo((m_modeIndex - xMinValue) / xUnit, m_rect.top);

			m_meanValue = GetMeanValue(pointList);
			m_variance = GetVariance(pointList, m_meanValue);

			CFont font;
			font.CreatePointFont(160, _T("宋体"));
			CFont*def_font = m_pDC->SelectObject(&font);  //选择该字体进入PDC

			CString tempStr;
			tempStr.Format(_T("EX = %.2f"), m_meanValue);
			m_pDC->TextOut(m_rect.left + 40, m_rect.top + 20, tempStr);
			tempStr.Format(_T("DX = %.2f"), m_variance);
			m_pDC->TextOut(m_rect.left + 40, m_rect.top + 60, tempStr);

			m_pDC->SelectObject(def_font);//恢复PDC的缺省字体
			font.DeleteObject();//释放font对象
		}
	}

	
}

void CMyHistorgram::DrawLine(CPoint point1, CPoint point2, int lineStyle, int lineWidth, COLOR color)
{
	CPen newPen;
	newPen.CreatePen(lineStyle, lineWidth, color);
	m_pDC->SelectObject(&newPen);
	CPoint point;
	point.x = (point1.x - xMinValue) / xUnit;
	point.y = m_rect.bottom - (point1.y - yMinValue) / yUnit;
	m_pDC->MoveTo(point);
	point.x = (point2.x - xMinValue) / xUnit;
	point.y = m_rect.bottom - (point2.y - yMinValue) / yUnit;
	m_pDC->LineTo(point);
}
float CMyHistorgram::GetMeanValue(vector<CPoint> pointList)
{
	int instanceCount = 0;
	double sum = 0;
	for (int i = 0; i < pointList.size(); i++)
	{
		//sum += pointList[i].x*pointList[i].y;
		//instanceCount += pointList[i].y;
		sum += pointList[i].y;
		instanceCount++;
	}
	float meanValue = sum / instanceCount;
	return meanValue;
}
float CMyHistorgram::GetVariance(vector<CPoint> pointList, float meanValue)
{
	int instanceCount = 0;
	double sum = 0;
	for (int i = 0; i < pointList.size(); i++)
	{
		//sum += pointList[i].y * pow((pointList[i].x - meanValue), 2);
		//instanceCount += pointList[i].y;
		sum += pow((pointList[i].y - meanValue), 2);
		instanceCount++;
	}
	sum /= instanceCount;
	float variance = sum;// pow((sum), 0.5);
	return variance;
}