
// ORBTrackingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ORBTracking.h"
#include "ORBTrackingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CORBTrackingDlg 对话框



CORBTrackingDlg::CORBTrackingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CORBTrackingDlg::IDD, pParent)
	, m_checkTrack1(FALSE)
	, m_checkTrack2(FALSE)
	, m_checkProbe(FALSE)
	, m_checkTrackSpeed(FALSE)
	, m_checkTrack(FALSE)
	, m_checkROI(FALSE)
	, m_checkMouseControl(FALSE)
	, m_sliderGLCircleR(0)
	, m_checkProbe2(FALSE)
	, m_checkRaw(FALSE)
	, m_sliderTgHeight(0)
	, m_sliderTrHeight(0)
	, m_checkDisAA(FALSE)
	, m_checkIdealTrajectory(FALSE)
	, m_sliderXStartPos(50)
	, m_sliderYStartPos(50)
	, m_sliderZStartPos(50)
	, m_sliderSpeed(50)
	, m_checkVariance(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_vehicleCount = 0;
}

void CORBTrackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_checkTrack1);
	DDX_Check(pDX, IDC_CHECK2, m_checkTrack2);
	DDX_Check(pDX, IDC_CHECK3, m_checkProbe);
	DDX_Check(pDX, IDC_CHECK4, m_checkTrackSpeed);
	DDX_Check(pDX, IDC_CHECK6, m_checkTrack);
	DDX_Check(pDX, IDC_CHECK5, m_checkROI);
	DDX_Check(pDX, IDC_CHECK7, m_checkMouseControl);
	DDX_Slider(pDX, IDC_SLIDER1, m_sliderGLCircleR);
	DDX_Check(pDX, IDC_CHECK8, m_checkProbe2);
	DDX_Check(pDX, IDC_CHECK9, m_checkRaw);
	DDX_Slider(pDX, IDC_SLIDER2, m_sliderTgHeight);
	DDX_Slider(pDX, IDC_SLIDER3, m_sliderTrHeight);
	DDX_Check(pDX, IDC_CHECK10, m_checkDisAA);
	DDX_Check(pDX, IDC_CHECK11, m_checkIdealTrajectory);
	DDX_Control(pDX, IDC_COMBO1, m_comboTrack);
	DDX_Slider(pDX, IDC_SliderXStartPos, m_sliderXStartPos);
	DDX_Slider(pDX, IDC_SliderYStartPos, m_sliderYStartPos);
	DDX_Slider(pDX, IDC_SliderZStartPos, m_sliderZStartPos);
	DDX_Slider(pDX, IDC_SliderSpeed, m_sliderSpeed);
	DDX_Check(pDX, IDC_CHECK12, m_checkVariance);
}

BEGIN_MESSAGE_MAP(CORBTrackingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BtnPlay, &CORBTrackingDlg::OnBnClickedBtnplay)
	ON_BN_CLICKED(IDC_BtnNextFrame, &CORBTrackingDlg::OnBnClickedBtnnextframe)
	ON_BN_CLICKED(IDC_BtnBefoFrame, &CORBTrackingDlg::OnBnClickedBtnbefoframe)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CORBTrackingDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CORBTrackingDlg::OnBnClickedCheck2)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK3, &CORBTrackingDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CORBTrackingDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CORBTrackingDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CORBTrackingDlg::OnBnClickedCheck6)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK7, &CORBTrackingDlg::OnBnClickedCheck7)
	ON_WM_MOUSEHWHEEL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CORBTrackingDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_CHECK8, &CORBTrackingDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CORBTrackingDlg::OnBnClickedCheck9)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CORBTrackingDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CORBTrackingDlg::OnNMCustomdrawSlider3)
	ON_BN_CLICKED(IDC_CHECK10, &CORBTrackingDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, &CORBTrackingDlg::OnBnClickedCheck11)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CORBTrackingDlg::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SliderXStartPos, &CORBTrackingDlg::OnNMCustomdrawSliderxstartpos)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SliderYStartPos, &CORBTrackingDlg::OnNMCustomdrawSliderystartpos)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SliderZStartPos, &CORBTrackingDlg::OnNMCustomdrawSliderzstartpos)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SliderSpeed, &CORBTrackingDlg::OnNMCustomdrawSliderspeed)
	ON_BN_CLICKED(IDC_CHECK12, &CORBTrackingDlg::OnBnClickedCheck12)
END_MESSAGE_MAP()


// CORBTrackingDlg 消息处理程序

