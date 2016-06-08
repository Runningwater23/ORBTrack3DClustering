
// ORBTrackingDlg.h : 头文件
//

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <windows.h>

using namespace cv;

#include "CvvImage.h"
#include "common.h"
#include "MyORB.h"
#include "MyTDMap.h"
#include "MyHistorgramAnalyse.h"
#include "MyOpenGL.h"
//#include "kmeans.h"
#include "DBSCAN.h"
#include "afxwin.h"

#include "MySift.h"

typedef DWORD COLOR;
#define TrackNodeEqu5 5

// CORBTrackingDlg 对话框
class CORBTrackingDlg : public CDialogEx
{
public:
	struct ROI
	{
		int left;
		int right;
		int top;
		int bottom;
	};
	struct POINT2D
	{
		float x;
		float y;
	};
	/*
	struct POINT3D
	{
		float X;
		float Y;
		float Z;
	};*/
	class CTrackLine
	{
	public:
		vector<POINT2D> pointList;
		bool addNewNode;
		float height;
		float realCoord[3];
		float proSpeed;
		int clusterID;
		bool bRefeTrajectoryOK;
		bool bBadTrajectory;
	public:
		CTrackLine()
		{
			height = 0;
			bRefeTrajectoryOK = false;
			bBadTrajectory = false;
		}
	};
public:
	CString m_strFilePathName;
	CString m_fileName;//文件名，不带格式后缀
	int mImageHeight;
	int mImageWidth;

	IplImage *m_frame;
	CvCapture *m_capture;
	int m_nChannels;
	int m_nFrames;
	int m_totalFrames;

	cv::Mat m_image;
	///////////////////////
	ROI m_roi;
	cv::Mat m_orbImg1;
	cv::Mat m_orbImg2;
	cv::Mat m_jointImg;
	IplImage *m_siftImg1;
	IplImage *m_siftImg2;
	IplImage *m_showJointImage;
	////////////////////
	CMyORB m_myORB1;
	CMyORB m_myORB2;
	CMySift m_sift1;
	CMySift m_sift2;
	///////////////////
	vector<FeatureMatch> m_matchList;
	vector<CTrackLine> m_trackList;
	int m_vehicleCount;

	CTDMap m_TDMap;
	CTrackLine m_refeTrajectory;

	CMyHistorgram m_historgramZOX;
	vector<CPoint> m_pointZOX;
	CMyHistorgram m_historgramYOX;
	vector<CPoint> m_pointYOX;
	bool m_historgramOK;
	CMyHistorgram m_historgramZOY;
	vector<CPoint> m_pointZOY;
	
	CMyHistorgram m_historgramDisAA;
	vector<CPoint> m_pointDisAA;

	CMyHistorgram m_historgramHeightDiffError;
	vector<CPoint> m_pointHeightDiffError;

	vector<POINT3D> m_pointList;
	vector<int> m_pointIDList;
	CDBSCAN m_DBSCAN;

	float m_laneBorder1;
	float m_laneBorder2;
	/////////////////////
	vector<CTrackLine> m_niceTrackList;
	vector<POINT2D> m_trackPointList1;
	vector<POINT2D> m_trackPointList2;

	ROI m_trackROI;
	bool m_dropFlag;
	vector<CTrackLine> m_roiTrackList;
	/////////////////////////////
	float m_T1XStartCoord;
	float m_T1YStartCoord;
	float m_T1ZStartCoord;
	float m_T1Speed;
	float m_T2XStartCoord;
	float m_T2YStartCoord;
	float m_T2ZStartCoord;
	float m_T2Speed;
	//////////////////////////////////////
	CMyOpenGL m_openGL;
	vector<vector<POINT3D>> m_point3DList;
	/////////////////////
public:
	void DrawPicToHDC(IplImage *img, UINT ID);
	void ShowImage(Mat& src, UINT ID);
	void GetROIImage(cv::Mat frame, ROI roi, cv::Mat image);
	void JointImage(cv::Mat image1, cv::Mat image2, cv::Mat stackImage);
	void DrawPoint(CPoint imgPoint, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int nRadius);
	void DrawLine(vector<POINT2D> pointList, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int lineWidth);
	void DrawRect(int left, int right, int top, int bottom, int IMAGE_HEIGHT, int IMAGE_WIDTH, UINT nID, COLOR color, int lineWidth);

	/////////////////////////
	void Tracking(IplImage *frame);
	void InitTrackList(vector<CTrackLine> &trackList);
	void DealTrackList(vector<CTrackLine> &trackList,CTrackLine *refeTrajectory);
	void FilterMatchFeature(vector<FeatureMatch> &orbMatchList, int minOffset, int maxOffset);
	void AddTrackNode(vector<FeatureMatch> &orbMatchList, vector<CTrackLine> &trackList);
	void DrawTrackLine(IplImage *stackImage, vector<CTrackLine> trackList);
	void ConfirmTrajectoryRelativeHeight(CTDMap mTDMap, vector<CTrackLine> &trackList, CTrackLine *refeTrajectory);
	void VehicleCounting(vector<CTrackLine> &trackList, int *vehicleCount,int upLine);
	
	void SectionalProjectinAnalyseOX(vector<CTrackLine> trackList, vector<CPoint> &pointZOX, vector<CPoint> &pointYOX);
	void SectionalProjectionAnalyseZO(vector<POINT3D> point3DList, vector<CPoint> &pointZOX, vector<CPoint> &pointZOY);
	bool GetMinValue(vector<CPoint> pointList, bool bYFlag, int *minValue);
	bool GetMaxValue(vector<CPoint> pointList, bool bYFlag, int *maxValue);
	void GetPointSets(vector<CTrackLine> trackList, vector<POINT3D> &pointList, float xScale, float yScale, float zScale);
	/////////////////////////
	bool SavePointSet(const char* path, vector<POINT3D> pointList);
		/////////////////////////
// 构造
public:
	CORBTrackingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ORBTRACKING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnplay();
	afx_msg void OnBnClickedBtnnextframe();
	afx_msg void OnBnClickedBtnbefoframe();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_checkTrack1;
	BOOL m_checkTrack2;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL m_checkProbe;
	BOOL m_checkTrackSpeed;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	BOOL m_checkTrack;
	BOOL m_checkROI;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL m_checkMouseControl;
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_sliderGLCircleR;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_checkProbe2;
	afx_msg void OnBnClickedCheck8();
	BOOL m_checkRaw;
	afx_msg void OnBnClickedCheck9();
	int m_sliderTgHeight;
	int m_sliderTrHeight;
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_checkDisAA;
	afx_msg void OnBnClickedCheck10();
	BOOL m_checkIdealTrajectory;
	afx_msg void OnBnClickedCheck11();
	CComboBox m_comboTrack;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMCustomdrawSliderxstartpos(NMHDR *pNMHDR, LRESULT *pResult);
	int m_sliderXStartPos;
	int m_sliderYStartPos;
	int m_sliderZStartPos;
	int m_sliderSpeed;
	afx_msg void OnNMCustomdrawSliderystartpos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderzstartpos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderspeed(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_checkVariance;
	afx_msg void OnBnClickedCheck12();
};
