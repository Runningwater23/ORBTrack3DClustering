
#include "stdafx.h"
#include "MyOpenGL.h"

CMyOpenGL::CMyOpenGL() :m_bInit(FALSE), m_bFullScreen(FALSE),
m_hRenderDC(NULL), m_hRenderRC(NULL), m_pWnd(NULL)
{
	PixelFormat = 0;
	circle_R = 3;
	m_theta = 30;
	m_phi = 30;
	angle_Z = 0;
	angle_X = 0;
	angle_Y = 0;
	origin_X = 0;
	origin_Y = 0;
	origin_Z = 0;
	eye_X = 1;
	eye_Y = 1;
	eye_Z = 1;
	center_X = 0;
	center_Y = 0;
	center_Z = 0;
	up_X = 0;
	up_Y = 0;
	up_Z = 1;

	m_imageData = NULL;
	m_imageRows = 0;
	m_imageCols = 0;
}
CMyOpenGL::~CMyOpenGL()
{

}

BEGIN_MESSAGE_MAP(CMyOpenGL, CWnd)
	//{{AFX_MSG_MAP(COpenGL)
	ON_WM_CREATE()
	//ON_WM_PAINT()
	//ON_WM_SIZE()
	//ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	//ON_WM_KEYDOWN()
END_MESSAGE_MAP()

int CMyOpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &m_DMsaved);


	GLuint PixelFormat;            // Holds The Results After Searching For A Match
	static PIXELFORMATDESCRIPTOR pfd =         // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),         // Size Of This Pixel Format Descriptor
		1,                // Version Number
		PFD_DRAW_TO_WINDOW |           // Format Must Support Window
		PFD_SUPPORT_OPENGL |           // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,            // Must Support Double Buffering
		PFD_TYPE_RGBA,             // Request An RGBA Format
		m_DMsaved.dmBitsPerPel,            // Select Our Color Depth
		0, 0, 0, 0, 0, 0,            // Color Bits Ignored
		0,                // No Alpha Buffer
		0,                // Shift Bit Ignored
		0,                // No Accumulation Buffer
		0, 0, 0, 0,              // Accumulation Bits Ignored
		16,                // 16Bit Z-Buffer (Depth Buffer) 
		0,                // No Stencil Buffer
		0,                // No Auxiliary Buffer
		PFD_MAIN_PLANE,             // Main Drawing Layer
		0,                // Reserved
		0, 0, 0               // Layer Masks Ignored
	};
	/*

	if (!(m_hDC = ::GetDC(m_hWnd))) {       // Did We Get A Device Context?                 
		KillGLWindow();             // Reset The Display
		TRACE("Can't Create A GL Device Context.");
		return FALSE;
	}*/

	if (!(PixelFormat = ChoosePixelFormat(m_hRenderDC, &pfd))) {  // Did Windows Find A Matching Pixel Format?                  
		KillGLWindow();            // Reset The Display
		TRACE("Can't Find A Suitable PixelFormat.");
		return FALSE;
	}

	if (!SetPixelFormat(m_hRenderDC, PixelFormat, &pfd)){    // Are We Able To Set The Pixel Format?                  
		KillGLWindow();            // Reset The Display
		TRACE("Can't Set The PixelFormat.");
		return FALSE;
	}

	if (!(m_hRenderRC = wglCreateContext(m_hRenderDC))) {     // Are We Able To Get A Rendering Context?                 
		KillGLWindow();            // Reset The Display
		TRACE(" Can't Create A GL Rendering Context.");
		return FALSE;
	}

	if (!wglMakeCurrent(m_hRenderDC, m_hRenderRC)) {       // Try To Activate The Rendering Context                  
		KillGLWindow();            // Reset The Display
		TRACE("Can't Activate The GL Rendering Context.");
		return FALSE;
	}

	if (!InitGL()) {             // Initialize Our Newly Created GL Window                  
		KillGLWindow();            // Reset The Display
		TRACE("Initialization Failed.");
		return FALSE;
	}
	m_bInit = TRUE;

	return 0;
}
bool CMyOpenGL::InitGL()
{
	return true;
}