BOOL CORBTrackingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_historgramZOX.Create(IDC_PicZOX, this);
	m_historgramYOX.Create(IDC_PicYOX, this);
	m_historgramZOY.Create(IDC_PicZOY, this);
	m_historgramYOX.SetLimit(-900, 500, 0, 6000);
	m_historgramZOX.SetLimit(-900, 500, -100, 600);
	m_historgramDisAA.Create(IDC_PicDisAA, this);
	m_historgramHeightDiffError.Create(IDC_PicZOY, this);


	m_laneBorder1 = -4.5;
	m_laneBorder2 = -0.5;

	m_openGL.Create(IDC_Pic3DShow, this);
	m_openGL.InitLimit(-8.5, 6, 0, 30, 0, 10);
	m_openGL.SetWatchAngle(m_openGL.circle_R, m_openGL.m_theta, m_openGL.m_phi);

	m_comboTrack.AddString(_T("ORB"));
	m_comboTrack.AddString(_T("SIFT"));
	m_comboTrack.SetCurSel(0);

	CString tempStr;
	m_T1XStartCoord = (m_sliderXStartPos - 50)*0.2;
	m_T1YStartCoord = 10 + m_sliderYStartPos * 0.2;
	m_T1ZStartCoord = m_sliderZStartPos*0.1;
	m_T1Speed = m_sliderSpeed;
	tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T1XStartCoord, m_T1YStartCoord, m_T1ZStartCoord, m_T1Speed);
	GetDlgItem(IDC_StaT1Info)->SetWindowText(tempStr);
	m_T2XStartCoord = (m_sliderXStartPos - 50)*0.2;
	m_T2YStartCoord = 10 + m_sliderYStartPos * 0.2;
	m_T2ZStartCoord = m_sliderZStartPos*0.1;
	m_T2Speed = m_sliderSpeed;
	tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T2XStartCoord, m_T2YStartCoord, m_T2ZStartCoord, m_T2Speed);
	GetDlgItem(IDC_StaT2Info)->SetWindowText(tempStr);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CORBTrackingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CORBTrackingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CORBTrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CORBTrackingDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // 复制图片
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内
	ReleaseDC(pDC);
}
//////////////////////////////
/*
输入：IplImage *img //要显示的图像
输入：int id //显示图像的控件的id号
*/
//////////////////////////////
void CORBTrackingDlg::ShowImage(Mat& src, UINT ID)
{

	Mat dest = src.clone();
	if (src.empty())
	{
		AfxMessageBox(_T("没有获取图像"));
		return;
	}
	dest = cv::Scalar::all(0);
	// 计算将图片缩放到 Image 区域所需的比例因子
	double wRatio = dest.cols / (double)src.cols;
	double hRatio = dest.rows / (double)src.rows;
	double srcWH = src.cols / (double)src.rows;
	double desWH = dest.cols / (double)dest.rows;
	double scale = srcWH > desWH ? wRatio : hRatio;

	// 缩放后图片的宽和高
	int nw = (int)(src.cols * scale);
	int nh = (int)(src.rows * scale);

	// 为了将缩放后的图片存入 des 的正中部位，需计算图片在 des 左上角的期望坐标值
	int tlx = (int)((dest.cols - nw) / 2);
	int tly = (int)((dest.rows - nh) / 2);

	// 设置 des 的 ROI 区域，用来存入图片 img
	cv::Mat desRoi = dest(cv::Rect(tlx, tly, nw, nh));

	// 如果src是单通道图像，则转换为三通道图像
	if (src.channels() == 1)
	{
		cv::Mat src_c;
		cvtColor(src, src_c, CV_GRAY2BGR);
		// 对图片 src_t 进行缩放，并存入到 des 中
		resize(src_c, desRoi, desRoi.size());
	}
	else
	{
		// 对图片 src 进行缩放，并存入到 des 中
		resize(src, desRoi, desRoi.size());
	}

	CDC* pDC = GetDlgItem(ID)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);	// 获取控件尺寸位置
	CvvImage image;
	IplImage cpy = desRoi;  //desRoi 是缩放后的图片，des是没有适应控件的图片
	image.CopyOf(&cpy);						// 复制图片
	image.DrawToHDC(hDC, &rect);				// 将图片绘制到显示控件的指定区域内
	ReleaseDC(pDC);/**/
}
void CORBTrackingDlg::DrawPoint(CPoint imgPoint, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int nRadius)
{
	CRect rect;
	GetDlgItem(nID)->GetWindowRect(rect);

	CWnd* pWnd = GetDlgItem(nID);
	CDC* pDC = pWnd->GetDC();

	CBrush brush(color);
	pDC->SelectObject(brush);
	CPoint point;
	point.x = imgPoint.x*rect.Width() / IMAGE_WIDTH;
	point.y = imgPoint.y*rect.Height() / IMAGE_HEIGHT;
	pDC->Ellipse(point.x - nRadius, point.y - nRadius, point.x + nRadius, point.y + nRadius);

	pDC->DeleteDC();
}
void CORBTrackingDlg::DrawLine(vector<POINT2D> pointList, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int lineWidth)
{
	CRect rect;
	GetDlgItem(nID)->GetWindowRect(rect);

	CWnd* pWnd = GetDlgItem(nID);
	CDC* pDC = pWnd->GetDC();
	CPen pen(PS_SOLID, lineWidth, color);
	pDC->SelectObject(pen);

	if (pointList.size())
	{
		CPoint tempPoint;
		tempPoint.x = (pointList[0].x + m_roi.right)*rect.Width() / IMAGE_WIDTH;
		tempPoint.y = pointList[0].y*rect.Height() / IMAGE_HEIGHT;
		pDC->Ellipse(tempPoint.x - 2, tempPoint.y - 2, tempPoint.x + 2, tempPoint.y + 2);
		pDC->MoveTo(tempPoint);
		for (int i = 1; i < pointList.size(); i++)
		{
			tempPoint.x = (pointList[i].x + m_roi.right)*rect.Width() / IMAGE_WIDTH;
			tempPoint.y = pointList[i].y*rect.Height() / IMAGE_HEIGHT;
			pDC->Ellipse(tempPoint.x - 2, tempPoint.y - 2, tempPoint.x + 2, tempPoint.y + 2);
			pDC->LineTo(tempPoint);
		}
	}

	pDC->DeleteDC();
}
void CORBTrackingDlg::DrawRect(int left, int right, int top, int bottom, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int lineWidth)
{
	CRect rect;
	GetDlgItem(nID)->GetWindowRect(rect);

	CWnd* pWnd = GetDlgItem(nID);
	CDC* pDC = pWnd->GetDC();
	CPen pen(PS_SOLID, lineWidth, color);
	pDC->SelectObject(pen);

	pDC->MoveTo(left*rect.Width() / IMAGE_WIDTH, bottom*rect.Height() / IMAGE_HEIGHT);
	pDC->LineTo(right*rect.Width() / IMAGE_WIDTH, bottom*rect.Height() / IMAGE_HEIGHT);
	pDC->LineTo(right*rect.Width() / IMAGE_WIDTH, top*rect.Height() / IMAGE_HEIGHT);
	pDC->LineTo(left*rect.Width() / IMAGE_WIDTH, top*rect.Height() / IMAGE_HEIGHT);
	pDC->LineTo(left*rect.Width() / IMAGE_WIDTH, bottom*rect.Height() / IMAGE_HEIGHT);

	pDC->DeleteDC();
}
void CORBTrackingDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//获取文件路径并显示到编辑框中  
	TCHAR filePath[MAX_PATH] = { 0 };
	//单文件操作，第二参数置0，第三个参数返回文件路径                             
	DragQueryFile(hDropInfo, 0, filePath, MAX_PATH - 1);
	m_strFilePathName = filePath;

	m_fileName = m_strFilePathName.Mid(m_strFilePathName.ReverseFind('\\') + 1, ((m_strFilePathName.ReverseFind('.') - m_strFilePathName.ReverseFind('\\')) - 1));//用CString 成员函数来获取。  
	CString fileType = m_strFilePathName.Right(m_strFilePathName.GetLength() - m_strFilePathName.ReverseFind('.') - 1); //截取到文件的后缀（jpg格式或者rar格式等）
	char* pFilePathName = new char[2048];
	Common::TcharToChar(filePath, pFilePathName);
	if (fileType == _T("avi"))
	{
		m_capture = cvCaptureFromAVI(pFilePathName);
		m_frame = cvQueryFrame(m_capture);
		//m_nChannels = (m_frame->nChannels == 1) ? CV_8UC1 : CV_8UC3;
		m_image.create(m_frame->height, m_frame->width, CV_8UC3);
		m_image.data = (uchar*)m_frame->imageData;
		ShowImage(m_image, IDC_PicShow);

		m_totalFrames = (int)cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_COUNT);
		m_nFrames = 1;
		CString strFrameNumber;
		strFrameNumber.Format(_T("%d/%d"), m_nFrames, m_totalFrames);
		//GetDlgItem(IDC_StaFrameNumber)->SetWindowText(strFrameNumber);

		mImageHeight = m_frame->height;
		mImageWidth = m_frame->width;

		///////////////////////////////////
		m_roi.left = 0; m_roi.right = m_frame->width;
		m_roi.bottom = m_frame->height/2; m_roi.top = m_frame->height;
		m_orbImg1.create((m_roi.top - m_roi.bottom), (m_roi.right - m_roi.left), CV_8UC3);
		m_orbImg2.create((m_roi.top - m_roi.bottom), (m_roi.right - m_roi.left), CV_8UC3);
		m_jointImg.create((m_roi.top - m_roi.bottom), (m_roi.right - m_roi.left)*2, CV_8UC3);
		CvSize size;
		size.height = (m_roi.top - m_roi.bottom);
		size.width = (m_roi.right - m_roi.left) * 2;
		m_showJointImage = cvCreateImage(size, IPL_DEPTH_8U, 3);

		size.width /= 2;
		m_siftImg1 = cvCreateImage(size, IPL_DEPTH_8U, 3);
		m_siftImg2 = cvCreateImage(size, IPL_DEPTH_8U, 3);

		GetROIImage(m_image, m_roi, m_orbImg1);
		if(m_comboTrack.GetCurSel()==0) m_myORB1.ORBDescriptorExtract(m_orbImg1);
		else
		{
			m_siftImg1->imageData = (char*)m_orbImg1.data;
			m_sift1.SiftFeatureExtract(m_siftImg1);
		}

		m_frame = cvQueryFrame(m_capture);
		m_nFrames++;
		m_image.data = (uchar*)m_frame->imageData;

		GetROIImage(m_image, m_roi, m_orbImg2);
		if (m_comboTrack.GetCurSel() == 0) m_myORB2.ORBDescriptorExtract(m_orbImg2);
		else
		{
			m_siftImg2->imageData = (char *)m_orbImg2.data;
			m_sift2.SiftFeatureExtract(m_siftImg2);
		}

		if (m_comboTrack.GetCurSel() == 0) m_myORB1.ORBDescriptorMatch(m_myORB2.m_keyPoint, m_myORB2.m_descriptors, m_matchList);
		else  m_sift1.SiftFeatureMatch(m_sift1.m_feat, m_sift2.m_feat, m_sift1.m_featNum, m_sift2.m_featNum, m_matchList);
		///////////////////////////////////////
		JointImage(m_orbImg1, m_orbImg2, m_jointImg);
		m_showJointImage->imageData = (char*)m_jointImg.data;
		CvPoint pt1, pt2;
		for (int i = 0; i < m_matchList.size(); i++)
		{
			pt1 = m_matchList[i].pnt;
			pt2 = m_matchList[i].matchPoint;
			pt2.x += m_roi.right;
			cvLine(m_showJointImage, pt1, pt2, CV_RGB(255, 0, 255), 1, 8, 0);
		}
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		///////////////////////////////////
		POINT2D tempPoint;
		CTrackLine tempTrackLine;
		for (int i = 0; i < m_matchList.size(); i++)
		{
			tempTrackLine.pointList.clear();
			tempPoint.x = m_matchList[i].pnt.x;
			tempPoint.y = m_matchList[i].pnt.y;
			tempTrackLine.pointList.push_back(tempPoint);
			tempPoint.x = m_matchList[i].matchPoint.x;
			tempPoint.y = m_matchList[i].matchPoint.y;
			tempTrackLine.pointList.push_back(tempPoint);
			m_trackList.push_back(tempTrackLine);
		}
		m_myORB1 = m_myORB2;
		memcpy(m_myORB1.m_descriptors.data, m_myORB2.m_descriptors.data, m_myORB1.m_descriptors.rows*m_myORB1.m_descriptors.cols*m_myORB1.m_descriptors.channels());
		memcpy(m_orbImg1.data,m_orbImg2.data,(m_orbImg1.rows*m_orbImg1.cols*m_orbImg1.channels()));

		if (CMyTDMap::ReadTDMapMatrix("TDMapMatrix.dat", &m_TDMap))
		{
			m_TDMap.isok = true;
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CORBTrackingDlg::OnBnClickedBtnplay()
{
	// TODO:  在此添加控件通知处理程序代码
	static bool playFlag = true;
	if (playFlag)
	{
		SetTimer(1, 400, NULL);
		GetDlgItem(IDC_BtnPlay)->SetWindowText(_T("暂停"));
		playFlag = false;
	}
	else
	{
		KillTimer(1);
		GetDlgItem(IDC_BtnPlay)->SetWindowText(_T("播放"));
		playFlag = true;
	}
}


void CORBTrackingDlg::OnBnClickedBtnnextframe()
{
	// TODO:  在此添加控件通知处理程序代码
	KillTimer(1);
	m_frame = cvQueryFrame(m_capture);
	if (m_frame)
	{
		m_image.data = (uchar*)m_frame->imageData;
		ShowImage(m_image, IDC_PicShow);

		m_nFrames++;
		CString strFrameNumber;
		strFrameNumber.Format(_T("%d/%d"), m_nFrames, m_totalFrames);
		GetDlgItem(IDC_StaFrameNumber)->SetWindowText(strFrameNumber);

		//if (m_nFrames % 2)
			Tracking(m_frame);
	}
}


void CORBTrackingDlg::OnBnClickedBtnbefoframe()
{
	// TODO:  在此添加控件通知处理程序代码
	KillTimer(1);
	cvSetCaptureProperty(m_capture, CV_CAP_PROP_POS_FRAMES, --m_nFrames);
	m_frame = cvQueryFrame(m_capture);

	if (m_frame)
	{
		m_image.data = (uchar*)m_frame->imageData;
		ShowImage(m_image, IDC_PicShow);

		CString strFrameNumber;
		strFrameNumber.Format(_T("%d/%d"), m_nFrames, m_totalFrames);
		GetDlgItem(IDC_StaFrameNumber)->SetWindowText(strFrameNumber);
	}
}


void CORBTrackingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//KillTimer(1);
	m_frame = cvQueryFrame(m_capture);
	if (m_frame)
	{
		m_image.data = (uchar*)m_frame->imageData;
		ShowImage(m_image, IDC_PicShow);

		m_nFrames++;
		CString strFrameNumber;
		strFrameNumber.Format(_T("%d/%d"), m_nFrames, m_totalFrames);
		GetDlgItem(IDC_StaFrameNumber)->SetWindowText(strFrameNumber);

		//if(m_nFrames%2) 
			Tracking(m_frame);
	}
	else
	{
		KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}
void CORBTrackingDlg::GetROIImage(cv::Mat frame, ROI roi, cv::Mat image)
{
	for (int i = roi.bottom; i < roi.top; i++)
	{
		for (int j = roi.left; j < roi.right; j++)
		{
			for (int k = 0; k < image.channels(); k++)
			{
				image.data[((i - roi.bottom)*image.cols + (j - roi.left))*image.channels() + k] = frame.data[(i*frame.cols + j)*frame.channels() + k];
			}
		}
	}
}
void CORBTrackingDlg::JointImage(cv::Mat image1, cv::Mat image2, cv::Mat stackImage)
{
	for (int i = 0; i < image1.rows; i++)
	{
		for (int j = 0; j < image1.cols; j++)
		{
			for (int k = 0; k < image1.channels(); k++)
			{
				stackImage.data[(i*stackImage.cols + j)*stackImage.channels() + k] = image1.data[(i*image1.cols + j)*image1.channels() + k];
			}
		}
	}
	for (int i = 0; i < image2.rows; i++)
	{
		for (int j = 0; j < image2.cols; j++)
		{
			for (int k = 0; k < image2.channels(); k++)
			{
				stackImage.data[(i*stackImage.cols + (j + image1.cols))*stackImage.channels() + k] = image2.data[(i*image2.cols + j)*image2.channels() + k];
			}
		}
	}
}
void CORBTrackingDlg::Tracking(IplImage *frame)
{
	clock_t start, finish;//用来为程序计时
	double duration;
	start = clock();
	InitTrackList(m_trackList);
	m_image.data = (uchar*)frame->imageData;
	GetROIImage(frame, m_roi, m_orbImg2);
	/////////
	if (m_comboTrack.GetCurSel() == 0)
	{
		m_myORB1.ORBDescriptorExtract(m_orbImg1);
		m_myORB2.ORBDescriptorExtract(m_orbImg2);
		m_myORB1.ORBDescriptorMatch(m_myORB2.m_keyPoint, m_myORB2.m_descriptors, m_matchList);
	}
	else
	{
		m_siftImg2->imageData = (char *)m_orbImg2.data;
		m_sift2.SiftFeatureExtract(m_siftImg2);
		m_sift1.SiftFeatureMatch(m_sift1.m_feat, m_sift2.m_feat, m_sift1.m_featNum, m_sift2.m_featNum, m_matchList);
	}
	FilterMatchFeature(m_matchList, 3, 100);
	AddTrackNode(m_matchList, m_trackList);
	DealTrackList(m_trackList,&m_refeTrajectory);
	ConfirmTrajectoryRelativeHeight(m_TDMap, m_trackList, &m_refeTrajectory);
	
	CString tempStr;
	tempStr.Format(_T("%d"), m_vehicleCount);
	GetDlgItem(IDC_StaVehicleCount)->SetWindowText(tempStr);
///////////	
	SectionalProjectinAnalyseOX(m_trackList, m_pointZOX, m_pointYOX);
	GetPointSets(m_trackList, m_pointList, 2.5, 1, 1.5);

	m_DBSCAN.Init(m_pointList);
	m_DBSCAN.DoDBSCAN(2.5, 2, m_pointIDList);
	m_openGL.m_3DPointList = m_pointList;
	m_openGL.m_pointIDList = m_pointIDList;
	m_openGL.RenderScene();
////////////
	if (m_historgramOK)
	{
		m_historgramYOX.SetLimit(-900, 500, 0, 6000);
		m_historgramZOX.SetLimit(-900, 500, -100, 600);
		m_historgramYOX.m_pointList = m_pointYOX;
		m_historgramZOX.m_pointList = m_pointZOX;
		m_historgramYOX.RenderScene();
		m_historgramZOX.RenderScene();
	}


	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;

	JointImage(m_orbImg1, m_orbImg2, m_jointImg);
	m_showJointImage->imageData = (char*)m_jointImg.data;
	DrawTrackLine(m_showJointImage, m_trackList);

	//m_myORB1 = m_myORB2;
	//memcpy(m_myORB1.m_descriptors.data, m_myORB2.m_descriptors.data, m_myORB1.m_descriptors.rows*m_myORB1.m_descriptors.cols*m_myORB1.m_descriptors.channels());
	if (m_comboTrack.GetCurSel() == 0)
	{
		memcpy(m_orbImg1.data, m_orbImg2.data, (m_orbImg1.rows*m_orbImg1.cols*m_orbImg1.channels()));
	}
	else
	{
		free(m_sift1.m_feat);
		memcpy(m_siftImg1->imageData, m_siftImg2->imageData, m_siftImg1->height*m_siftImg1->widthStep*sizeof(uchar));
		m_sift1 = m_sift2;
	}

	//DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
	///////////////////////////////////////
	//CvPoint pt1, pt2;
	//for (int i = 0; i < m_matchList.size(); i++)
	//{
	//	pt1 = m_matchList[i].pnt;
	//	pt2 = m_matchList[i].matchPoint;
	//	pt2.x += m_roi.right;
	//	cvLine(m_showJointImage, pt1, pt2, CV_RGB(255, 0, 255), 1, 8, 0);
	//}
	DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
	///////////////////////////////////
	//if(m_refeTrajectory.bRefeTrajectoryOK) DrawLine(m_refeTrajectory.pointList, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 255, 0), 2);
	//DrawLine(m_refeTrajectory.pointList, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 255, 0), 2);
	//m_point3DList.clear();
	//vector<POINT3D> temp3DList;
	//POINT3D temp3DPoint;
	//int imgPnt[2];
	//float realCoord[3];
	//for (int i = 0; i < m_trackList.size(); i++)
	//{
	//	if (m_trackList[i].pointList.size()>(TrackNodeEqu5 - 1))
	//	{
	//		temp3DList.clear();
	//		for (int k = 0; k < m_trackList[i].pointList.size(); k++)
	//		{
	//			imgPnt[0] = m_trackList[i].pointList[k].x;
	//			imgPnt[1] = m_trackList[i].pointList[k].y;
	//			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt, m_trackList[i].height, realCoord);
	//			temp3DPoint.X = realCoord[0];
	//			temp3DPoint.Y = realCoord[1];
	//			temp3DPoint.Z = realCoord[2];
	//			temp3DList.push_back(temp3DPoint);
	//		}
	//		m_point3DList.push_back(temp3DList);
	//	}
	//}
	//m_openGL.RenderScene(m_point3DList);
	
	//////////////////////////////////////////////
	//用于测试
	CPoint tempPoint;
	m_niceTrackList.clear();
	int r=1, g=0, b=0;
	int index,count;
	for (int i = 0; i < m_trackList.size(); i++)
	{
		if (m_trackList[i].pointList.size()>(TrackNodeEqu5-3))
		{
			tempPoint.x = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].x + m_roi.right;
			tempPoint.y = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].y;

	       	DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(r*255, g*255, b*255), 2);

			m_niceTrackList.push_back(m_trackList[i]);
		}
		else
		{
			tempPoint.x = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].x + m_roi.right;
			tempPoint.y = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].y;
			//DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 255), 2);
		}
	}
	
	////////////////////////////////////////////
}
void CORBTrackingDlg::InitTrackList(vector<CTrackLine> &trackList)
{
	for (int i = 0; i < trackList.size(); i++)
	{
		trackList[i].addNewNode = false;
	}
}
void CORBTrackingDlg::AddTrackNode(vector<FeatureMatch> &orbMatchList, vector<CTrackLine> &trackList)
{
	POINT2D tempPoint;
	int index;
	for (int i = 0; i < orbMatchList.size(); i++)
	{
		tempPoint.x = orbMatchList[i].pnt.x;
		tempPoint.y = orbMatchList[i].pnt.y;
		for (int k = 0; k < trackList.size(); k++)
		{
			index = trackList[k].pointList.size() - 1;
			if (tempPoint.x == trackList[k].pointList[index].x&&tempPoint.y == trackList[k].pointList[index].y)
			{
				tempPoint.x = orbMatchList[i].matchPoint.x;
				tempPoint.y = orbMatchList[i].matchPoint.y;
				trackList[k].pointList.push_back(tempPoint);
				trackList[k].addNewNode = true;

				orbMatchList.erase(orbMatchList.begin() + i);
				i--;
				break;
			}
		}
	}

	CTrackLine tempTrackLine;
	for (int i = 0; i < orbMatchList.size(); i++)
	{
		tempTrackLine.pointList.clear();
		tempPoint.x = orbMatchList[i].pnt.x;
		tempPoint.y = orbMatchList[i].pnt.y;
		tempTrackLine.pointList.push_back(tempPoint);
		tempPoint.x = orbMatchList[i].matchPoint.x;
		tempPoint.y = orbMatchList[i].matchPoint.y;
		tempTrackLine.pointList.push_back(tempPoint);
		tempTrackLine.addNewNode = true;
		trackList.push_back(tempTrackLine);
	}
}

