
// videodemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "videodemo.h"
#include "videodemoDlg.h"
#include "afxdialogex.h"

#include <opencv/cvaux.h>  
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CvideodemoDlg dialog



CvideodemoDlg::CvideodemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CvideodemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvideodemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvideodemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD, &CvideodemoDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_PLAY, &CvideodemoDlg::OnBnClickedPlay)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CvideodemoDlg message handlers

BOOL CvideodemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	createWnd();
	moveWnd();
	showWnd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CvideodemoDlg::moveWnd()
{
	CRect   rcClient, rcControl;
	GetClientRect(rcClient);
	rcControl = rcClient;
#define X_MARGIN                      4
#define Y_MARGIN                      4
#define BUTTON_WIDTH          80//16
#define BUTTON_HEIGHT         16//16
#define BUTTON_GAP            10//20
#define X_CAMERA_WND_GAP      2
#define Y_CAMERA_WND_GAP      2
#define CAPTION_HEIGHT 10
	rcControl.left = rcClient.left + X_MARGIN;
	rcControl.top = rcClient.top + 2 + CAPTION_HEIGHT;
	rcControl.right = rcClient.right - X_MARGIN - BUTTON_WIDTH;
	rcControl.bottom = rcClient.bottom - (Y_MARGIN + 2 * 2);
	m_pLivePlay->MoveWindow(rcControl);
	m_pLivePlay->Invalidate();
	m_pLivePlay->UpdateWindow();
}

void CvideodemoDlg::createWnd()
{
	m_pLivePlay = new CLivePlayDlg(this);
	m_pLivePlay->Create(IDD_LIVE_PLAY_DLG, this);
}

void CvideodemoDlg::showWnd()
{
	m_pLivePlay->ShowWindow(SW_SHOW);
}

void CvideodemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CvideodemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CvideodemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
///////////////////////////////////////////////////////////////////////////
void UnicodeToAnsi(const wchar_t *szUnicode, int len, char *szAnsi)
{
	WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, szAnsi, len, NULL, NULL);
}

void AnsiToUnicode(const char *szAnsi, wchar_t *szUnicode, int len)
{
	MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, szUnicode, len);
}
/////////////////////////////////////////////////////////////////////////////
char szFile[MAX_PATH];
char szFileName[MAX_PATH];
void CvideodemoDlg::OnBnClickedLoad()
{
	CString szFilter = VIDEO_FILTER;
	CFileDialog	fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER, szFilter);

	CString fileName;
	const int c_cMaxFiles = 200;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;

	//fileDlg.GetOFN().lpstrInitialDir = theApp.m_szOpenDir;
	fileDlg.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
	fileDlg.GetOFN().nMaxFile = c_cbBuffSize;

	if (fileDlg.DoModal() == IDOK)
	{
		POSITION	pos;
		int nSeek = 0;

		pos = fileDlg.GetStartPosition();
		while (pos != NULL)
		{
			CString	strFilePath;
			strFilePath = fileDlg.GetNextPathName(pos);

			CString		strDisplayFileName = _T("");
			strDisplayFileName += "\"";
			strDisplayFileName += strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind('\\') - 1);
			strDisplayFileName += "\" ";

			UnicodeToAnsi(strFilePath, sizeof(szFile), szFile);
			UnicodeToAnsi(strDisplayFileName, sizeof(szFileName), szFileName);
			////////////////////////////////////////////////////////////////////////////////////
			//DisplayImage(this->GetDC()->GetSafeHdc(), szFile);
			m_curFullPath = szFile;
			m_curFileName = szFileName;
			////////////////////////////////////////////////////////////////////////////////////
			m_pLivePlay->Initialize(std::string(szFile));
			///////////////////////////////////////
		}
	}

	fileName.ReleaseBuffer();
}


void CvideodemoDlg::OnBnClickedPlay()
{
	// TODO: Add your control notification handler code here
#if 1
	m_pLivePlay->Play();
#else
	VideoCapture cap(m_curFullPath);
	if (!cap.isOpened())
		return;

	Mat in;
	cap.read(m_Mat);
	while (true)
	{
		bool success = cap.read(in);
		if (!success)
			break;
		if (!in.empty())
		{
			m_pLivePlay->SendMessage(WM_FRAME_SEND_RECEIVE, (WPARAM)in.data);
			//m_pLivePlay->Invalidate();
			//m_pLivePlay->SetFrame(in.data);
			//Sleep(50);
		}
	}
	m_pLivePlay->SetFrame();
#endif
}


void CvideodemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pLivePlay)
		moveWnd();

}


BOOL CvideodemoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CvideodemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (m_pLivePlay)
		m_pLivePlay->SendMessage(WM_STOP_PLAYING);
	CDialogEx::OnClose();
}
