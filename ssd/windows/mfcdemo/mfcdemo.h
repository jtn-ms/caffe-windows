
// mfcdemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define VIDEO_FILTER	_T("All supported files (*.avi;*.mpeg;*.MP4;*.mp4,*.MKV)|;*.avi;*.mpeg;*.MP4;*.mp4,*.MKV|") \
	_T("AVI files (*.avi)|*.avi|") \
	_T("MPEG files (*.mpeg;*.mp4;*.MP4)|*.mpeg|*.mp4|*.MP4|") \
	_T("MKV files (*.MKV)|*.MKV|") \
	_T("|")

#define IMAGE_FILTER	_T("All supported files (*.bmp;*.jpg;*.tif;*.png;...)|*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.tif;*.tiff;*.png|") \
	_T("Bitmap files (*.bmp)|*.dib|") \
	_T("JPEG files (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif|") \
	_T("TIFF files (*.tif;*.tiff)|*.tif;*.tiff|") \
	_T("PNG files (*.png)|*.png|") \
	_T("All files (*.*)|*.*||")

#define WM_FRAME_SEND_RECEIVE			(WM_USER + 30)
#define WM_STOP_PLAYING					(WM_USER + 31)
///////////////////////////////////////////////////////////////////////////
#include <chrono>
#include <string>
#include <sys\utime.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


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
///////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CmfcdemoApp:
// See mfcdemo.cpp for the implementation of this class
//

class CmfcdemoApp : public CWinApp
{
public:
	CmfcdemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CmfcdemoApp theApp;