void CORBTrackingDlg::DealTrackList(vector<CTrackLine> &trackList, CTrackLine *refeTrajectory)
{
	//////////////////
	//判断轨迹的线性度删除错误的跟踪线
	int index1,index2;
	float k, b;
	bool bY_X;
	float disX, disY;
	for (int i = 0; i < trackList.size(); i++)
	{
		if (trackList[i].pointList.size()>(TrackNodeEqu5 - 1)&&trackList[i].addNewNode)
		{
			index1 = trackList[i].pointList.size() - 1;
			index2 = trackList[i].pointList.size() - TrackNodeEqu5;
			disX = trackList[i].pointList[index1].x - trackList[i].pointList[index2].x;
			disY = trackList[i].pointList[index1].y - trackList[i].pointList[index2].y;
			if ((disY*disY) < (disX*disX))
			{
				bY_X = true;
				k = 1.0*disY / disX;
				b = 1.0*trackList[i].pointList[index2].y - k*trackList[i].pointList[index2].x;
			}
			else
			{
				bY_X = false;
				k = 1.0*disX / disY;
				b = 1.0*trackList[i].pointList[index2].x - k*trackList[i].pointList[index2].y;
			}
			float dis;
			float maxDis = 0;
			float x, y;
			float deta = pow((1+k*k), 0.5);
			for (int t = index1; t > index2; t--)
			{
				x = trackList[i].pointList[t].x;
				y = trackList[i].pointList[t].y;
				if (bY_X)	dis = y - k*x - b;
				else dis = x - k*y - b;
				dis = dis > 0 ? dis : (-dis);
				dis /= deta;
				if (dis > maxDis) maxDis = dis;
				if (maxDis > 10)
				{
					trackList[i].bBadTrajectory = true;
					trackList.erase(trackList.begin() + i);
					break;
				}
			}
		}
	}
	///////////////////////////////////////////
	for (int i = 0; i < trackList.size(); i++)
	{
		if (trackList[i].addNewNode == false)
		{
			if (trackList[i].bRefeTrajectoryOK)
			{	
				trackList[i].bRefeTrajectoryOK = false;
				refeTrajectory->bRefeTrajectoryOK = false;
				for (int k = 0; k < trackList.size(); k++)
				{
					if (trackList[k].height != 0 && trackList[k].addNewNode)
					{
						*refeTrajectory = trackList[k];
						trackList[k].bRefeTrajectoryOK = true;
						refeTrajectory->bRefeTrajectoryOK = true;
						break;
					}
				}
			}
			trackList.erase(trackList.begin() + i);
			i--;
		}
		else if (trackList[i].bBadTrajectory)
		{
			if (trackList[i].bRefeTrajectoryOK)
			{
				trackList[i].bRefeTrajectoryOK = false;
				refeTrajectory->bRefeTrajectoryOK = false;
				for (int k = 0; k < trackList.size(); k++)
				{
					if (trackList[k].height != 0 && trackList[k].addNewNode)
					{
						*refeTrajectory = trackList[k];
						trackList[k].bRefeTrajectoryOK = true;
						refeTrajectory->bRefeTrajectoryOK = true;
						break;
					}
				}
			}
		}
		else if (trackList[i].bRefeTrajectoryOK)
		{
			*refeTrajectory = trackList[i];
		}
	}
}
void CORBTrackingDlg::FilterMatchFeature(vector<FeatureMatch> &orbMatchList, int minOffset, int maxOffset)
{
	int offset;
	for (int i = 0; i < orbMatchList.size(); i++)
	{
		offset = abs(orbMatchList[i].pnt.x - orbMatchList[i].matchPoint.x) + abs(orbMatchList[i].pnt.y - orbMatchList[i].matchPoint.y);
		if (offset<minOffset || offset>maxOffset)
		{
			orbMatchList.erase(orbMatchList.begin() + i);
			i--;
		}
	}
}
void CORBTrackingDlg::DrawTrackLine(IplImage *stackImage, vector<CTrackLine> trackList)
{
	CvPoint pt1, pt2;
	for (int i = 0; i < trackList.size(); i++)
	{  
		if (trackList[i].bBadTrajectory)
		{
			for (int nNo = 1; nNo < trackList[i].pointList.size(); nNo++)
			{
				pt1.x = trackList[i].pointList[nNo - 1].x + m_roi.right;
				pt1.y = trackList[i].pointList[nNo - 1].y;
				pt2.x = trackList[i].pointList[nNo].x + m_roi.right;
				pt2.y = trackList[i].pointList[nNo].y;

				cvLine(stackImage, pt1, pt2, CV_RGB(0, 0,0));
			}
		}
		else if (trackList[i].pointList.size()>(TrackNodeEqu5 - 1))
		{
			for (int nNo = 1; nNo < trackList[i].pointList.size(); nNo++)
			{
				pt1.x = trackList[i].pointList[nNo - 1].x + m_roi.right;
				pt1.y = trackList[i].pointList[nNo - 1].y;
				pt2.x = trackList[i].pointList[nNo].x + m_roi.right;
				pt2.y = trackList[i].pointList[nNo].y;
				if (trackList[i].realCoord[0]<m_laneBorder1)
				  cvLine(stackImage, pt1, pt2, CV_RGB(255, 0, 0));
				else if (trackList[i].realCoord[0]<m_laneBorder2)
				  cvLine(stackImage, pt1, pt2, CV_RGB(0, 255, 0));
				else 
				  cvLine(stackImage, pt1, pt2, CV_RGB(0, 0, 255));
			}
		}

	}
}

