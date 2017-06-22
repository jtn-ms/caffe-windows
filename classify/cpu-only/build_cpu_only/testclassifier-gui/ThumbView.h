#pragma once

#include "MemoryDC.h"
#include <list>
#include <opencv2/opencv.hpp>

using namespace std;


typedef struct tTHUMB_INFO
{
	HBITMAP		hThumbBmp;
	tTHUMB_INFO()
	{
		hThumbBmp = NULL;
	}
} THUMB_INFO, *PTHUMB_INFO;
//typedef CArray<THUMB_INFO, THUMB_INFO&> THUMB_LIST, *PTHUMB_LIST;
typedef vector<THUMB_INFO> THUMB_LIST, *PTHUMB_LIST;
typedef list<INT> SEL_THUMB_LIST, *PSEL_THUMB_LIST;

#define TYPE_THUMB_VIEW_UPLIST		0
#define TYPE_THUMB_VIEW_QUEUE		1
#define TYPE_THUMB_VIEW_READY		2
#define TYPE_THUMB_VIEW_FAIL		3
#define TYPE_THUMB_VIEW_ARCHIVE		4

#ifdef VA_CLIENT
#define UPLOAD_STATUS_PREPARE	0
#define UPLOAD_STATUS_PROGRESS	1
#define UPLOAD_STATUS_COMPLETE	2
#define UPLOAD_STATUS_FAIL		3

typedef struct
{
	CString szFileName;
	int nStatus;
	int nProgress;
	VIDEO Video;
} UPLOAD_VIDEO;

typedef CList<UPLOAD_VIDEO, UPLOAD_VIDEO&> UPLOAD_VIDEO_LIST, *PUPLOAD_VIDEO_LIST;
#endif

#define THUMB_WIDTH			64
#define THUMB_HEIGHT		64
#define THUMB_GAP			4
#define THUMB_PROG_WIDTH	80
#define THUMB_PROG_HEIGHT1	2
#define THUMB_PROG_HEIGHT2	(THUMB_PROG_HEIGHT1+2)
#define THUMB_PROG_HEIGHT3	(THUMB_PROG_HEIGHT2+2)
#define THUMB_PROG_HEIGHT4	(THUMB_PROG_HEIGHT3+2)
#define THUMB_PROG_HEIGHT5	(THUMB_PROG_HEIGHT4+2)
#define THUMB_PROG_HEIGHT6	(THUMB_PROG_HEIGHT5+2)
#define THUMB_PROG_HEIGHT	(THUMB_PROG_HEIGHT6+2)
// CThumbView view

class CThumbView : public CScrollView
{
	DECLARE_DYNCREATE(CThumbView)

public:
	CThumbView(COLORREF clrBack = RGB(255, 255, 255), COLORREF clrThumbBack = RGB(255, 255, 255), int nThumbCntPerRow = 0);           // protected constructor used by dynamic creation
	virtual ~CThumbView();

	int AddThumbnail(cv::Mat &image);
	void AddString(vector<char> string);
	void DelThumbnail(int nIndex);
	void DelAllThumbnail();

	int GetThumbnailCount()	{ return (int)m_ThumbList.size(); }
	void GetSelectedIndex(SEL_THUMB_LIST &selList);
	PTHUMB_INFO GetAt(int nIndex);
	PTHUMB_LIST GetSelectedList();
	
protected:
	HBITMAP Mat2Bitmap(cv::Mat &image);
	void AdjustScrollSize(BOOL bRepaint = TRUE);
	void AdjustScrollPos(BOOL bRepaint = TRUE);
	void CalcSel(CPoint point);
	void ClearList();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	BOOL m_bBorder;
	CBrush m_brushBack;
	CBrush m_brushThumbBack;
	THUMB_LIST m_ThumbList;
	BOOL m_bAutoCntPerRow;
	int m_nThumbCntPerRow;
	int m_nSel;
	int m_nThumbListCnt;
	SEL_THUMB_LIST m_nSelList;
	CMemoryDC m_ThumbMemDC;
	vector<char> m_string;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	
};


