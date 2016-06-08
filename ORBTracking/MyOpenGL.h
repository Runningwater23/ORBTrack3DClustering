#pragma once

#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <glut.h>
#include <vector>
using namespace std;

struct POINT3D
{
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
};
class GLRGBColor3f
{
public:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLRGBColor3f()
	{
		r = 1.0;
		g = 1.0;
		b = 1.0;
	}
	GLRGBColor3f(GLfloat rr, GLfloat gg, GLfloat bb)
	{
		r = rr;
		g = gg;
		b = bb;
	}
};
const double PI = 3.14159;
class CMyOpenGL : public CWnd
{
public:
	CMyOpenGL();
	~CMyOpenGL();
public:
	float circle_R;
	float m_theta;//«Ú√Ê◊¯±Í”ÎZ÷·º–Ω«(YOZ)
	float m_phi;  //«Ú√Ê◊¯±Í”ÎX÷·º–Ω«(XOZ)

	int angle_Z;
	int angle_X;
	int angle_Y;
	float origin_X;
	float origin_Y;
	float origin_Z;
	float eye_X;
	float eye_Y;
	float eye_Z;
	float center_X;
	float center_Y;
	float center_Z;
	float up_X;
	float up_Y;
	float up_Z;

	float xMinValue;
	float xMaxValue;
	float yMinValue;
	float yMaxValue;
	float zMinValue;
	float zMaxValue;
	float xLength;
	float yWidth;
	float zHeight;

	float m_xyScale;
	float m_zScale;

	float xLineLength;
	float yLineLength;
	float zLineLength;

	int PixelFormat;

	unsigned char *m_imageData;
	int m_imageRows;
	int m_imageCols;

	vector<POINT3D> m_3DPointList;
	vector<int> m_pointIDList;
public:
	bool Create(UINT nID, CWnd *parent);
	void InitLimit(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	void SetWatchAngle(float circleR,float theta, float phi);
	void DrawCoordSystem(float lineWidth);
	void DrawGrid(float lineWidth);
	void RenderScene(vector<vector<POINT3D>> point3DList);
	void RenderScene();
	////////////////////
	void DrawCube();
	POINT3D ScaleTransformation(POINT3D point, float xyScale, float zScale);
	void DrawPoint(POINT3D point, GLfloat pointSize,GLRGBColor3f GLcolor);
	void DrawPoint(vector<POINT3D> pointList, GLfloat pointSize, GLRGBColor3f GLcolor);
	void DrawPoint(vector<POINT3D> pointList, vector<int> pointIDList, GLfloat pointSize);
	void DrawLine(vector<POINT3D> pointList, GLfloat lineWidth, GLenum lineType, GLRGBColor3f GLcolor);
	void DrawLine(POINT3D pnt1, POINT3D pnt2, GLfloat lineWidth, GLenum lineType, GLRGBColor3f GLcolor);
	void DrawMatrix(unsigned char *data, int rows, int cols, GLfloat lineWidth, GLenum lineType);
	void DrawMatrix(unsigned char *data, int rows, int cols, int step);
	void DrawPlane(vector<POINT3D> pointList, GLRGBColor3f GLcolor);

//protected:
public:
	UINT m_nID;
	CWnd *m_parent;
	bool m_bInit;
	BOOL m_bFullScreen;
	CRect m_rect;
	CWnd *m_pWnd;
	DEVMODE m_DMsaved;
	HDC m_hRenderDC;
	HGLRC m_hRenderRC;  //RC
	bool InitGL();
	void KillGLWindow();
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);     //View GL Context
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CPoint tempPoint;
	bool m_dropFlag;
	void OnLButtonDown(CPoint point);
	void OnLButtonUp(CPoint point);
	void OnMouseMove(CPoint point);
	void OnMouseWheel(short zDelta);
	void OnKeyDown(char key);
};