void CORBTrackingDlg::ConfirmTrajectoryRelativeHeight(CTDMap mTDMap, vector<CTrackLine> &trackList, CTrackLine *refeTrajectory)
{
	int index;
	float imgPnt1[2], imgPnt2[2];
	float realCoord1[3], realCoord2[3];
	
    if (refeTrajectory->bRefeTrajectoryOK==false)
	{
		refeTrajectory->proSpeed = 1000;
		int refeIndex = -1;
		for (int i = 0; i < trackList.size(); i++)
		{
			if (trackList[i].pointList.size() > (TrackNodeEqu5-1))
			{
				index = trackList[i].pointList.size() - 1;
				imgPnt1[0] = trackList[i].pointList[index].x + m_roi.left;
				imgPnt1[1] = trackList[i].pointList[index].y + m_roi.bottom;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, 0, realCoord1);
				imgPnt2[0] = trackList[i].pointList[index - TrackNodeEqu5 + 1].x + m_roi.left;
				imgPnt2[1] = trackList[i].pointList[index - TrackNodeEqu5 + 1].y + m_roi.bottom;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, 0, realCoord2);

				trackList[i].proSpeed = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);
				if (trackList[i].proSpeed < refeTrajectory->proSpeed)
				{
					refeTrajectory->proSpeed = trackList[i].proSpeed;
					refeIndex = i;
				}
			}
		}
		if (refeIndex != -1)
		{
			trackList[refeIndex].bRefeTrajectoryOK = true;
             *refeTrajectory = trackList[refeIndex];
		}
	}
	if (refeTrajectory->bRefeTrajectoryOK)
	{
		index = refeTrajectory->pointList.size() - 1;
		imgPnt1[0] = refeTrajectory->pointList[index].x + m_roi.left;
		imgPnt1[1] = refeTrajectory->pointList[index].y + m_roi.bottom;
		imgPnt2[0] = refeTrajectory->pointList[index - TrackNodeEqu5 + 1].x + m_roi.left;
		imgPnt2[1] = refeTrajectory->pointList[index - TrackNodeEqu5 + 1].y + m_roi.bottom;

		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, refeTrajectory->height, realCoord1);
		for (int i = 0; i < 3; i++) 	refeTrajectory->realCoord[i] = realCoord1[i];

		float point[3][3];
		point[0][0] = 0; point[0][1] = 0;
		point[1][0] = 0; point[1][1] = 2.0;
		point[2][0] = 2.0; point[2][1] = 2.0;
		float offset1[3], offset2;
		for (int k = 0; k < 3; k++)
		{
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[k][0], realCoord1);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[k][0], realCoord2);
			offset1[k] = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);
			offset1[k] = pow(offset1[k], 0.5);
		}
		
		float A, B, C;
		for (int i = 0; i < trackList.size(); i++)
		{
			if (trackList[i].pointList.size() >(TrackNodeEqu5 - 1))
			{
				index = trackList[i].pointList.size() - 1;
				imgPnt1[0] = trackList[i].pointList[index].x + m_roi.left;
				imgPnt1[1] = trackList[i].pointList[index].y + m_roi.bottom;
				imgPnt2[0] = trackList[i].pointList[index - TrackNodeEqu5 + 1].x + m_roi.left;
				imgPnt2[1] = trackList[i].pointList[index - TrackNodeEqu5 + 1].y + m_roi.bottom;
				for (int k = 0; k < 3; k++)
				{
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[k][1], realCoord1);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[k][1], realCoord2);
					offset2 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);
					offset2 = pow(offset2, 0.5);
					point[k][2] = offset1[k] - offset2;
				}
				C = point[0][2];
				B = (point[1][2] - C) / 2.0;
				A = (point[2][2] - C - B*2.0) / 2.0;
				trackList[i].height = (-C - A*refeTrajectory->height) / B;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, trackList[i].height, realCoord1);
				for (int k = 0; k < 3; k++) trackList[i].realCoord[k] = realCoord1[k];
				trackList[i].height = realCoord1[2];
			}
		}
	}
}
void CORBTrackingDlg::VehicleCounting(vector<CTrackLine> &trackList, int *vehicleCount, int upLine)
{
	int index;
	for (int i = 0; i < trackList.size(); i++)
	{
		if (trackList[i].pointList.size()>(TrackNodeEqu5 - 1))
		{
			index = trackList[i].pointList.size() - 1;
			if (trackList[i].pointList[index].y<=upLine&&trackList[i].pointList[index - 1].y>upLine)
			{

			}
		}

	}
}
void CORBTrackingDlg::SectionalProjectinAnalyseOX(vector<CTrackLine> trackList, vector<CPoint> &pointZOX, vector<CPoint> &pointYOX)
{
	m_historgramOK = false;
	pointZOX.clear();
	pointYOX.clear();
	CPoint tempPoint;
	int leftIndex;
	int rightIndex;
	int mindleIndex;
	for (int i = 0; i < trackList.size(); i++)
	{
		if (trackList[i].pointList.size() >(TrackNodeEqu5 - 1))
		{
			m_historgramOK = true;
			tempPoint.x = trackList[i].realCoord[0] * 100;		
			tempPoint.y = trackList[i].realCoord[2] * 100;
			if (pointZOX.size())
			{
				leftIndex = 0;
				rightIndex = pointZOX.size() - 1;
				if (tempPoint.x < pointZOX[0].x)
				{
					pointZOX.insert(pointZOX.begin(), tempPoint);
					tempPoint.y = m_trackList[i].realCoord[1] * 100;
					pointYOX.insert(pointYOX.begin(), tempPoint);
				}
				else if (tempPoint.x>pointZOX[rightIndex].x)
				{
					pointZOX.push_back(tempPoint);
					tempPoint.y = trackList[i].realCoord[1] * 100;
					pointYOX.push_back(tempPoint);
				}
				else
				{
				    while (rightIndex - leftIndex >1)
				    {
						mindleIndex = (leftIndex + rightIndex) / 2;
						if (tempPoint.x >= pointZOX[mindleIndex].x) leftIndex = mindleIndex;
						else rightIndex = mindleIndex;
				    }
					pointZOX.insert(pointZOX.begin() + leftIndex+1, tempPoint);
					tempPoint.y = trackList[i].realCoord[1] * 100;
					pointYOX.insert(pointYOX.begin() + leftIndex+1, tempPoint);
				}

			}
			else
			{
				pointZOX.push_back(tempPoint);
				tempPoint.y = trackList[i].realCoord[1] * 100;
				pointYOX.push_back(tempPoint);
			}
		}
	}
}
void CORBTrackingDlg::SectionalProjectionAnalyseZO(vector<POINT3D> point3DList, vector<CPoint> &pointZOX, vector<CPoint> &pointZOY)
{
	m_historgramOK = false;
	pointZOX.clear();
	pointZOY.clear();
	CPoint tempPoint;
	int leftIndex;
	int rightIndex;
	int mindleIndex;
	for (int i = 0; i < point3DList.size(); i++)
	{

			m_historgramOK = true;
			tempPoint.x = point3DList[i].X * 100;
			tempPoint.y = point3DList[i].Z * 100;
			if (pointZOX.size())
			{
				leftIndex = 0;
				rightIndex = pointZOX.size() - 1;
				if (tempPoint.x < pointZOX[0].x)
				{
					pointZOX.insert(pointZOX.begin(), tempPoint);
					tempPoint.y = point3DList[i].Z * 100;
				}
				else if (tempPoint.x>pointZOX[rightIndex].x)
				{
					pointZOX.push_back(tempPoint);
					tempPoint.y = point3DList[i].Z * 100;
				}
				else
				{
					while (rightIndex - leftIndex > 1)
					{
						mindleIndex = (leftIndex + rightIndex) / 2;
						if (tempPoint.x >= pointZOX[mindleIndex].x) leftIndex = mindleIndex;
						else rightIndex = mindleIndex;
					}
					pointZOX.insert(pointZOX.begin() + leftIndex + 1, tempPoint);
					tempPoint.y = point3DList[i].Z * 100;
				}

			}
			else
			{
				pointZOX.push_back(tempPoint);
				tempPoint.y = point3DList[i].Z * 100;
			}
	}
	for (int i = 0; i < point3DList.size(); i++)
	{

		tempPoint.x = point3DList[i].Y * 100;
		tempPoint.y = point3DList[i].Z * 100;
		if (pointZOY.size())
		{
			leftIndex = 0;
			rightIndex = pointZOY.size() - 1;
			if (tempPoint.x < pointZOY[0].x)
			{
				pointZOY.insert(pointZOY.begin(), tempPoint);
				tempPoint.y = point3DList[i].Z * 100;
			}
			else if (tempPoint.x>pointZOY[rightIndex].x)
			{
				pointZOY.push_back(tempPoint);
				tempPoint.y = point3DList[i].Z * 100;
			}
			else
			{
				while (rightIndex - leftIndex > 1)
				{
					mindleIndex = (leftIndex + rightIndex) / 2;
					if (tempPoint.x >= pointZOY[mindleIndex].x) leftIndex = mindleIndex;
					else rightIndex = mindleIndex;
				}
				pointZOY.insert(pointZOY.begin() + leftIndex + 1, tempPoint);
				tempPoint.y = point3DList[i].Z * 100;
			}

		}
		else
		{
			pointZOY.push_back(tempPoint);
			tempPoint.y = point3DList[i].Z * 100;
		}
	}
}
bool CORBTrackingDlg::GetMinValue(vector<CPoint> pointList,bool bYFlag, int *minValue)
{
	if (pointList.size())
	{
		if (bYFlag)
		{
			*minValue = pointList[0].y;
			for (int i = 1; i < pointList.size(); i++)
			{
				if (pointList[i].y < *minValue) *minValue = pointList[i].y;
			}
		}
		else
		{
			*minValue = pointList[0].x;
			for (int i = 1; i < pointList.size(); i++)
			{
				if (pointList[i].x < *minValue) *minValue = pointList[i].x;
			}
		}
		return true;
	}
	return false;
}
bool CORBTrackingDlg::GetMaxValue(vector<CPoint> pointList,bool bYFlag, int *maxValue)
{
	if (pointList.size())
	{
		if (bYFlag)
		{
			*maxValue = pointList[0].y;
			for (int i = 1; i < pointList.size(); i++)
			{
				if (pointList[i].y > *maxValue) *maxValue = pointList[i].y;
			}
		}
		else
		{
			*maxValue = pointList[0].x;
			for (int i = 1; i < pointList.size(); i++)
			{
				if (pointList[i].x > *maxValue) *maxValue = pointList[i].x;
			}
		}
		return true;
	}
	return false;
}
void CORBTrackingDlg::GetPointSets(vector<CTrackLine> trackList, vector<POINT3D> &pointList, float xScale, float yScale, float zScale)
{
	POINT3D temp3DPoint;
	pointList.clear();
	for (int i = 0; i < trackList.size(); i++)
	{
		if (trackList[i].pointList.size()>(TrackNodeEqu5 - 1))
		{
			temp3DPoint.X = trackList[i].realCoord[0]*xScale;
			temp3DPoint.Y = trackList[i].realCoord[1]*yScale;
			temp3DPoint.Z = trackList[i].realCoord[2]*zScale;
			pointList.push_back(temp3DPoint);
		}
	}
}
void CORBTrackingDlg::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_checkTrack1)
	{
		m_checkTrack2 = false;

		m_checkROI = false;
		m_checkTrack = false;
	}
	UpdateData(false);
}


void CORBTrackingDlg::OnBnClickedCheck2()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_checkTrack2)
	{
		m_checkTrack1 = false;

		m_checkROI = false;
		m_checkTrack = false;
	}
	UpdateData(false);
}


void CORBTrackingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	if (mImageHeight)
	{
		CRect rect;
		GetDlgItem(IDC_PicMatchImage)->GetClientRect(rect);
		//ClientToScreen(&point);

		CWnd* pWnd = GetDlgItem(IDC_PicMatchImage);
		CDC* pDC = pWnd->GetDC();

		CPoint imgPoint;
		imgPoint.x = (point.x - rect.left)*m_showJointImage->width / rect.Width();
		imgPoint.y = (point.y - rect.top)*m_showJointImage->height / rect.Height();

		if (m_checkROI)
		{
			m_trackROI.left = imgPoint.x;
			m_trackROI.right = imgPoint.x;
			m_trackROI.top = imgPoint.y;
			m_trackROI.bottom = imgPoint.y;

			m_dropFlag = true;
		}
		/////////////////////////////
		if (m_checkTrack1 || m_checkTrack2)
		{
			CPoint tempPoint;
			tempPoint.x = imgPoint.x - m_roi.right;
			tempPoint.y = imgPoint.y;
			float dis, minDis = 100;
			int minDisIndex;
			CPoint pnt;
			for (int i = 0; i < m_niceTrackList.size(); i++)
			{
				pnt.x = m_niceTrackList[i].pointList[m_niceTrackList[i].pointList.size() - 1].x;
				pnt.y = m_niceTrackList[i].pointList[m_niceTrackList[i].pointList.size() - 1].y;
				dis = pow((tempPoint.x - pnt.x), 2) + pow((tempPoint.y - pnt.y), 2);
				dis = pow(dis, 0.5);
				if (dis < minDis)
				{
					minDis = dis;
					minDisIndex = i;
				}
			}
			if (minDis < 5)
			{
				DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
				if (m_checkTrack1)
				{
					m_trackPointList1 = m_niceTrackList[minDisIndex].pointList;
					//ShowTrackLine(m_trackPointList1, 1);
					OnBnClickedCheck12();
				}
				else
				{
					m_trackPointList2 = m_niceTrackList[minDisIndex].pointList;
					//ShowTrackLine(m_trackPointList2, 2);
					OnBnClickedCheck12();
				}
				for (int i = 0; i < m_niceTrackList.size(); i++)
				{
					tempPoint.x = m_niceTrackList[i].pointList[m_niceTrackList[i].pointList.size() - 1].x + m_roi.right;
					tempPoint.y = m_niceTrackList[i].pointList[m_niceTrackList[i].pointList.size() - 1].y;
					DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);
				}
				DrawLine(m_trackPointList1, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 0), 2);
				DrawLine(m_trackPointList2, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);
			}
		}
	}


	if (m_checkMouseControl)
	{
		UpdateData(true);
		m_openGL.OnLButtonDown(point);
		::SetFocus(m_hWnd);
		UpdateData(false);
		//m_openGL.RenderScene(m_point3DList);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CORBTrackingDlg::OnBnClickedCheck3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_checkProbe)
	{
		if (m_trackPointList1.size() < TrackNodeEqu5 || m_trackPointList2.size() < TrackNodeEqu5)
		{

		}
		else
		{
			int index1 = m_trackPointList1.size() - 1;
			int index2 = m_trackPointList2.size() - 1;

			float imgPnt1[2], imgPnt2[2];
			float imgPnt12[2], imgPnt22[2];
			float realCoord1[3], realCoord2[3];
			float realCoord12[3], realCoord22[3];
			float scale1, scale2;
			CPoint tempPoint;

			float offset1, offset2;
			cv::Size size(400, 400);
			cv::Mat img;
			img.create(size, CV_8UC1);
			float accOffset[400 * 400];

			index1 = m_trackPointList1.size() - 1;
			index2 = m_trackPointList2.size() - 1;
			imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
			imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
			imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
			imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;

			imgPnt12[0] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt12[1] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].y + m_roi.bottom;
			imgPnt22[0] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt22[1] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].y + m_roi.bottom;
			///////////////////////////////////////////
			////////////////////////////
			float point[3][3];
			point[0][0] = 0; point[0][1] = 0;
			point[1][0] = 0; point[1][1] = 200;
			point[2][0] = 200; point[2][1] = 200;
			float offsetO[3];
			/////////////////////////////
			for (int i = 0; i < 3; i++)
			{
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[i][0]*0.01, realCoord1);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, point[i][0]*0.01, realCoord12);
				offset1 = pow((realCoord1[0] - realCoord12[0]), 2) + pow((realCoord1[1] - realCoord12[1]), 2);
				offset1 = pow(offset1, 0.5);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[i][1] * 0.01, realCoord2);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, point[i][1] * 0.01, realCoord22);
				offset2 = pow((realCoord2[0] - realCoord22[0]), 2) + pow((realCoord2[1] - realCoord22[1]), 2);
				offset2 = pow(offset2, 0.5);
				offsetO[i] = offset1 - offset2;
			///////////////	
				offset1 = realCoord1[0] - realCoord2[0];
				offset2 = realCoord12[0] - realCoord22[0];
				for (int k = 1; k < 3; k++)
				{
					offset1 += (realCoord1[k] - realCoord2[k]);
					offset2 += (realCoord12[k] - realCoord22[k]);
				}
				point[i][2] = offset1 - offset2;
			}
			float A, B, C;
			C = point[0][2];
			B = (point[1][2] - C) / 2.0;
			A = (point[2][2] - C - B*2.0) / 2.0;
			CString tempStr;
			//tempStr.Format(_T("Hg = %.3f Hr + %.3f"), -B / A, -C / A);
			tempStr.Format(_T("%.3f Hg + %.3f Hr + %.3f"),A,B,C);
			GetDlgItem(IDC_StaABEquation_M)->SetWindowText(tempStr);
			C = offsetO[0];
			B = (offsetO[1] - C) / 2.0;
			A = (offsetO[2] - C - B*2.0) / 2.0;
			//tempStr.Format(_T("Hg = %.3f Hr + %.3f"), -B / A, -C / A);
			tempStr.Format(_T("%.3f Hg + %.3f Hr + %.3f"), A, B, C);
			GetDlgItem(IDC_StaABEquation_O)->SetWindowText(tempStr);


			for (int H1 = 0; H1 < 400; H1++)
			{
				for (int H2 = 0; H2 < 400; H2++)
				{
					accOffset[H1 * 400 + H2] = A*H1*0.01 + B*H2*0.01 + C;
				}
			}
			float maxValue = accOffset[0]>0 ? accOffset[0] : (-accOffset[0]);
			int index[3] = { 399, 399 * 400, 399 * 400 + 399 };
			float value;
			for (int i = 0; i<3; i++)
			{
				value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
				if (value > maxValue)	maxValue = value;
			}
			for (int i = 0; i < 400; i++)
			{
				for (int j = 0; j < 400; j++)
				{
					if (m_checkRaw)
					{
						img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
					}
					else
					{
					    img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j]*127/maxValue);
					}

				}
			}
			imwrite("offsetDiffABAB_O.jpg", img);
			/////////////////////////////////////////
			//for (int H1 = 0; H1 < 400; H1++)
			//{
			//	for (int H2 = 0; H2 < 400; H2++)
			//	{
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, H1 * 0.01, realCoord1);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, H1 * 0.01, realCoord12);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, H2 * 0.01, realCoord2);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, H2 * 0.01, realCoord22);
			//		offset1 = 0;
			//		offset2 = 0;
			//		for (int k = 0; k < 3; k++)
			//		{
			//			offset1 += (realCoord1[k] - realCoord12[k]);
			//			offset2 += (realCoord2[k] - realCoord22[k]);
			//		}

			//		accOffset[H1 * 400 + H2] = offset1 - offset2;
			//	}
			//}

		 //   maxValue = accOffset[0]>0 ? accOffset[0] : (-accOffset[0]);
			//for (int i = 0; i<3; i++)
			//{
			//	value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
			//	if (value > maxValue)	maxValue = value;
			//}
			//for (int i = 0; i < 400; i++)
			//{
			//	for (int j = 0; j < 400; j++)
			//	{
			//		if (m_checkRaw)
			//		{
			//			img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
			//		}
			//		else
			//		{
   //                     img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j]*127/maxValue);
			//		}
			//		
			//	}
			//}
			//imwrite("offsetDiffABAB_M.jpg", img);
			/////////////
			img.release();
			/////////////////////////////////////////////
		}
	}
	UpdateData(false);
}

void CORBTrackingDlg::OnBnClickedCheck4()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkTrackSpeed)
	{
		if (m_trackPointList1.size() < TrackNodeEqu5 || m_trackPointList2.size() < TrackNodeEqu5)
		{

		}
		else
		{
			int index1 = m_trackPointList1.size() - 1;
			int index2 = m_trackPointList2.size() - 1;

			float imgPnt1[2], imgPnt2[2];
			float imgPnt12[2], imgPnt22[2];
			float realCoord1[3], realCoord2[3];
			float realCoord12[3], realCoord22[3];
			float scale1, scale2;
			CPoint tempPoint;

			float offset1, offset2;
			cv::Size size(400, 400);
			cv::Mat img;
			img.create(size, CV_8UC1);
			float accOffset[400 * 400];

			index1 = m_trackPointList1.size() - 1;
			index2 = m_trackPointList2.size() - 1;
			imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
			imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
			imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
			imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;

			imgPnt12[0] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt12[1] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].y + m_roi.bottom;
			imgPnt22[0] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt22[1] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].y + m_roi.bottom;
			///////////////////////////////////////////
			////////////////////////////
			float point[3][3];
			point[0][0] = 0; point[0][1] = 0;
			point[1][0] = 0; point[1][1] = 200;
			point[2][0] = 200; point[2][1] = 200;
			/////////////////////////////
			for (int i = 0; i < 3; i++)
			{
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[i][0] * 0.01, realCoord1);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, point[i][0] * 0.01, realCoord12);
				offset1 = pow((realCoord1[0] - realCoord12[0]), 2) + pow((realCoord1[1] - realCoord12[1]), 2);
				offset1 = pow(offset1, 0.5);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[i][1] * 0.01, realCoord2);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, point[i][1] * 0.01, realCoord22);
				offset2 = pow((realCoord2[0] - realCoord22[0]), 2) + pow((realCoord2[1] - realCoord22[1]), 2);
				offset2 = pow(offset2, 0.5);
				point[i][2] = offset1 - offset2;
			}
			float A, B, C;
			C = point[0][2];
			B = (point[1][2] - C) / 2.0;
			A = (point[2][2] - C - B*2.0) / 2.0;
			CString tempStr;
			tempStr.Format(_T("%.2f Hg + %.2f Hr + %.2f = 0"), A, B, C);
			GetDlgItem(IDC_StaLeftEquation)->SetWindowText(tempStr);

			float T1Height, T2Height;
			m_pointDisAA.clear();
			for (int i = 0; i < 200; i++)
			{
				T1Height = i*0.01;
				//T2Height = (-C - A*T1Height) / B;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, T1Height, realCoord1);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, T1Height, realCoord12);
				offset1 = pow((realCoord1[0] - realCoord12[0]), 2) + pow((realCoord1[1] - realCoord12[1]), 2);
				offset1 = pow(offset1, 0.5);
				tempPoint.x = i;
				tempPoint.y = offset1 / 0.16 * 3.6;
				m_pointDisAA.push_back(tempPoint);
			}
			m_historgramHeightDiffError.SetLimit(0, 200, 0, 100);
			m_historgramHeightDiffError.m_pointList = m_pointDisAA;
			m_historgramHeightDiffError.m_bShowEXDX = true;
			m_historgramHeightDiffError.RenderScene();
		}
	}
	UpdateData(FALSE);
}



void CORBTrackingDlg::OnBnClickedCheck5()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	UpdateData(false);
}