void CMyOpenGL::KillGLWindow()
{
	if (m_bFullScreen)               // Are We In Fullscreen Mode?
	{
		if (!ChangeDisplaySettings(NULL, CDS_TEST)) {      // if the shortcut doesn't work
			ChangeDisplaySettings(NULL, CDS_RESET);       // Do it anyway (to get the values out of the registry)
			ChangeDisplaySettings(&m_DMsaved, CDS_RESET);      // change it to the saved settings
		}
		else {
			ChangeDisplaySettings(NULL, CDS_RESET);
		}

		ShowCursor(TRUE);             // Show Mouse Pointer
	}

	if (m_hRenderRC) {               // Do We Have A Rendering Context?                  
		if (!wglMakeCurrent(NULL, NULL)) {       // Are We Able To Release The DC And RC Contexts?
			TRACE("Release Of DC And RC Failed.");
		}

		if (!wglDeleteContext(m_hRenderRC)) {        // Are We Able To Delete The RC?
			TRACE("Release Rendering Context Failed.");
		}
		m_hRenderRC = NULL;              // Set RC To NULL
	}

	if (m_hRenderDC && !::ReleaseDC(m_hWnd, m_hRenderDC)) {      // Are We Able To Release The DC
		TRACE("Release Device Context Failed.");
		m_hRenderDC = NULL;              // Set DC To NULL
	}

	if (m_hWnd && !::DestroyWindow(m_hWnd)) {       // Are We Able To Destroy The Window?
		TRACE("Could Not Release m_hWnd.");
		m_hWnd = NULL;              // Set m_hWnd To NULL
	}
}
bool CMyOpenGL::Create(UINT nID, CWnd *parent)
{
	m_nID = nID;
	m_parent = parent;

	m_pWnd = parent->GetDlgItem(nID);
	m_hRenderDC = ::GetDC(m_pWnd->GetSafeHwnd());
	if (SetWindowPixelFormat(m_hRenderDC) == FALSE)
		return 0;

	if (CreateViewGLContext(m_hRenderDC) == FALSE)
		return 0;
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	///////////////////////////////////////////
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	//CRect rect;
	parent->GetDlgItem(nID)->GetWindowRect(m_rect);

	glViewport(0, 0, m_rect.Width(), m_rect.Height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, 1, 0.1, 100.0);
	gluPerspective(45.0f, (float)m_rect.Width() / (float)m_rect.Height(), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
	glClearDepth(1.0f);         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);       // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	/////////////////////////////////////////////////////////////////////////
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	return true;
}
BOOL CMyOpenGL::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
	if (PixelFormat == 0) // Choose default
	{
		PixelFormat = 1;
		if (DescribePixelFormat(hDC, PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}
	if (SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
void CMyOpenGL::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	::ValidateRect(m_hWnd, NULL);

	// Do not call CWnd::OnPaint() for painting messages
}


void CMyOpenGL::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (cy == 0) {              // Prevent A Divide By Zero By                 
		cy = 1;               // Making Height Equal One
	}
	/*
	glViewport(0,0,cx,cy);           // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);           // Select The Projection Matrix
	glLoadIdentity();              // Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)cx/(GLfloat)cy,0.1f,100.0f);  // Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);            // Select The Modelview Matrix
	glLoadIdentity();              // Reset The Modelview Matrix
	*/
}
BOOL CMyOpenGL::CreateViewGLContext(HDC hDC)
{
	m_hRenderRC = wglCreateContext(hDC);
	if (m_hRenderRC == NULL)
		return FALSE;
	if (wglMakeCurrent(hDC, m_hRenderRC) == FALSE)
		return FALSE;
	return TRUE;
}
void CMyOpenGL::InitLimit(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	xMinValue = xMin;
	xMaxValue = xMax;
	yMinValue = yMin;
	yMaxValue = yMax;
	zMinValue = zMin;
	zMaxValue = zMax;
	xLength = xMax - xMin;
	yWidth = yMax - yMin;
	zHeight = zMax - zMin;

	xLineLength = 1.0;
	yLineLength = 2 * yWidth / xLength - 1;
	zLineLength = 0.2;

	m_xyScale = 2.0 / xLength;
	m_zScale = 1.2 / zHeight;
}
void CMyOpenGL::SetWatchAngle(float circleR,float theta, float phi)
{
	m_theta = theta;
	m_phi = phi;
	eye_X = circleR*cos(theta*PI / 180);
	eye_Y = circleR*sin(theta*PI / 180)*sin(phi*PI / 180);
	eye_Z = circleR*sin(theta*PI / 180)*cos(phi*PI / 180);
	
}
void CMyOpenGL::DrawCoordSystem(float lineWidth)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eye_X, eye_Y, eye_Z, center_X, center_Y, center_Z, up_X, up_Y, up_Z);

	glTranslatef(origin_X, origin_Y, origin_Z);      // Move Left 1.5 Units And Into The Screen 6.0

	//glRotated(angle_Z, 0.0, 0.0, 1.0);
	//glRotated(angle_X, 1.0, 0.0, 0.0);
	//glRotated(angle_Y, 0.0, 1.0, 0.0);
	
	glColor3f(1.0f, 0.0f, 0.0f);//x轴
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex3f(xLineLength*1.2, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glPushMatrix();
	glTranslatef(1.2, -1.0f, -1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glutWireCone(0.027, 0.09, 10, 10);
	glPopMatrix();
	
	glColor3f(0.0f, 1.0f, 0.0f);//y轴
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex3f(-1.0f, yLineLength*1.2, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glPushMatrix();
	glTranslatef(-1.0, yLineLength*1.2, -1.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutWireCone(0.027, 0.09, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 0.0f, 1.0f);//z轴
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex3f(-1.0f, -1.0f, zLineLength*1.2);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glPushMatrix();
	glTranslatef(-1.0f, -1.0f, zLineLength*1.2);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutWireCone(0.027, 0.09, 10, 10);
	glPopMatrix();
	
}
void CMyOpenGL::DrawGrid(float lineWidth)
{
	glPushMatrix();
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
    glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
	for (float i = -1+0.1; i <= xLineLength; i += 0.1)
	{
		
		glVertex3f(i, -1.0, -1.0);
		glVertex3f(i, yLineLength, -1.0);

	}
	for (float i = -1 + 0.1; i <= yLineLength; i += 0.1)
	{
		glVertex3f(xLineLength, i, -1.0);
		glVertex3f( -1.0,i, -1.0);
	}
	glEnd();
	glPopMatrix();
}
void CMyOpenGL::OnLButtonDown(CPoint point)
{
	CRect rect;
	m_parent->GetDlgItem(m_nID)->GetWindowRect(&rect);
	m_parent->ClientToScreen(&point);
	if (point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom)
	{
		tempPoint = point;
		m_dropFlag = true;
	}
}
void CMyOpenGL::OnLButtonUp(CPoint point)
{
	m_dropFlag = false;
}
void CMyOpenGL::OnKeyDown(char key)
{
	//KeyLeft ascii 37
		//KeyRight ascii 39
		//KeyUp ascii 38
		//KeyDown ascii 40
	switch (key)
	{
	case 40:
		origin_X += 0.1;
		break;
	case 38:
		origin_X -= 0.1;
	case 39:
		origin_Y += 0.1;
		break;
	case 37:
		origin_Y -= 0.1;
	default:break;
	}
}
void CMyOpenGL::OnMouseWheel(short zDelta)
{
	if (zDelta < 0)  circle_R -= 0.1;
	else circle_R += 0.1;
	SetWatchAngle(circle_R, m_theta, m_phi);
}
void CMyOpenGL::OnMouseMove(CPoint point)
{
	CRect rect;
	m_parent->GetDlgItem(m_nID)->GetWindowRect(&rect);
	m_parent->ClientToScreen(&point);
	if (point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom  && m_dropFlag)
	{
		int angle_Y = point.y - tempPoint.y;
		int angle_X = point.x - tempPoint.x;
		m_theta += (angle_X / 100);
		m_phi += (angle_Y / 100);
		m_theta = (int)m_theta % 360;
		m_phi = (int)m_phi % 360;

		SetWatchAngle(circle_R, m_theta, m_phi);
	}
}
void CMyOpenGL::RenderScene()
{
	DrawCoordSystem(6.0 / circle_R);
	DrawGrid(2.0 / circle_R);

	if (m_pointIDList.size())
	{
		DrawPoint(m_3DPointList, m_pointIDList, 0.1/circle_R);
	}
	else
	{
		GLRGBColor3f GLColor(1.0, 0.0, 1.0);
		DrawPoint(m_3DPointList, 0.1/circle_R, GLColor);
	}

	SwapBuffers(m_hRenderDC);
}

void CMyOpenGL::RenderScene(vector<vector<POINT3D>> point3DList)
{
	DrawCoordSystem(6.0 / circle_R);
	DrawGrid(2.0 / circle_R);

	if (m_pointIDList.size())
	{
		DrawPoint(m_3DPointList, m_pointIDList, 0.1 / circle_R);
	}
	else
	{
		GLRGBColor3f GLColor(1.0, 0.0, 1.0);
		DrawPoint(m_3DPointList, 0.1 / circle_R, GLColor);
	}
	
	GLRGBColor3f glColor(0, 0, 0);
	for (int i = 0; i < point3DList.size(); i++)
	{
		m_3DPointList = point3DList[i];
		if (m_3DPointList[0].X < -4.5)
		{
			glColor.r = 1.0; glColor.g = 0; glColor.b = 0;
			DrawLine(m_3DPointList, 3.0 / circle_R, GL_LINE_STRIP, glColor);
		}
		else if (m_3DPointList[0].X < -0.5)
		{
			glColor.r = 0; glColor.g = 1.0; glColor.b = 0;
			DrawLine(m_3DPointList, 3.0 / circle_R, GL_LINE_STRIP, glColor);
		}
		else
		{
			glColor.r = 0.0; glColor.g = 0; glColor.b = 1.0;
			DrawLine(m_3DPointList, 3.0 / circle_R, GL_LINE_STRIP, glColor);
		}
	}

	POINT3D tempPoint3D, tempShowPnt;
	//if (m_3DPointList.size())
	{
		glColor3f(0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);               // 按顺时针绘制一个正方形，在右上方

		tempPoint3D.X = -0.5;
		tempPoint3D.Y = yMinValue;
		tempPoint3D.Z = 0.5;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.Y = yMaxValue;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.Z = 0.1;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.Y = yMinValue;
		tempPoint3D.Z = 0.1;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		glEnd();

		glBegin(GL_LINE_LOOP);               // 按顺时针绘制一个正方形，在右上方

		tempPoint3D.X = -4.5;
		tempPoint3D.Y = yMinValue;
		tempPoint3D.Z = 0.5;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.X = -4.5;
		tempPoint3D.Y = yMaxValue;
		tempPoint3D.Z = 0.5;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.Z = 0.1;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		tempPoint3D.Y = yMinValue;
		tempPoint3D.Z = 0.1;
		tempShowPnt = ScaleTransformation(tempPoint3D, m_xyScale, m_zScale);
		glVertex3f(tempShowPnt.X, tempShowPnt.Y, tempShowPnt.Z);

		glEnd();
	}
	SwapBuffers(m_hRenderDC);
}

void CMyOpenGL::DrawCube()
{
	glColor3f(1.0f, 0, 0);
	glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
}
POINT3D CMyOpenGL::ScaleTransformation(POINT3D point, float xyScale,float zScale)
{
	point.X = (point.X-xMinValue)*xyScale - 1;
	point.Y = (point.Y-yMinValue)*xyScale - 1;
	point.Z = (point.Z-zMinValue)*zScale - 1;
	return point;
}
void  CMyOpenGL::DrawPoint(POINT3D point, GLfloat pointSize, GLRGBColor3f GLcolor)
{
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glPointSize(pointSize);
	glBegin(GL_POINT);

	POINT3D tempPoint = ScaleTransformation(point, m_xyScale, m_zScale);
	  glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);

	glEnd();
}
void  CMyOpenGL::DrawPoint(vector<POINT3D> pointList, GLfloat pointSize, GLRGBColor3f GLcolor)
{
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	POINT3D tempPoint;
	for (int i = 0; i < pointList.size(); i++)
	{
		tempPoint = ScaleTransformation(pointList[i], m_xyScale, m_zScale);
		
		glPushMatrix();
		glTranslatef(tempPoint.X, tempPoint.Y, tempPoint.Z);
		glutSolidSphere(pointSize, 5, 5);
		glPopMatrix();
	}
	glEnd();
	
	glColor3f(GLcolor.r, 1.0-GLcolor.g, 1.0-GLcolor.b);
	glPointSize(pointSize/4);
	for (int i = 0; i < pointList.size(); i++)
	{
        glBegin(GL_LINES);

		tempPoint = ScaleTransformation(pointList[i], m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
		glVertex3f(tempPoint.X, tempPoint.Y, -1);

		glEnd();
	}
	
}
void CMyOpenGL::DrawPoint(vector<POINT3D> pointList, vector<int> pointIDList, GLfloat pointSize)
{
	GLRGBColor3f glColor;
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	POINT3D tempPoint;
	int r, g, b;
	int colorIndex;
	for (int i = 0; i < pointList.size(); i++)
	{
		colorIndex = pointIDList[i] + 1;
		r = colorIndex % 2;
		g = (colorIndex >> 1) % 2;
		b = (colorIndex >> 2) % 2;
		glColor.r = r;
		glColor.g = g;
		glColor.b = b;
		glColor3f(glColor.r, glColor.g, glColor.b);

		tempPoint = ScaleTransformation(pointList[i], m_xyScale, m_zScale);

		glPushMatrix();
		glTranslatef(tempPoint.X, tempPoint.Y, tempPoint.Z);
		glutSolidSphere(pointSize, 5, 5);
		glPopMatrix();
	}
	glEnd();

}
void CMyOpenGL::DrawLine(vector<POINT3D> pointList, float lineWidth, GLenum lineType, GLRGBColor3f GLcolor)
{
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glLineWidth(lineWidth);
	glBegin(lineType);
	POINT3D tempPoint;
	for (int i = 0; i < pointList.size(); i++)
	{
		tempPoint = ScaleTransformation(pointList[i], m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
	}
	glEnd();
}
void CMyOpenGL::DrawPlane(vector<POINT3D> pointList, GLRGBColor3f GLcolor)
{
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
	POINT3D tempPoint;
	for (int i = 0; i < pointList.size(); i++)
	{
		tempPoint = ScaleTransformation(pointList[i], m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
	}
	glEnd();
}
void CMyOpenGL::DrawLine(POINT3D pnt1, POINT3D pnt2, float lineWidth, GLenum lineType, GLRGBColor3f GLcolor)
{
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glLineWidth(lineWidth);
	glBegin(lineType);

		POINT3D tempPoint;
		tempPoint = ScaleTransformation(pnt1, m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
		tempPoint = ScaleTransformation(pnt2, m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);

	glEnd();
}
void CMyOpenGL::DrawMatrix(unsigned char *data, int rows, int cols, GLfloat lineWidth, GLenum lineType)
{
	GLRGBColor3f GLcolor;
	glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
	glLineWidth(lineWidth);
	glBegin(lineType);
	POINT3D tempPoint;
	int value, ColorValue[3];
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++)
	{

		value = data[i*cols + j];
		for (int c = 0; c < 3; c++)
		{
			ColorValue[c] = value % 85;
			value -= 85;
			if (value < 0)
			{
				for (c++; c < 3; c++) ColorValue[c] = 0;
				break;
			}
		}

		GLcolor.b = 1.0*ColorValue[0] / 85;
		GLcolor.g = 1.0*ColorValue[1] / 85;
		GLcolor.r = 1.0*ColorValue[2] / 85;
		/*
		GLcolor.b = data[i*cols + j] / 255.0;
		GLcolor.g = GLcolor.b;
		GLcolor.r = GLcolor.b;*/
		glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);

		tempPoint.X = j;
		tempPoint.Y = i;
		tempPoint.Z = data[i*cols + j];
		tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
	}
	for (int j = 0; j < cols; j++)
	for (int i = 0; i < rows; i++)
	{

		value = data[i*cols + j];
		for (int c = 0; c < 3; c++)
		{
			ColorValue[c] = value % 85;
			value -= 85;
			if (value < 0)
			{
				for (c++; c < 3; c++) ColorValue[c] = 0;
				break;
			}
		}

		GLcolor.b = 1.0*ColorValue[0] / 85;
		GLcolor.g = 1.0*ColorValue[1] / 85;
		GLcolor.r = 1.0*ColorValue[2] / 85;
		/*
		GLcolor.b = data[i*cols + j] / 255.0;
		GLcolor.g = GLcolor.b;
		GLcolor.r = GLcolor.b;*/
		glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);

		tempPoint.X = j;
		tempPoint.Y = i;
		tempPoint.Z = data[i*cols + j];
		tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
		glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
	}
	glEnd();
}
void CMyOpenGL::DrawMatrix(unsigned char *data, int rows, int cols, int step)
{
	POINT3D tempPoint;
	GLRGBColor3f GLcolor;
	for (int i = step; i < rows; i += step)
	{
		for (int j = step; j < cols; j += step)
		{
			GLcolor.b = data[i*cols + j] / 255.0;
			GLcolor.g = GLcolor.b;
			GLcolor.r = GLcolor.b;
			glColor3f(GLcolor.r, GLcolor.g, GLcolor.b);
			glBegin(GL_POLYGON);
			tempPoint.X = j;
			tempPoint.Y = i;
			tempPoint.Z = data[i*cols + j];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			tempPoint.X = j - step;
			tempPoint.Y = i;
			tempPoint.Z = data[i*cols + j - step];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			tempPoint.X = j - step;
			tempPoint.Y = i - step;
			tempPoint.Z = data[(i - step)*cols + j - step];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			glEnd();

			glBegin(GL_POLYGON);
			tempPoint.X = j;
			tempPoint.Y = i;
			tempPoint.Z = data[i*cols + j];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			tempPoint.X = j;
			tempPoint.Y = i - step;
			tempPoint.Z = data[(i - step)*cols + j];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			tempPoint.X = j - step;
			tempPoint.Y = i - step;
			tempPoint.Z = data[(i - step)*cols + j - step];
			tempPoint = ScaleTransformation(tempPoint, m_xyScale, m_zScale);
			glVertex3f(tempPoint.X, tempPoint.Y, tempPoint.Z);
			glEnd();
		}
	}

}