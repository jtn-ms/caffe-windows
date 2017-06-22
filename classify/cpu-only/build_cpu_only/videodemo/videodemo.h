
// videodemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

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
#include "resource.h"		// main symbols


// CvideodemoApp:
// See videodemo.cpp for the implementation of this class
//

class CvideodemoApp : public CWinApp
{
public:
	CvideodemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CvideodemoApp theApp;