void CORBTrackingDlg::OnBnClickedCheck6()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_checkTrack&&m_trackList.size())
	{
		int TrackMinLength = TrackNodeEqu5;
		CPoint pnt;
		m_roiTrackList.clear();
		CvPoint pt1, pt2;
		for (int i = 0; i < m_trackList.size(); i++)
		{
			pnt.x = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].x + m_roi.right;
			pnt.y = m_trackList[i].pointList[m_trackList[i].pointList.size() - 1].y;
			if (pnt.x>m_trackROI.left&&pnt.x<m_trackROI.right&&pnt.y>m_trackROI.bottom&&pnt.y < m_trackROI.top)
			{
				if (m_trackList[i].pointList.size()>(TrackMinLength-1))
				{
					for (int nNo = 1; nNo < m_trackList[i].pointList.size(); nNo++)
					{
						pt1.x = m_trackList[i].pointList[nNo - 1].x + m_roi.right;
						pt1.y = m_trackList[i].pointList[nNo - 1].y;
						pt2.x = m_trackList[i].pointList[nNo].x + m_roi.right;
						pt2.y = m_trackList[i].pointList[nNo].y;
						cvLine(m_showJointImage, pt1, pt2, CV_RGB(0, 255, 0));
					}
					m_roiTrackList.push_back(m_trackList[i]);
				    
				}
			}
		}
		vector<Point> points;
		Point pt;
		for (int i = 0; i < m_roiTrackList.size(); i++)
		{	
			pt.x = m_roiTrackList[i].pointList[m_roiTrackList[i].pointList.size() - 1].x + m_roi.right;
			pt.y = m_roiTrackList[i].pointList[m_roiTrackList[i].pointList.size() - 1].y;
	 	    points.push_back(pt);
		}
		vector<int> hull;
		convexHull(Mat(points), hull, true);
		int hullcount = (int)hull.size();
		Point pt0 = points[hull[hullcount - 1]];
		for (int i = 0; i < hullcount; i++)
		{
			Point pt = points[hull[i]];
			cvLine(m_showJointImage, pt0, pt, CV_RGB(255, 0, 0),4);
			pt0 = pt;
		}
	//////////////////
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		DrawRect(m_trackROI.left, m_trackROI.right, m_trackROI.top, m_trackROI.bottom, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 0, 255), 2);
		////////////////////////////////
		CPoint tempPoint;
		for (int i = 0; i < m_roiTrackList.size(); i++)
		{
			tempPoint.x = m_roiTrackList[i].pointList[m_roiTrackList[i].pointList.size() - 1].x + m_roi.right;
			tempPoint.y = m_roiTrackList[i].pointList[m_roiTrackList[i].pointList.size() - 1].y;
			DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);
		}
		/////////////////////////////////////	
		float minSpeed = 100;
		float speed;
		int minSpeedIndex;
		int index;
		int imgPnt1[2], imgPnt2[2];
		float realCoord1[3], realCoord2[3];
		for (int i = 0; i < m_roiTrackList.size(); i++)
		{
			index = m_roiTrackList[i].pointList.size() - 1;
			imgPnt1[0] = m_roiTrackList[i].pointList[index].x + m_roi.left;
			imgPnt1[1] = m_roiTrackList[i].pointList[index].y + m_roi.bottom;
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, 0, realCoord1);
			imgPnt2[0] = m_roiTrackList[i].pointList[index - TrackMinLength + 1].x + m_roi.left;
			imgPnt2[1] = m_roiTrackList[i].pointList[index - TrackMinLength + 1].y + m_roi.bottom;
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, 0, realCoord2);

			speed = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);

			if (speed < minSpeed)
			{
				minSpeed = speed;
				minSpeedIndex = i;
			}

		}
		index = m_roiTrackList[minSpeedIndex].pointList.size() - 1;
		imgPnt1[0] = m_roiTrackList[minSpeedIndex].pointList[index].x + m_roi.left;
		imgPnt1[1] = m_roiTrackList[minSpeedIndex].pointList[index].y + m_roi.bottom;;
		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, 0, realCoord1);
		m_roiTrackList[minSpeedIndex].height = 0;
		for (int i = 0; i < 3; i++) 	m_roiTrackList[minSpeedIndex].realCoord[i] = realCoord1[i];

		vector<POINT3D> point3DList;
		POINT3D temp3DPoint;
		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, 0, realCoord1);
		temp3DPoint.X = realCoord1[0];
		temp3DPoint.Y = realCoord1[1];
		temp3DPoint.Z = realCoord1[2];
		point3DList.push_back(temp3DPoint);

		//////////////
		tempPoint.x = imgPnt1[0] - m_roi.left + m_roi.right;
		tempPoint.y = imgPnt1[1] - m_roi.bottom;
		DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 4);
		//////////////

		float point[3][3];
		point[0][0] = 0; point[0][1] = 0;
		point[1][0] = 0; point[1][1] = 2.0;
		point[2][0] = 2.0; point[2][1] = 2.0;
		float offset1[3], offset2;
		index = m_roiTrackList[minSpeedIndex].pointList.size() - 1;
		imgPnt1[0] = m_roiTrackList[minSpeedIndex].pointList[index].x + m_roi.left;
		imgPnt1[1] = m_roiTrackList[minSpeedIndex].pointList[index].y + m_roi.bottom;
		imgPnt2[0] = m_roiTrackList[minSpeedIndex].pointList[index - TrackMinLength + 1].x + m_roi.left;
		imgPnt2[1] = m_roiTrackList[minSpeedIndex].pointList[index - TrackMinLength + 1].y + m_roi.bottom;
		for (int k = 0; k < 3; k++)
		{
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[k][0], realCoord1);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[k][0], realCoord2);
			offset1[k] = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);
			offset1[k] = pow(offset1[k], 0.5);
		}
		float A, B, C;
		float xMin = 100;
		float xMax = -100;
		float yMin = 100;
		float yMax = -100;
		for (int i = 0; i < m_roiTrackList.size(); i++)
		{
			if (i != minSpeedIndex)
			{
				index = m_roiTrackList[i].pointList.size() - 1;
				imgPnt1[0] = m_roiTrackList[i].pointList[index].x + m_roi.left;
				imgPnt1[1] = m_roiTrackList[i].pointList[index].y + m_roi.bottom;
				imgPnt2[0] = m_roiTrackList[i].pointList[index - TrackMinLength + 1].x + m_roi.left;
				imgPnt2[1] = m_roiTrackList[i].pointList[index - TrackMinLength + 1].y + m_roi.bottom;
				for (int k = 0; k < 3; k++)
				{
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[k][1], realCoord1);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[k][1], realCoord2);
					offset2 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2);
					offset2 = pow(offset2, 0.5);
					point[k][2] = offset1[k] - offset2;
				}
				C = point[0][2];
				B = (point[1][2] - C) / 2.0;
				A = (point[2][2] - C - B*2.0) / 2.0;
				m_roiTrackList[i].height = -C / B;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, m_roiTrackList[i].height, realCoord1);
				for (int k = 0; k < 3; k++) m_roiTrackList[i].realCoord[k] = realCoord1[k];

				temp3DPoint.X = realCoord1[0];
				temp3DPoint.Y = realCoord1[1];
				temp3DPoint.Z = realCoord1[2];
				if (xMax < temp3DPoint.X) xMax = temp3DPoint.X;
				if (xMin > temp3DPoint.X) xMin = temp3DPoint.X;
				if (yMax < temp3DPoint.Y) yMax = temp3DPoint.Y;
				if (yMin > temp3DPoint.Y) yMin = temp3DPoint.Y;
				point3DList.push_back(temp3DPoint);

				if (realCoord1[2] < 0)
				{
					tempPoint.x = imgPnt1[0] - m_roi.left + m_roi.right;
					tempPoint.y = imgPnt1[1] - m_roi.bottom;
					DrawPoint(tempPoint, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 255, 0), 3);
				}
			}
		}
		char* pFileName = new char[512];
		strcpy(pFileName, "pointSet.dat");
		if (SavePointSet(pFileName, point3DList))
		{
			MessageBox(_T("save success"));
		}
		delete[] pFileName;

		SectionalProjectionAnalyseZO(point3DList, m_pointZOX, m_pointZOY);
		int zMinValue;
		GetMinValue(m_pointZOX, true, &zMinValue);
		for (int i = 0; i < m_pointZOX.size(); i++)
		{
			m_pointZOX[i].y -= zMinValue;
			m_pointZOY[i].y -= zMinValue;
		}
		if (m_historgramOK)
		{
			m_historgramZOX.SetLimit(m_pointZOX[0].x, m_pointZOX[m_pointZOX.size() - 1].x, 0, 300);
			m_historgramZOX.m_pointList = m_pointZOX;

			hull.clear();
			points.clear();
			for (int i = 0; i < m_pointZOX.size(); i++)
			{
				pt.x = m_pointZOX[i].x;
				pt.y = m_pointZOX[i].y;
				points.push_back(pt);
			}
			convexHull(Mat(points), hull, true);
			hullcount = (int)hull.size();
			vector<CPoint> pointList;
			for (int i = 0; i < hullcount; i++)
			{
				tempPoint.x = points[hull[i]].x;
				tempPoint.y = points[hull[i]].y;
				pointList.push_back(tempPoint);
			}
			m_historgramZOX.RenderScene();
			m_historgramZOX.DrawLine(pointList, PS_DOT, 3, RGB(255, 0, 0));


			hull.clear();
			points.clear();
			for (int i = 0; i < m_pointZOY.size(); i++)
			{
				pt.x = m_pointZOY[i].x;
				pt.y = m_pointZOY[i].y;
				points.push_back(pt);
			}
			convexHull(Mat(points), hull, true);
			hullcount = (int)hull.size();
			for (int i = 0; i < hullcount; i++)
			{
				tempPoint.x = points[hull[i]].x;
				tempPoint.y = points[hull[i]].y;
				pointList.push_back(tempPoint);
			}
			m_historgramZOY.SetLimit(m_pointZOY[0].x, m_pointZOY[m_pointZOY.size() - 1].x, 0, 300);
			m_historgramZOY.m_pointList = m_pointZOY;
			m_historgramZOY.RenderScene();
			m_historgramZOY.DrawLine(pointList, PS_DOT, 3, RGB(255, 0, 0));
		}
	}

	UpdateData(false);
}
bool CORBTrackingDlg::SavePointSet(const char* path, vector<POINT3D> pointList)
{
	int size = pointList.size();
	FILE *stream;
	if ((stream = fopen(path, "w+b")) != NULL)
	{
		fwrite(&size, sizeof(int), 1, stream);
		for (int i = 0; i < size; i++)
		{
			fwrite(&pointList[i], sizeof(POINT3D), 1, stream);
		}
	}
	else
	{
		return false;
	}
	fclose(stream);
	return true;
}
void CORBTrackingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);

	if (m_checkROI&&m_dropFlag)
	{
		CRect rect;
		GetDlgItem(IDC_PicMatchImage)->GetClientRect(rect);
		//ClientToScreen(&point);

		CWnd* pWnd = GetDlgItem(IDC_PicMatchImage);
		CDC* pDC = pWnd->GetDC();

		CPoint imgPoint;
		imgPoint.x = (point.x - rect.left)*m_showJointImage->width / rect.Width();
		imgPoint.y = (point.y - rect.top)*m_showJointImage->height / rect.Height();

		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		DrawRect(m_trackROI.left, imgPoint.x, m_trackROI.top, imgPoint.y, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 0, 255), 2);

		pDC->DeleteDC();
	}


	if (m_checkMouseControl)
	{
		UpdateData(true);
		m_openGL.OnMouseMove(point);
		UpdateData(false);
		m_openGL.RenderScene();
	}

	UpdateData(false);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CORBTrackingDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_checkROI&&m_dropFlag)
	{
		CRect rect;
		GetDlgItem(IDC_PicMatchImage)->GetClientRect(rect);
		//ClientToScreen(&point);

		CWnd* pWnd = GetDlgItem(IDC_PicMatchImage);
		CDC* pDC = pWnd->GetDC();

		CPoint imgPoint;
		imgPoint.x = (point.x - rect.left)*m_showJointImage->width / rect.Width();
		imgPoint.y = (point.y - rect.top)*m_showJointImage->height / rect.Height();

		if (imgPoint.x > m_trackROI.right) m_trackROI.right = imgPoint.x;
		else if (imgPoint.x < m_trackROI.left) m_trackROI.left = imgPoint.x;

		if (imgPoint.y > m_trackROI.top) m_trackROI.top = imgPoint.y;
		else if (imgPoint.y < m_trackROI.bottom) m_trackROI.bottom = imgPoint.y;

		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		DrawRect(m_trackROI.left, m_trackROI.right, m_trackROI.top, m_trackROI.bottom, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 0, 255), 2);

		pDC->DeleteDC();
	}

	m_dropFlag = false;


	if (m_checkMouseControl)
	{
		UpdateData(true);
		m_openGL.OnLButtonUp(point);
		UpdateData(false);
		m_openGL.RenderScene();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CORBTrackingDlg::OnBnClickedCheck7()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	UpdateData(false);
}


void CORBTrackingDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_checkMouseControl)
	{
		UpdateData(true);
		m_openGL.OnMouseWheel(zDelta);
		UpdateData(false);
		m_openGL.RenderScene();
	}
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CORBTrackingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	if (m_checkMouseControl)
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			m_openGL.OnKeyDown(pMsg->wParam);
			::SetFocus(m_hWnd);
			break;
		default:break;
		}
		m_openGL.RenderScene();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CORBTrackingDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (m_checkMouseControl)
	{
		m_openGL.circle_R = m_sliderGLCircleR*0.1;
		m_openGL.RenderScene();
	}
	
	UpdateData(true);
	*pResult = 0;
}


void CORBTrackingDlg::OnBnClickedCheck8()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (m_checkProbe2)
	{
		if (m_trackPointList1.size() < TrackNodeEqu5 || m_trackPointList2.size() < TrackNodeEqu5)
		{

		}
		else
		{
			int index1 = m_trackPointList1.size() - 1;
			int index2 = m_trackPointList2.size() - 1;
			float imgPnt1[2], imgPnt2[2];
			float imgPnt12[2], imgPnt22[2];
			float realCoord1[3], realCoord2[3];
			float realCoord12[3], realCoord22[3];
			float scale1, scale2;
			CPoint tempPoint;

			float offset1, offset2;
			cv::Size size(400, 400);
			cv::Mat img;
			img.create(size, CV_8UC1);
			float accOffset[400 * 400];

			index1 = m_trackPointList1.size() - 1;
			index2 = m_trackPointList2.size() - 1;
			imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
			imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
			imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
			imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;

			imgPnt12[0] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt12[1] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].y + m_roi.bottom;
			imgPnt22[0] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].x + m_roi.left;
			imgPnt22[1] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].y + m_roi.bottom;

			for (int h1 = 0; h1 < 400; h1++)
			{
				for (int h2 = 0; h2 < 400; h2++)
				{
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, h1*0.01, realCoord1);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, h1*0.01, realCoord12);

					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, h2*0.01, realCoord2);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, h2*0.01, realCoord22);

					offset1 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
					offset1 = pow(offset1, 0.5);

					offset2 = pow((realCoord12[0] - realCoord22[0]), 2) + pow((realCoord12[1] - realCoord22[1]), 2) + pow((realCoord12[2] - realCoord22[2]), 2);
					offset2 = pow(offset2, 0.5);
					
					accOffset[h1 * 400 + h2] = (offset1 - offset2);
				}
			}
			float maxValue = accOffset[0]>0 ? accOffset[0] : (-accOffset[0]);
			int index[3] = { 399, 399 * 400, 399 * 400 + 399 };
			float value;
			for (int i = 0; i<3; i++)
			{
				value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
				if (value > maxValue)	maxValue = value;
			}
			for (int i = 0; i < 400; i++)
			{
				for (int j = 0; j < 400; j++)
				{
					if (m_checkRaw)
					{
						img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
					}
					else
					{
						img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j] * 127 / maxValue);
					}
				}
			}
			imwrite("offsetDiffAABB_O.jpg", img);
