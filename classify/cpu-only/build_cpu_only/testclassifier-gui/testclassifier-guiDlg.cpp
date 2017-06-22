
// testclassifier-guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testclassifier-gui.h"
#include "testclassifier-guiDlg.h"
#include "afxdialogex.h"

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2411d.lib")
#pragma comment(lib, "opencv_highgui2411d.lib")
//#pragma comment(lib, "opencv_imgcodecs2411d.lib")
#pragma comment(lib, "opencv_imgproc2411d.lib")
//#else
//#pragma comment(lib, "opencv_core320.lib")
//#pragma comment(lib, "opencv_highgui320.lib")
//#pragma comment(lib, "opencv_imgcodecs320.lib")
//#pragma comment(lib, "opencv_imgproc320.lib")
//#pragma comment(lib, "opencv_videoio320.lib")
#endif

#include "libclassifier.h"
#include <string>
#include <chrono>
#include <sys\utime.h>

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
class Timer_CHRONO {
	using Clock = std::chrono::high_resolution_clock;
public:
	/*! \brief start or restart timer */
	inline void Tic() {
		start_ = Clock::now();
	}
	/*! \brief stop timer */
	inline void Toc() {
		end_ = Clock::now();
	}
	/*! \brief return time in ms */
	inline double Elasped() {
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
		return duration.count();
	}

private:
	Clock::time_point start_, end_;
};

class Timer_CV {
public:
	/*! \brief start or restart timer */
	inline void Tic() {
		start_ = getTickCount();
	}
	/*! \brief stop timer */
	inline void Toc() {
		end_ = getTickCount();
	}
	/*! \brief return time in ms */
	inline double Elasped() {
		return (end_ - start_) * 1000 / getTickFrequency();
	}

private:
	int64 start_, end_;
};


inline ULONGLONG GetMilliTickCount()
{
#if defined WIN32 || defined _WIN32
	LARGE_INTEGER	liPerfCount, liFreq;

	QueryPerformanceCounter(&liPerfCount);
	QueryPerformanceFrequency(&liFreq);

	return (liPerfCount.QuadPart * 1000LL / liFreq.QuadPart);
#else
	return 0;
#endif
}

class Timer_SYSTEM {
public:
	/*! \brief start or restart timer */
	inline void Tic() {
		start_ = GetMilliTickCount();
	}
	/*! \brief stop timer */
	inline void Toc() {
		end_ = GetMilliTickCount();
	}
	/*! \brief return time in ms */
	inline double Elasped() {
		return end_ - start_;
	}

private:
	int64 start_, end_;
};
//
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


// CtestclassifierguiDlg dialog



CtestclassifierguiDlg::CtestclassifierguiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestclassifierguiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestclassifierguiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestclassifierguiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CtestclassifierguiDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDOK2, &CtestclassifierguiDlg::OnBnClickedOk2)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestclassifierguiDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

string dir = "";// = "H:\\open-dnn-master\\bin\\x64\\Release\\";
#if 0
string model = "deploy.prototxt";
string trained = "snapshot_iter_51100.caffemodel";
string mean_fn = "mean.binaryproto";
string label = "labels.txt";
#else
string model = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\bvlc_googlenet.prototxt";
string trained = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\bvlc_googlenet.caffemodel";
string mean_fn = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\mean.binaryproto";
string label = "H:\\BackUp\\Caffe\\caffe-windows-master\\bin\\synset_words.txt";
#endif
string img_ = "H:\\Samples\\Remotes\\Real\\aircon-tornado-legend-real.png";
string model_file = dir + model;
string trained_file = dir + trained;
string mean_file = dir + mean_fn;
string label_file = dir + label;
string file = img_;

// CtestclassifierguiDlg message handlers

