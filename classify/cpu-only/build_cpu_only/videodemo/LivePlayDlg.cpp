// LivePlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "videodemo.h"
#include "videodemoDlg.h"
#include "LivePlayDlg.h"
#include "afxdialogex.h"

#ifdef USE_OPENCV
#include <opencv/cv.h>  
#include <opencv/cvaux.h>  
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2411d.lib")
#pragma comment(lib, "opencv_highgui2411d.lib")
//#pragma comment(lib, "opencv_imgcodecs2411d.lib")
#pragma comment(lib, "opencv_imgproc2411d.lib")
#else
#pragma comment(lib, "opencv_core320.lib")
#pragma comment(lib, "opencv_highgui320.lib")
#pragma comment(lib, "opencv_imgcodecs320.lib")
#pragma comment(lib, "opencv_imgproc320.lib")
#pragma comment(lib, "opencv_videoio320.lib")
#endif
#include "libclassifier.h"
#endif
#include <string>
#include <chrono>
#include <sys\utime.h>

using namespace cv;
using namespace std;

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
END_MESSAGE_MAP()



string model = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\bvlc_googlenet.prototxt";
string trained = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\bvlc_googlenet.caffemodel";
string mean_fn = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\mean.binaryproto";
string label = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\synset_words.txt";

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

void CLivePlayDlg::Play()
{
	VideoCapture cap(m_FilePath.c_str());
	if (!cap.isOpened())
		return;

	Mat in;
	CvFont * font = new CvFont;
	cvInitFont(font, CV_FONT_VECTOR0, 0.5f, 1.0f, 0, 1, 8); //rate of width and height is 1:2
	int fps = 5;
	int frame_count = 0;
	while (!m_bClose)
	{
		bool success = cap.read(in);
		if (!success)
			break;
		if (!in.empty() && (frame_count % fps == 0))
		{
			m_pCurFrame = (LPBYTE)in.data;
			vector<string> result = recognizeImage(in.data, in.cols, in.rows);
			int x = 20, y = 40;
			for (size_t i = 0; i < result.size(); ++i) {
				string msg;
				msg += "(ClassName:Prob) : " + result[i];
				putText(in, msg, Point(x, y), CV_FONT_NORMAL, 1, Scalar(255, 0, 255), 1, 1);
				y += 40;
			}
			
			Invalidate();
			UpdateWindow();
		}
		frame_count++;
	}
	Invalidate();
	SetFrame();
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



BOOL CLivePlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	EnableToolTips(TRUE);
	InitializeClassifier(model, trained, mean_fn, label);
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