/////////////////////////////////////////////////////////////
			float leftLineY[2] = { -999, -999 };
			float rightLineY[2] = { -999, -999 };
			float H1[2] = { 0, 4 };
			int H2;
			for (int w = 0; w < 2; w++)
			{
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, H1[w], realCoord1);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, H1[w], realCoord12);
				offset1 = 100, offset2 = 0;
				float offset;

				for (H2 = -800; H2 < 800; H2++)
				{
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, H2*0.01, realCoord2);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, H2*0.01, realCoord22);
					offset1 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
					offset1 = pow(offset1, 0.5);
					offset2 = pow((realCoord12[0] - realCoord22[0]), 2) + pow((realCoord12[1] - realCoord22[1]), 2) + pow((realCoord12[2] - realCoord22[2]), 2);
					offset2 = pow(offset2, 0.5);
					offset = offset1 - offset2;
					offset = offset > 0 ? offset : (-offset);
					if (offset < 0.001)
					{
					    leftLineY[w] = H2*0.01;
						break;
					}
				}

				offset1 = 100, offset2 = 0;
				for (H2 = 800; H2 > -800; H2--)
				{
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, H2*0.01, realCoord2);
					CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, H2*0.01, realCoord22);
					offset1 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
					offset1 = pow(offset1, 0.5);
					offset2 = pow((realCoord12[0] - realCoord22[0]), 2) + pow((realCoord12[1] - realCoord22[1]), 2) + pow((realCoord12[2] - realCoord22[2]), 2);
					offset2 = pow(offset2, 0.5);
					offset = offset1 - offset2;
					offset = offset > 0 ? offset : (-offset);
					if (offset < 0.001)
					{
						rightLineY[w] = H2*0.01;
						break;
					}
				}
			}
			float kl = -1, bl;
			float kr = -1, br;
			if (leftLineY[0] != -999 && leftLineY[1] != -999)
			{
				 kl = 4 / (leftLineY[1] - leftLineY[0]);
				 bl = 4 - kl*leftLineY[1];
				CString tempStr;
				tempStr.Format(_T("Hg = %.3f Hr + %.3f"), kl, bl);
				GetDlgItem(IDC_StaLeftEquation2)->SetWindowText(tempStr);
			}
			if (rightLineY[0] != -999 && rightLineY[1] != -999)
			{
			     kr = 4 / (rightLineY[1] - rightLineY[0]);
				 br = 4 - kr*rightLineY[1];
				CString tempStr;
				tempStr.Format(_T("Hg = %.3f Hr + %.3f"), kr, br);
				GetDlgItem(IDC_StaRightEquation)->SetWindowText(tempStr);
			}
			if (kl != -1 && kr != -1)
			{
				float Hr, Hg;
				Hr = (br - bl) / (kl - kr);
				Hg = kl*Hr + bl;
				CString tempStr;
				tempStr.Format(_T("(%.3f,%.2f)"), Hg, Hr);
				GetDlgItem(IDC_StaHgHr)->SetWindowText(tempStr);
			}
			/////////////
			//////////////////////////////
			//for (int h1 = 0; h1 < 400; h1++)
			//{
			//	for (int h2 = 0; h2 < 400; h2++)
			//	{
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, h1*0.01, realCoord1);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, h1*0.01, realCoord12);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, h2*0.01, realCoord2);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, h2*0.01, realCoord22);
			//		//offset1 = pow((realCoord1[0] - realCoord12[0]), 2) + pow((realCoord1[1] - realCoord12[1]), 2);
			//		//offset1 = pow(offset1, 0.5);
			//		//offset2 = pow((realCoord2[0] - realCoord22[0]), 2) + pow((realCoord2[1] - realCoord22[1]), 2);
			//		//offset2 = pow(offset2, 0.5);
			//		//accOffset[h1 * 400 + h2] = (offset1 - offset2);
			//		offset1 = 0;
			//		offset2 = 0;
			//		for (int k = 0; k < 3; k++)
			//		{
			//			offset1 += (realCoord1[k] - realCoord2[k]);
			//			offset2 += (realCoord12[k] - realCoord22[k]);
			//		}
			//		accOffset[h1 * 400 + h2] = (offset1 - offset2);
			//	}
			//}
		 //   maxValue = accOffset[0]>0 ? accOffset[0] : (-accOffset[0]);
			//for (int i = 0; i<3; i++)
			//{
			//	value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
			//	if (value > maxValue)	maxValue = value;
			//}
			//for (int i = 0; i < 400; i++)
			//{
			//	for (int j = 0; j < 400; j++)
			//	{
			//		if (m_checkRaw)
			//		{
			//			img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
			//		}
			//		else
			//		{
			//			img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j] * 127 / maxValue);
			//		}
			//	}
			//}
			//imwrite("offsetDiffAABB_M.jpg", img);
			////////////
			//float dis[TrackNodeEqu5];
			//float disAA;
			//float meanValue;
			//float variance;
			//float sum;
			//index1 = m_trackPointList1.size() - 1;
			//index2 = m_trackPointList2.size() - 1;
			//for (int h1 = 0; h1 < 400; h1++)
			//{
			//	for (int h2 = 0; h2 < 400; h2++)
			//	{
			//		meanValue = 0;
			//		for (int k = 0; k < TrackNodeEqu5; k++)
			//		{
			//			imgPnt1[0] = m_trackPointList1[index1 - k].x + m_roi.left;
			//			imgPnt1[1] = m_trackPointList1[index1 - k].y + m_roi.bottom;
			//			imgPnt2[0] = m_trackPointList2[index2 - k].x + m_roi.left;
			//			imgPnt2[1] = m_trackPointList2[index2 - k].y + m_roi.bottom;
			//			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, h1*0.01, realCoord1);
			//			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, h2*0.01, realCoord2);
			//			disAA = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
			//			disAA = pow(disAA, 0.5);
			//			dis[k] = disAA;
			//			meanValue += disAA;
			//		}
			//		meanValue /= TrackNodeEqu5;
			//		sum = 0;
			//		for (int k = 0; k < TrackNodeEqu5; k++)
			//		{
			//			sum += pow((dis[k] - meanValue), 2);
			//		}
			//		accOffset[h1 * 400 + h2] = sum / TrackNodeEqu5;
			//	}
			//}

			//maxValue = accOffset[0]>0 ? accOffset[0] : (-accOffset[0]);
			//for (int i = 0; i<3; i++)
			//{
			//	value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
			//	if (value > maxValue)	maxValue = value;
			//}
			//for (int i = 0; i < 400; i++)
			//{
			//	for (int j = 0; j < 400; j++)
			//	{
			//		if (m_checkRaw)
			//		{
			//			img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
			//		}
			//		else
			//		{
			//			img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j] * 127 / maxValue);
			//		}
			//	}
			//}
			//imwrite("offsetDiffAABB_Variance.jpg", img);
			/////////////////////////
			img.release();
		}
	}

	UpdateData(false);
}


void CORBTrackingDlg::OnBnClickedCheck9()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	UpdateData(false);
}


void CORBTrackingDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	float T1Height = m_sliderTgHeight*0.1;
	CString tempStr;
	tempStr.Format(_T("Tg H:%.2fm"), T1Height);
	GetDlgItem(IDC_StaTgHeight)->SetWindowText(tempStr);
	OnBnClickedCheck10();
	UpdateData(false);
	*pResult = 0;
}


void CORBTrackingDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	float T2Height = m_sliderTrHeight*0.1;
	CString tempStr;
	tempStr.Format(_T("Tr H:%.2fm"), T2Height);
	GetDlgItem(IDC_StaTrHeight)->SetWindowText(tempStr);
	OnBnClickedCheck10();
	UpdateData(false);
	*pResult = 0;
}


void CORBTrackingDlg::OnBnClickedCheck10()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (m_checkDisAA)
	{
		if (m_trackPointList1.size() && m_trackPointList2.size())
		{
			int index1 = m_trackPointList1.size() - 1;
			int index2 = m_trackPointList2.size() - 1;
			float T1Height = m_sliderTgHeight*0.1;
			float T2Height = m_sliderTrHeight*0.1;
			float dis;
			float maxDis = 0, minDis = 999;
			float imgPnt1[2], imgPnt2[2];
			float realCoord1[3], realCoord2[3];
			m_pointDisAA.clear();
			CPoint tempPoint;
			tempPoint.x = 0;
			while (index1 >= 0 && index2 >= 0)
			{
				imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
				imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
				imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
				imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, T1Height, realCoord1);
				CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, T2Height, realCoord2);
				dis = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
				dis = pow(dis, 0.5)*100;
				tempPoint.x++;
				tempPoint.y = dis;
				m_pointDisAA.push_back(tempPoint);
				if (dis > maxDis) maxDis = dis;
				if (dis < minDis) minDis = dis;
				index1--;
				index2--;
			}
			m_historgramDisAA.m_pointList = m_pointDisAA;
		//	m_historgramDisAA.SetLimit(0, 10, minDis*0.8, maxDis*1.1);
			m_historgramDisAA.m_bShowEXDX = true;
			m_historgramDisAA.SetLimit(0, 10, 0, 2000);
			m_historgramDisAA.RenderScene();
		}
	}

	UpdateData(false);
}


void CORBTrackingDlg::OnBnClickedCheck11()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	//if (m_checkIdealTrajectory)
	//{
	//	float realCoord1[3], realCoord2[3];
	//	float realCoord12[3], realCoord22[3];
	//	int imgPnt1[2], imgPnt2[2];
	//	int imgPnt12[2], imgPnt22[2];
	//	realCoord1[0] = -4; realCoord1[1] = 13; realCoord1[2] = 0.5;
	//	realCoord2[0] = -2; realCoord2[1] = 15; realCoord2[2] = 2.5;

	//	realCoord12[0] = -4; realCoord12[1] = 23; realCoord12[2] = 0.5;
	//	realCoord22[0] = -2; realCoord22[1] = 25; realCoord22[2] = 2.5;

	//	CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord1, imgPnt1);
	//	CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord12, imgPnt12);
	//	CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord2, imgPnt2);
	//	CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord22, imgPnt22);

	//	float offset1, offset2;
	//	cv::Size size(400, 400);
	//	cv::Mat img;
	//	img.create(size, CV_8UC1);
	//	float accOffset[400 * 400];
	//	for (int H1 = 0; H1 < 400; H1++)
	//	for (int H2 = 0; H2 < 400; H2++)
	//	{
	//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, H1*0.01, realCoord1);
	//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, H2*0.01, realCoord2);
	//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, H1*0.01, realCoord12);
	//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, H2*0.01, realCoord22);
	//		offset1 = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
	//		offset1 = pow(offset1, 0.5);
	//		offset2 = pow((realCoord12[0] - realCoord22[0]), 2) + pow((realCoord12[1] - realCoord22[1]), 2) + pow((realCoord12[2] - realCoord22[2]), 2);
	//		offset2 = pow(offset2, 0.5);
	//		accOffset[H1 * 400 + H2] = (offset1 - offset2);
	//	}

	//	float maxValue = accOffset[0] > 0 ? accOffset[0] : (-accOffset[0]);
	//	int index[3] = { 399, 399 * 400, 399 * 400 + 399 };
	//	float value;
	//	for (int i = 0; i<3; i++)
	//	{
	//		value = accOffset[index[i]]>0 ? accOffset[index[i]] : (-accOffset[index[i]]);
	//		if (value > maxValue)	maxValue = value;
	//	}
	//	for (int i = 0; i < 400; i++)
	//	{
	//		for (int j = 0; j < 400; j++)
	//		{
	//			if (m_checkRaw)
	//			{
	//				img.data[i * 400 + j] = 128 + (int)(10.0*accOffset[i * 400 + j]);
	//			}
	//			else
	//			{
	//				img.data[i * 400 + j] = 128 + (int)(1.0*accOffset[i * 400 + j] * 127 / maxValue);
	//			}
	//		}
	//	}

	//	imwrite("offsetDiffAABB_Ideal.jpg", img);
	//	img.release();
	//}

	UpdateData(false);
}


void CORBTrackingDlg::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_comboTrack.GetCurSel() == 1)
	{
		m_siftImg1->imageData = (char*)m_orbImg1.data;
		m_sift1.SiftFeatureExtract(m_siftImg1);
	}
	UpdateData(false);
}


