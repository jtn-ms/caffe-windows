// LivePlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcdemo.h"
#include "mfcdemoDlg.h"
#include "LivePlayDlg.h"
#include "afxdialogex.h"

#ifdef USE_OPENCV
#include <opencv/cv.h>  
#include <opencv/cvaux.h>  
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2411d.lib")
#pragma comment(lib, "opencv_highgui2411d.lib")
//#pragma comment(lib, "opencv_imgcodecs2411d.lib")
#pragma comment(lib, "opencv_imgproc2411d.lib")
#else
//#pragma comment(lib, "opencv_core2410.lib")
//#pragma comment(lib, "opencv_highgui2410.lib")
//#pragma comment(lib, "opencv_imgcodecs2410.lib")
//#pragma comment(lib, "opencv_imgproc2410.lib")
//#pragma comment(lib, "opencv_video2410.lib")
#endif
#include "libdetector.h"
#endif
#include <string>
#include <chrono>
#include <sys\utime.h>

using namespace cv;
using namespace std;
//

// CLivePlayDlg dialog

IMPLEMENT_DYNAMIC(CLivePlayDlg, CDialogEx)

CLivePlayDlg::CLivePlayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLivePlayDlg::IDD, pParent)
	, m_pCurFrame(NULL)
	, m_pDrawData(NULL)
	, m_bIsRecording(FALSE)
{
	memset(&m_BmpInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	m_BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BmpInfo.bmiHeader.biPlanes = 1;
	m_BmpInfo.bmiHeader.biBitCount = 24;
	//
	m_pFont = new CFont();

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH;
	lf.lfCharSet = DEFAULT_CHARSET;
	lstrcpyn(lf.lfFaceName, _T("SimSun"), LF_FACESIZE);

	m_pFont->CreateFontIndirect(&lf);
}

CLivePlayDlg::~CLivePlayDlg()
{

	delete m_pFont;
}

void CLivePlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLivePlayDlg, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CLivePlayDlg message handlers
void CLivePlayDlg::Initialize(std::string fullpath)
{
	VideoCapture cap(fullpath);
	if (!cap.isOpened())
		return;

	Mat in;
	bool success = cap.read(in);
	if (!success)
		return;

	m_nWidth = in.cols;
	m_nHeight = in.rows;
	m_BmpInfo.bmiHeader.biWidth = m_nWidth;
	m_BmpInfo.bmiHeader.biHeight = -m_nHeight;
	m_memDC.Create(m_nWidth, m_nHeight);

	m_FilePath = fullpath;
	m_bClose = false;
	//

}

void CLivePlayDlg::Initialize(int width, int height)
{
	m_nWidth = width;
	m_nHeight = height;
	m_BmpInfo.bmiHeader.biWidth = m_nWidth;
	m_BmpInfo.bmiHeader.biHeight = -m_nHeight;
	m_memDC.Create(m_nWidth, m_nHeight);
}

DWORD  dwThreadID[1];
DWORD WINAPI callback_timer(LPVOID lpParam)
{

	return 0;
}

VideoCapture cap = NULL;
int fps = 2;
int frame_count = 0;

void CLivePlayDlg::Play()
{
	SetTimer(1, 10, NULL);
	cap = VideoCapture(m_FilePath.c_str());
	if (!cap.isOpened())
		return;
	m_bClose = false;
	//CreateThread(NULL, 0, callback_timer, &cap, 0, &dwThreadID[0]);
	Invalidate();
}


CString CAMERA_RECORD_GetSaveName(const TCHAR *defName)
{
	CString szName;
	CString szFilter = VIDEO_FILTER;

	CFileDialog	dlgFile(FALSE, NULL, defName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlgFile.m_ofn.Flags |= OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (IDOK == dlgFile.DoModal())
	{
		szName = dlgFile.GetPathName();

		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath_s(szName.GetBuffer(), drive, dir, fname, ext);
		if (lstrlen(ext)<=1) {
			int filterIndex = dlgFile.m_ofn.nFilterIndex;
			TCHAR *newExt = NULL;
			if (filterIndex==3)
				newExt = _T("mpeg");
			else
				newExt = _T("avi");

			if (lstrlen(ext)==0)
				szName += _T(".");
			szName += newExt;
		}

		//lstrcpy(theApp.m_szSaveDir, dlgFile.GetPathName().Left(dlgFile.GetPathName().ReverseFind('\\')));
		//theApp.SaveSettings();
	}
	return szName;
}

void CLivePlayDlg::Disconnect()
{
	if(m_pDrawData)
		free(m_pDrawData);
	//
	Invalidate();
}

void CLivePlayDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CLivePlayDlg::SetFrame(LPBYTE pFrame)
{
	if (pFrame == NULL)
	{
		m_pCurFrame = NULL;
		Invalidate();
	}
	else
	{
		m_pCurFrame = pFrame;
		Invalidate();
	}
}

typedef float REAL;

void CLivePlayDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CRect	rect;
	GetClientRect(rect);
	if(!rect.Width() ||
	   !rect.Height())
	   return;

	dc.SetStretchBltMode(HALFTONE);
	CDC *pMemDC = m_memDC.GetDC();
	CFont	font, *pOldFont = GetParent()->GetFont();
	LOGFONT	logfont;
	REAL	font_size = (REAL)(min(m_nWidth, m_nHeight) / 24.0);

	pOldFont->GetLogFont(&logfont);
	logfont.lfHeight *= 2;
	logfont.lfWeight = FW_BOLD;

	if (!pMemDC->GetSafeHdc())
	{
		font.CreateFontIndirectW(&logfont);
		pOldFont = dc.SelectObject(&font);
		
		dc.SetTextColor(RGB(64,64,64));
		dc.SetBkMode(TRANSPARENT);
		CBrush	brush;
		brush.CreateSolidBrush(RGB(200,200,200));
		dc.FillRect(&rect,&brush);
		dc.DrawText(_T("No Signal"), &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		font.DeleteObject();
		dc.SelectObject(pOldFont);

		return;
	}
#undef USE_OLD 
#ifdef USE_OLD
	if(IsConnected())
	{
		SetDIBitsToDevice(pMemDC->GetSafeHdc(),
			0, 0, m_memDC.GetWidth(), m_memDC.GetHeight(),
			0, 0, 0, m_nHeight,
			m_pCurFrame, &m_BmpInfo, DIB_RGB_COLORS);
	}
	CRect rect;
	GetClientRect(rect);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		pMemDC, 0, 0, m_nWidth, m_nHeight, SRCCOPY);
#else
	if(m_pCurFrame)
	{
		Mat tmp(m_nHeight,m_nWidth,CV_8UC3);
		tmp.data = m_pCurFrame;
		cv::resize(tmp,tmp,cv::Size(rect.Width(),rect.Height()));
		//imshow("aaa", tmp);waitKey(1);
		//
		int width,height = rect.Height();
		width =  (rect.Width() * 3 + 3) / 4 * 4; 
		//
		if(width != m_oldRect.Width() ||
		   height != m_oldRect.Height())
		{
			if(m_pDrawData)
			{
				free(m_pDrawData);
			}
			m_pDrawData = (char*)malloc(width * height);
			m_oldRect.SetRect(0,0,width,height);
		}

		int step1 = 0,step2 = 0;
		for(int j = 0; j < rect.Height(); j++)
		{
			memcpy(m_pDrawData + step1,tmp.data + step2,tmp.cols*3);
			step1 += width;
			step2 += tmp.cols*3;
		}

		m_BmpInfo.bmiHeader.biWidth = rect.Width();
		m_BmpInfo.bmiHeader.biHeight = -height;
		SetDIBitsToDevice(dc.GetSafeHdc(),
			0, 0, rect.Width(), rect.Height(),
			0, 0, 0, height,
			m_pDrawData, &m_BmpInfo, DIB_RGB_COLORS);
		
	}
#endif
}



LRESULT CLivePlayDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_FRAME_SEND_RECEIVE:
		{
			SetFrame((LPBYTE)wParam);
			//m_pCurFrame = (LPBYTE)wParam;
			Invalidate();
			UpdateWindow();
		}
		break;
	case  WM_STOP_PLAYING:
		m_bClose = true;
	default:
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CLivePlayDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	Disconnect();
	return CDialogEx::DestroyWindow();
}

void CLivePlayDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnRButtonDown(nFlags, point);
}

BOOL CLivePlayDlg::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(pResult);

	// need to handle both ANSI and UNICODE versions of the message
	LPTOOLTIPTEXT	pTTT = (LPTOOLTIPTEXT)pNMHDR;
	CString			strTipText;
	UINT_PTR		nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_GETDISPINFO)
	{
		if(true)
			strTipText.Format(L"%s","ToolTipNotify");

		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 0);
		lstrcpy(pTTT->szText, strTipText);
	}

	return TRUE;    // message was handled
}


const string& model_file = "deploy.prototxt";
const string& weights_file = "VGG_VOC0712_SSD_300x300_iter_120000.caffemodel";
const string& label_file = "labels-voc0712.txt";

BOOL CLivePlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	EnableToolTips(TRUE);
	
	// EXCEPTION: OCX Property Pages should return FALSE
	string strWorkDir = "E:\\Investigation\\DetectNet\\dev\\caffe-ssd-win-master\\Build\\x64\\Release";//
	char tmp[MAX_PATH] = {0};
	::GetCurrentDirectoryA(_MAX_PATH, tmp);
	strWorkDir = tmp;
	InitializeDetector(strWorkDir + "\\" + model_file, 
					   strWorkDir + "\\" + weights_file,
					   strWorkDir + "\\" + label_file);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