BOOL CtestclassifierguiDlg::OnInitDialog()
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
	Mat img = imread("H:\\Samples\\Remotes\\Real\\lg-nb4543-real.png");
	InitializeClassifier(model_file,trained_file,mean_file,label_file);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtestclassifierguiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestclassifierguiDlg::OnPaint()
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
HCURSOR CtestclassifierguiDlg::OnQueryDragIcon()
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
Mat equalizeIntensity(const Mat& inputImage)
{
	if (inputImage.channels() >= 3)
	{
		Mat ycrcb;

		cvtColor(inputImage, ycrcb, CV_BGR2YCrCb);

		vector<Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);

		Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_YCrCb2BGR);

		return result;
	}
	return Mat();
}
Mat refineMat(const char* fullpath)
{
	Mat origin = imread(fullpath);
	if (origin.empty())
		return Mat();

	normalize(origin, origin, 0, 255, NORM_MINMAX, CV_8UC3);
#ifdef USE_PAD
	int newsize = 0;
	if (origin.rows >= origin.cols)
		newsize = origin.rows;
	else
		newsize = origin.cols;

	int offset_x = (newsize - origin.cols) / 2;

	Mat img(newsize, newsize, CV_8UC3);

	memset(img.data, 0, newsize * newsize * 3);
	for (int j = 0; j < origin.rows; j++)
	{
		memcpy(img.data + j * newsize * 3 + offset_x * 3, origin.data + j * origin.cols * 3, 3 * origin.cols);
	}
	resize(img, img, Size(224, 224));
	return img;
#else
	//resize(origin, origin, Size(224, 224));
	//origin = equalizeIntensity(origin);
	return origin;
#endif

	
}

string double2string(double value)
{
	std::ostringstream strs;
	strs << value;
	std::string str = strs.str();
	return str;
}

char szFile[MAX_PATH];
char szFileName[MAX_PATH];
void CtestclassifierguiDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	TCHAR		szFilter[] = _T("All supported files (*.bmp;*.jpg;*.tif;*.png;...)|*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.tif;*.tiff;*.png|")
		_T("Bitmap files (*.bmp)|*.dib|")
		_T("JPEG files (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif|")
		_T("TIFF files (*.tif;*.tiff)|*.tif;*.tiff|")
		_T("PNG files (*.png)|*.png|")
		_T("All files (*.*)|*.*||");

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
			m_img = refineMat(szFile);		
			m_pThumbView->AddThumbnail(m_img);
			m_ImgList.push_back(m_img);
			m_fnList.push_back(strDisplayFileName);
			////////////////////////////////////////////////////////////////////////////////////
			std::string str = szFile;
			//m_pThumbView->AddString(str);
			///////////////////////////////////////
			//if (m_img.empty())
			//	return;

			//const int kTestN = 1000;
			//Timer_CV timer;
			//double time = 0;
			//timer.Tic();
			//vector<string> result = recognizeImage(m_img.data, m_img.cols, m_img.rows);
			//timer.Toc();
			//time += timer.Elasped();
			//string msg;
			//for (size_t i = 0; i < result.size(); ++i) {
			//	msg += "(ClassName:Prob) : " + result[i] + "\n";
			//}
			//msg += "ElapsedTime: " + double2string(time) + "ms";
			//MessageBoxA(NULL, msg.c_str(), m_curFileName, NULL);
			//waitKey(10000);
		}
	}

	fileName.ReleaseBuffer();
}


void CtestclassifierguiDlg::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here

}


int CtestclassifierguiDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pThumbView = new CThumbView(RGB(255, 255, 255));
	m_pThumbView->CreateEx(WS_EX_STATICEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(10, 10, 380, 250), this, 60000);
	return 0;
}



void CtestclassifierguiDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	SEL_THUMB_LIST selList;
	m_pThumbView->GetSelectedIndex(selList);
	if (selList.size() == 0)
	{
		AfxMessageBox(_T("Warining : please select image!"));
	}
	else
	{
		for (SEL_THUMB_LIST::iterator iter = selList.begin(); iter != selList.end(); iter++)
		{
			Mat img = m_ImgList[*iter];
			CString fn = m_fnList[*iter];
			char filename[MAX_PATH];
			UnicodeToAnsi(fn, sizeof(filename), filename);
			
			const int kTestN = 1000;
			Timer_CV timer;
			double time = 0;
			timer.Tic();
			vector<string> result = recognizeImage(img.data, img.cols, img.rows);
			timer.Toc();
			time += timer.Elasped();
			string msg = result[0];
			//for (size_t i = 0; i < result.size(); ++i) {
			//	msg += "(ClassName:Prob) : " + result[i] + "\n";
			//}
			//msg += "ElapsedTime: " + double2string(time) + "ms";
			//MessageBoxA(NULL, msg.c_str(), filename, NULL);ㄷ
			
			putText(img, msg.c_str(), Point(0, 100), cv::HersheyFonts::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255));
			imshow(filename, img);
			waitKey(10000);
		}
	}
}