void CORBTrackingDlg::OnNMCustomdrawSliderxstartpos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkIdealTrajectory)
	{
		CString tempStr;
		if (m_checkTrack1)
		{
			m_T1XStartCoord = (m_sliderXStartPos - 50)*0.2;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T1XStartCoord, m_T1YStartCoord, m_T1ZStartCoord, m_T1Speed);
			GetDlgItem(IDC_StaT1Info)->SetWindowText(tempStr);
		}
		else if (m_checkTrack2)
		{
			m_T2XStartCoord = (m_sliderXStartPos - 50)*0.2;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T2XStartCoord, m_T2YStartCoord, m_T2ZStartCoord, m_T2Speed);
			GetDlgItem(IDC_StaT2Info)->SetWindowText(tempStr);
		}
		POINT2D tempPoint;
		float imgPnt[2];
		float realCoord[3];
		float yStartCoord;
		if (m_checkTrack1)
		{
			m_trackPointList1.clear();
			realCoord[0] = m_T1XStartCoord;
			realCoord[1] = m_T1YStartCoord;
			realCoord[2] = m_T1ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T1Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList1.push_back(tempPoint);
			}
		}
		else if (m_checkTrack2)
		{
			m_trackPointList2.clear();
			realCoord[0] = m_T2XStartCoord;
			realCoord[1] = m_T2YStartCoord;
			realCoord[2] = m_T2ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T2Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0]-m_roi.left;
				tempPoint.y = imgPnt[1]-m_roi.bottom;
				m_trackPointList2.push_back(tempPoint);
			}
		}
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);

		DrawLine(m_trackPointList1, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 0), 2);
		DrawLine(m_trackPointList2, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);

		OnBnClickedCheck12();
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CORBTrackingDlg::OnNMCustomdrawSliderystartpos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkIdealTrajectory)
	{
		CString tempStr;
		if (m_checkTrack1)
		{
			m_T1YStartCoord = 10 + m_sliderYStartPos * 0.2;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T1XStartCoord, m_T1YStartCoord, m_T1ZStartCoord, m_T1Speed);
			GetDlgItem(IDC_StaT1Info)->SetWindowText(tempStr);
		}
		else if (m_checkTrack2)
		{
			m_T2YStartCoord = 10 + m_sliderYStartPos * 0.2;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T2XStartCoord, m_T2YStartCoord, m_T2ZStartCoord, m_T2Speed);
			GetDlgItem(IDC_StaT2Info)->SetWindowText(tempStr);
		}
		POINT2D tempPoint;
		float imgPnt[2];
		float realCoord[3];
		float yStartCoord;
		if (m_checkTrack1)
		{
			m_trackPointList1.clear();
			realCoord[0] = m_T1XStartCoord;
			realCoord[1] = m_T1YStartCoord;
			realCoord[2] = m_T1ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T1Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList1.push_back(tempPoint);
			}
		}
		else if (m_checkTrack2)
		{
			m_trackPointList2.clear();
			realCoord[0] = m_T2XStartCoord;
			realCoord[1] = m_T2YStartCoord;
			realCoord[2] = m_T2ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T2Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList2.push_back(tempPoint);
			}
		}
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);

		DrawLine(m_trackPointList1, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 0), 2);
		DrawLine(m_trackPointList2, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);
	
		OnBnClickedCheck12();
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CORBTrackingDlg::OnNMCustomdrawSliderzstartpos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkIdealTrajectory)
	{
		CString tempStr;
		if (m_checkTrack1)
		{
			m_T1ZStartCoord = m_sliderZStartPos*0.1;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T1XStartCoord, m_T1YStartCoord, m_T1ZStartCoord, m_T1Speed);
			GetDlgItem(IDC_StaT1Info)->SetWindowText(tempStr);
		}
		else if (m_checkTrack2)
		{
			m_T2ZStartCoord = m_sliderZStartPos*0.1;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T2XStartCoord, m_T2YStartCoord, m_T2ZStartCoord, m_T2Speed);
			GetDlgItem(IDC_StaT2Info)->SetWindowText(tempStr);
		}
		POINT2D tempPoint;
		float imgPnt[2];
		float realCoord[3];
		float yStartCoord;
		if (m_checkTrack1)
		{
			m_trackPointList1.clear();
			realCoord[0] = m_T1XStartCoord;
			realCoord[1] = m_T1YStartCoord;
			realCoord[2] = m_T1ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T1Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList1.push_back(tempPoint);
			}
		}
		else if (m_checkTrack2)
		{
			m_trackPointList2.clear();
			realCoord[0] = m_T2XStartCoord;
			realCoord[1] = m_T2YStartCoord;
			realCoord[2] = m_T2ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T2Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList2.push_back(tempPoint);
			}
		}
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		DrawLine(m_trackPointList1, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 0), 2);
		DrawLine(m_trackPointList2, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);

		OnBnClickedCheck12();
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CORBTrackingDlg::OnNMCustomdrawSliderspeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkIdealTrajectory)
	{
		CString tempStr;
		if (m_checkTrack1)
		{
			m_T1Speed = m_sliderSpeed;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T1XStartCoord, m_T1YStartCoord, m_T1ZStartCoord, m_T1Speed);
			GetDlgItem(IDC_StaT1Info)->SetWindowText(tempStr);
		}
		else if (m_checkTrack2)
		{
			m_T2Speed = m_sliderSpeed;
			tempStr.Format(_T("(%.2f, %.2f, %.2f, %.2f)"), m_T2XStartCoord, m_T2YStartCoord, m_T2ZStartCoord, m_T2Speed);
			GetDlgItem(IDC_StaT2Info)->SetWindowText(tempStr);
		}
		POINT2D tempPoint;
		float imgPnt[2];
		float realCoord[3];
		float yStartCoord;
		if (m_checkTrack1)
		{
			m_trackPointList1.clear();
			realCoord[0] = m_T1XStartCoord;
			realCoord[1] = m_T1YStartCoord;
			realCoord[2] = m_T1ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T1Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList1.push_back(tempPoint);
			}
		}
		else if (m_checkTrack2)
		{
			m_trackPointList2.clear();
			realCoord[0] = m_T2XStartCoord;
			realCoord[1] = m_T2YStartCoord;
			realCoord[2] = m_T2ZStartCoord;
			yStartCoord = realCoord[1];
			float ySpeed = m_T2Speed / 3.6;
			for (int i = 0; i < 10; i++)
			{
				realCoord[1] = yStartCoord + i*ySpeed*0.04;
				CMyTDMap::Calculate3DtoImg(m_TDMap, realCoord, imgPnt);
				tempPoint.x = imgPnt[0] - m_roi.left;
				tempPoint.y = imgPnt[1] - m_roi.bottom;
				m_trackPointList2.push_back(tempPoint);
			}
		}
		DrawPicToHDC(m_showJointImage, IDC_PicMatchImage);
		DrawLine(m_trackPointList1, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(0, 255, 0), 2);
		DrawLine(m_trackPointList2, m_showJointImage->height, m_showJointImage->width, IDC_PicMatchImage, RGB(255, 0, 0), 2);

		OnBnClickedCheck12();
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CORBTrackingDlg::OnBnClickedCheck12()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (m_trackPointList1.size() < TrackNodeEqu5 || m_trackPointList2.size() < TrackNodeEqu5)
	{

	}
	else
	{
		int index1 = m_trackPointList1.size() - 1;
		int index2 = m_trackPointList2.size() - 1;

		float imgPnt1[2], imgPnt2[2];
		float imgPnt12[2], imgPnt22[2];
		float realCoord1[3], realCoord2[3];
		float realCoord12[3], realCoord22[3];
		float scale1, scale2;
		CPoint tempPoint;

		float offset1, offset2;
		cv::Size size(400, 400);
		cv::Mat img;
		img.create(size, CV_8UC1);
		float accOffset[400 * 400];

		index1 = m_trackPointList1.size() - 1;
		index2 = m_trackPointList2.size() - 1;
		imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
		imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
		imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
		imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;

		imgPnt12[0] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].x + m_roi.left;
		imgPnt12[1] = m_trackPointList1[index1 - TrackNodeEqu5 + 1].y + m_roi.bottom;
		imgPnt22[0] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].x + m_roi.left;
		imgPnt22[1] = m_trackPointList2[index2 - TrackNodeEqu5 + 1].y + m_roi.bottom;
		///////////////////////////////////////////
		////////////////////////////
		float point[3][3];
		point[0][0] = 0; point[0][1] = 0;
		point[1][0] = 0; point[1][1] = 200;
		point[2][0] = 200; point[2][1] = 200;
		/////////////////////////////
		for (int i = 0; i < 3; i++)
		{
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, point[i][0] * 0.01, realCoord1);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt12, point[i][0] * 0.01, realCoord12);
			offset1 = pow((realCoord1[0] - realCoord12[0]), 2) + pow((realCoord1[1] - realCoord12[1]), 2);
			offset1 = pow(offset1, 0.5);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, point[i][1] * 0.01, realCoord2);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt22, point[i][1] * 0.01, realCoord22);
			offset2 = pow((realCoord2[0] - realCoord22[0]), 2) + pow((realCoord2[1] - realCoord22[1]), 2);
			offset2 = pow(offset2, 0.5);
			point[i][2] = offset1 - offset2;
		}
		float A, B, C;
		C = point[0][2];
		B = (point[1][2] - C) / 2.0;
		A = (point[2][2] - C - B*2.0) / 2.0;
		CString tempStr;
		//tempStr.Format(_T("%.2f Hg + %.2f Hr + %.2f = 0"), A, B, C);
		tempStr.Format(_T("Hg = %.2f Hr + %.2f"), -B / A, -C / A);
		GetDlgItem(IDC_StaLeftEquation)->SetWindowText(tempStr);

		float T1Height = -C / A;
		float T2Height;
		if (T1Height > 0) T2Height = 0;
		else
		{
			T1Height = 0;
			T2Height = -C / B;
		}
		index1 = m_trackPointList1.size() - 1;
		index2 = m_trackPointList2.size() - 1;
		float dis;
		float maxDis = 0, minDis = 999;
		m_pointDisAA.clear();
		tempPoint.x = 0;
		while (index1 >= 0 && index2 >= 0)
		{
			imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
			imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
			imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
			imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, T1Height, realCoord1);
			CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, T2Height, realCoord2);
			dis = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
			dis = pow(dis, 0.5) * 100;
			tempPoint.x++;
			tempPoint.y = dis;
			m_pointDisAA.push_back(tempPoint);
			if (dis > maxDis) maxDis = dis;
			if (dis < minDis) minDis = dis;
			index1--;
			index2--;
		}
		m_historgramDisAA.m_pointList = m_pointDisAA;
		m_historgramDisAA.m_bShowEXDX = true;
		m_historgramDisAA.SetLimit(0, 10, 0, 2000);
		m_historgramDisAA.RenderScene();

		if (m_checkIdealTrajectory)
		{
			float heightDiff = (m_T1ZStartCoord - m_T2ZStartCoord) * 100;
			m_pointHeightDiffError.clear();
			float heightError;
			for (int i = 0; i < 300; i++)
			{
				T2Height = 100 * (-C - A*i*0.01) / B;
				heightError = heightDiff - (i - T2Height);
				tempPoint.x = i;
				tempPoint.y = heightError;
				m_pointHeightDiffError.push_back(tempPoint);
			}
			m_historgramHeightDiffError.SetLimit(0, 300, -100, 100);
			m_historgramHeightDiffError.m_pointList = m_pointHeightDiffError;
			m_historgramHeightDiffError.RenderScene();

			//float meanValue, variance;
			//m_pointHeightDiffError.clear();
			//for (int i = 0; i < 300; i+=5)
			//{
			//	index1 = m_trackPointList1.size() - 1;
			//	index2 = m_trackPointList2.size() - 1;
			//	m_pointDisAA.clear();
			//	T1Height = i*0.01;
			//	T2Height = (-C - A*T1Height) / B;
			//	tempPoint.x = 0;
			//	while (index1 >= 0 && index2 >= 0)
			//	{
			//		imgPnt1[0] = m_trackPointList1[index1].x + m_roi.left;
			//		imgPnt1[1] = m_trackPointList1[index1].y + m_roi.bottom;
			//		imgPnt2[0] = m_trackPointList2[index2].x + m_roi.left;
			//		imgPnt2[1] = m_trackPointList2[index2].y + m_roi.bottom;
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt1, T1Height, realCoord1);
			//		CMyTDMap::CalculateImgto3D(m_TDMap, imgPnt2, T2Height, realCoord2);
			//		dis = pow((realCoord1[0] - realCoord2[0]), 2) + pow((realCoord1[1] - realCoord2[1]), 2) + pow((realCoord1[2] - realCoord2[2]), 2);
			//		dis = pow(dis, 0.5) * 100;
			//		tempPoint.x++;
			//		tempPoint.y = dis;
			//		m_pointDisAA.push_back(tempPoint);
			//		index1--;
			//		index2--;
			//	}
			//	meanValue = m_historgramHeightDiffError.GetMeanValue(m_pointDisAA);
			//	variance = m_historgramHeightDiffError.GetVariance(m_pointDisAA, meanValue);
			//	tempPoint.x = i;
			//	tempPoint.y = variance * 100;
			//	m_pointHeightDiffError.push_back(tempPoint);
			//}
			//m_historgramHeightDiffError.SetLimit(0, 300, 0, 100);
			//m_historgramHeightDiffError.m_pointList = m_pointHeightDiffError;
			//m_historgramHeightDiffError.RenderScene();
		}
	}

	UpdateData(false);
}