INT_PTR CLivePlayDlg::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: Add your specialized code here and/or call the base class
	int nSel = 0;
	CRect rect;
	GetClientRect(&rect);
	SetRect(&pTI->rect,rect.left, rect.top, rect.right, rect.bottom);
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)nSel;
	pTI->uFlags = TTF_ALWAYSTIP;
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return (INT_PTR)pTI->uId;
}


void CLivePlayDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLivePlayDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CLivePlayDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);//
}

void CLivePlayDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	Invalidate();
}


void CLivePlayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
		{
			  Mat in;
			  CvFont * font = new CvFont;
			  cvInitFont(font, CV_FONT_VECTOR0, 0.5f, 1.0f, 0, 1, 8); //rate of width and height is 1:2

			  if (!m_bClose)
			  {
				  bool success = cap.read(in);
				  if (!success)
				  {
					  m_bClose = true;
					  KillTimer(1);
					  break;
				  }
				  if (!in.empty())// && (frame_count % fps == 0))
				  {
					  
					  std::vector<vector<float> > detections = detectObject(in.data, in.cols, in.rows);

					  /* Print the detection results. */
					  for (int i = 0; i < detections.size(); ++i) {
						  const vector<float>& d = detections[i];
						  // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
						  const float score = d[2];
						  const float threshold = 0.4;
						  if (score >= threshold && (d[1] == 2 || d[1] == 6 || d[1] == 7 || d[1] == 14 || d[1] == 15 || d[1] == 19)) {
							  cv::rectangle(in,
								  cv::Point((d[3] * in.cols), (d[4] * in.rows)),
								  cv::Point((d[5] * in.cols), (d[6] * in.rows)),
								  cv::Scalar(255, 0, 0), 3, 8, 0);

							  float dist = d[6] >= 1.0 ? 0.0 : 1 - d[6];
							  int meterperpixel = 12 + (dist / 0.1) * 6;
							  std::stringstream bb_label;
							  bb_label.precision(2);
							  bb_label << getCaption(d[1]);
							  bb_label << (dist * meterperpixel);
							  bb_label << "m";
							  // bb_label << "," << score;
							  cv::putText(in, bb_label.str(),
								  cv::Point((d[3] * in.cols), (d[4] * in.rows)),
								  cv::FONT_HERSHEY_PLAIN, (in.rows / 300),
								  cv::Scalar(0, 0, 255), 3, 8, false);
						  }
					  }

					  m_pCurFrame = (LPBYTE)in.data;
					  Invalidate();
					  UpdateWindow();
				  }
				  frame_count++;
			  }
			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
