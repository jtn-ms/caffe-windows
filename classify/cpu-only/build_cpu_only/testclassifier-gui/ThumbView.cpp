// ThumbView.cpp : implementation file
//

#include "stdafx.h"
#include "testclassifier-gui.h"
#include "ThumbView.h"

using namespace cv;
// CThumbView

IMPLEMENT_DYNCREATE(CThumbView, CScrollView)

CThumbView::CThumbView(COLORREF clrBack, COLORREF clrThumbBack, int nThumbCntPerRow)
: m_bBorder(FALSE)
, m_nThumbCntPerRow(nThumbCntPerRow)
, m_nSel(-1)
, m_nThumbListCnt(0)
{
	m_brushBack.CreateSolidBrush(clrBack);
	m_brushThumbBack.CreateSolidBrush(clrThumbBack);


	if (nThumbCntPerRow)
	{
		m_bAutoCntPerRow = FALSE;
	}
	else
	{
		m_bAutoCntPerRow = TRUE;
	}
}

CThumbView::~CThumbView()
{
	m_brushBack.DeleteObject();
	m_brushThumbBack.DeleteObject();

	ClearList();
}


BEGIN_MESSAGE_MAP(CThumbView, CScrollView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


// CThumbView drawing

int CThumbView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	LONG	lExStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);

	if (lExStyle & WS_EX_STATICEDGE)
	{
		m_bBorder = TRUE;
	}

	return 0;
}

void CThumbView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal, sizePage, sizeLine;
	// TODO: calculate the total size of this view
	sizeTotal.cx = (THUMB_WIDTH + THUMB_GAP) * m_nThumbCntPerRow + THUMB_GAP - 2;
	sizeTotal.cy = THUMB_HEIGHT + THUMB_GAP * 2;
	sizePage.cx = THUMB_WIDTH + THUMB_GAP;
	sizePage.cy = THUMB_HEIGHT + THUMB_GAP;
	sizeLine.cx = THUMB_WIDTH + THUMB_GAP;
	sizeLine.cy = THUMB_HEIGHT + THUMB_GAP;
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

	m_ThumbMemDC.Create(sizeTotal.cx, sizeTotal.cy);

	EnableToolTips();
}

void CThumbView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	HDC			hMemDC;
	BITMAP		bitmap;
	HBITMAP		hBitmap, hOldBitmap;
	int			x, y, dx, dy;
	BOOL		bSelected;
	BOOL		bEnabled;

	bEnabled = IsWindowEnabled();

	m_ThumbMemDC.GetDC()->FillRect(CRect(0,0, m_ThumbMemDC.GetWidth(), m_ThumbMemDC.GetHeight()), &m_brushBack);
	
	hMemDC = CreateCompatibleDC(m_ThumbMemDC.GetDC()->GetSafeHdc());
	
	for (int i = 0; i < (int)m_ThumbList.size(); i++)
	{
		x = (i % m_nThumbCntPerRow) * (THUMB_WIDTH + THUMB_GAP);
		y = (i / m_nThumbCntPerRow) * (THUMB_HEIGHT + THUMB_GAP);

		hBitmap = m_ThumbList[i].hThumbBmp;
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		CBitmap::FromHandle(hBitmap)->GetBitmap(&bitmap);
		dx = (THUMB_WIDTH - bitmap.bmWidth) / 2 + THUMB_GAP;
		dy = (THUMB_HEIGHT - bitmap.bmHeight) / 2 + THUMB_GAP;
		BitBlt(m_ThumbMemDC.GetDC()->GetSafeHdc(), x+dx, y+dy, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);

		//display Selected item 
		bSelected = FALSE;
		for each (int nSel in m_nSelList)
		{
			if (i == nSel)
			{
				bSelected = TRUE;
				break;
			}
		}

		CRect	rc(x+THUMB_GAP-1, y+THUMB_GAP-1, x+THUMB_WIDTH+THUMB_GAP+1, y+THUMB_HEIGHT+THUMB_GAP+1);
			
		if(!m_string.empty())
		{
			CString cstring;
			cstring.AppendChar(m_string[i]);

			m_ThumbMemDC.GetDC()->SetTextColor(RGB(255, 0, 0));
			m_ThumbMemDC.GetDC()->DrawText(cstring, rc, DT_LEFT);
		}
		if (bSelected)
		{
			CBrush	*pBrush, *pOldBrush;
			CPen	pen(PS_SOLID, 2, RGB(0, 0, 255)), *pOldPen;
			
			pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			pOldBrush = m_ThumbMemDC.GetDC()->SelectObject(pBrush);
			pOldPen = m_ThumbMemDC.GetDC()->SelectObject(&pen);
			m_ThumbMemDC.GetDC()->Rectangle(rc);
			m_ThumbMemDC.GetDC()->SelectObject(pOldBrush);
			m_ThumbMemDC.GetDC()->SelectObject(pOldPen);
			pBrush->DeleteObject();
			pen.DeleteObject();
		}
		else
		{
			CBrush	*pBrush, *pOldBrush;
			CPen	pen(PS_SOLID, 1, RGB(0, 0, 0)), *pOldPen;
			
			pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			pOldBrush = m_ThumbMemDC.GetDC()->SelectObject(pBrush);
			pOldPen = m_ThumbMemDC.GetDC()->SelectObject(&pen);
			m_ThumbMemDC.GetDC()->Rectangle(rc);
			m_ThumbMemDC.GetDC()->SelectObject(pOldBrush);
			m_ThumbMemDC.GetDC()->SelectObject(pOldPen);
			pBrush->DeleteObject();
			pen.DeleteObject();
		}
	}

	DeleteDC(hMemDC);

	pDC->BitBlt(0, 0, m_ThumbMemDC.GetWidth(), m_ThumbMemDC.GetHeight(), m_ThumbMemDC.GetDC(), 0, 0, SRCCOPY);

	if (!bEnabled)
	{
		Gdiplus::Graphics	graphics(pDC->GetSafeHdc());
		Gdiplus::SolidBrush	brush(Gdiplus::Color(70, 222, 225, 231));
		CRect		rect;

		GetClientRect(rect);
		graphics.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
	}
}

BOOL CThumbView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush	*pOldBrush = pDC->SelectObject(&m_brushBack); 
	CRect	rect;

	pDC->GetClipBox(&rect); 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}


// CThumbView diagnostics

#ifdef _DEBUG
void CThumbView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CThumbView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CThumbView message handlers

void CThumbView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_bAutoCntPerRow)
	{
		//if (m_bBorder)
		//{
		//	cx -= GetSystemMetrics(SM_CXBORDER) * 2;
		//}

		//cx -= (GetSystemMetrics(SM_CXVSCROLL) + 2);
		m_nThumbCntPerRow = (cx - THUMB_GAP) / (THUMB_WIDTH + THUMB_GAP);
		cx = (THUMB_WIDTH + THUMB_GAP) * m_nThumbCntPerRow + THUMB_GAP - 2;
		if (m_nThumbCntPerRow)
		{
			cy = (THUMB_HEIGHT + THUMB_GAP) * (((int)m_ThumbList.size() + m_nThumbCntPerRow - 1) / m_nThumbCntPerRow) + THUMB_GAP;
			SetScrollSizes(MM_TEXT,
				CSize(cx, cy),
				CSize(THUMB_WIDTH+THUMB_GAP, THUMB_HEIGHT+THUMB_GAP),
				CSize(THUMB_WIDTH+THUMB_GAP, THUMB_HEIGHT+THUMB_GAP));

			m_ThumbMemDC.Create(cx, cy);
		}
	}
}


void CThumbView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int	nPrevSel = m_nSel;
	
	point += GetScrollPosition();
	CalcSel(point);

	if (nFlags & MK_CONTROL)
	{
		if (m_nSel != -1)
		{
			BOOL bFind = FALSE;

			for each (int nSel in m_nSelList)
			{
				if (nSel == m_nSel)
				{
					bFind = TRUE;
					break;
				}
			}
			if (bFind)
			{
				m_nSelList.remove(m_nSel);
			}
			else
			{
				m_nSelList.push_back(m_nSel);
			}

			//m_nSelList.sort(greater<int>());
			Invalidate(FALSE);
		}
	}
	else if (nFlags & MK_SHIFT)
	{
		if ((m_nSel != -1) && (nPrevSel != -1))
		{
			int firstIter, secondIter;

			m_nSelList.clear();
			firstIter = nPrevSel;
			secondIter = m_nSel;
			if (m_nSel < nPrevSel)
			{
				firstIter = m_nSel;
				secondIter = nPrevSel;	
			}
			for (int i = firstIter; i <=secondIter; i++)
			{
				m_nSelList.push_back(i);
			}
			///m_nSelList.sort(greater<int>());
			Invalidate(FALSE);
		}
		else if (nPrevSel < 0)
		{
			m_nSelList.clear();
			m_nSelList.push_back(m_nSel);
			Invalidate(FALSE);
		}
	}
	else 
	{
		if (m_nSel != -1)
		{
			m_nSelList.clear();
			m_nSelList.push_back(m_nSel);
			Invalidate(FALSE);
		}
		else
		{
			m_nSelList.clear();
			Invalidate(FALSE);
		}
	}

	SetFocus();
	CScrollView::OnLButtonDown(nFlags, point);
}


int CThumbView::AddThumbnail(cv::Mat &image)
{
	BOOL bServerGetThumb = FALSE;
	BOOL bManualSetThumb = FALSE;
	int nWidth = 0, nHeight = 0, nFrameCnt = 0;
	double dFps = 0;
	time_t ulStartTime = 0;

	THUMB_INFO	ThumbInfo;

	ThumbInfo.hThumbBmp = Mat2Bitmap(image);

	int	nIndex = -1;

	m_ThumbList.push_back(ThumbInfo);
	nIndex = m_ThumbList.size() - 1;

	AdjustScrollSize(FALSE);

	return nIndex;
}

void CThumbView::DelThumbnail(int nIndex)
{
	if (nIndex < 0)
	{
		for each (int nSel in m_nSelList)
		{
			DeleteObject(m_ThumbList[nSel].hThumbBmp);
			m_ThumbList.erase(m_ThumbList.begin()+nSel);
		}
		m_nSelList.clear();
		m_nSel = -1;
	}

	if (nIndex >= (int)m_ThumbList.size())
	{
		return;
	}

	if (nIndex >= 0)
	{
		DeleteObject(m_ThumbList[nIndex].hThumbBmp);
		m_ThumbList.erase(m_ThumbList.begin()+nIndex);
		m_nSel = -1;

		for each (int nSel in m_nSelList)
		{
			if (nIndex ==nSel)
			{
				m_nSelList.remove(nSel);
				break;
			}
		}

		m_nThumbListCnt--;
	}

	AdjustScrollSize();
}

void CThumbView::DelAllThumbnail()
{
	for (int i = (int)m_ThumbList.size()-1; i >= 0; i--)
	{
		DelThumbnail(i);
	}

	AdjustScrollSize();
}


void CThumbView::GetSelectedIndex(SEL_THUMB_LIST &selList)
{
	selList.clear();
	selList = m_nSelList;
	//selList.sort(greater<int>( ));
}

PTHUMB_INFO CThumbView::GetAt(int nIndex)
{
	return &m_ThumbList[nIndex];
}

PTHUMB_LIST CThumbView::GetSelectedList()
{
	return &m_ThumbList;
}


HBITMAP CThumbView::Mat2Bitmap(Mat &image)
{
	HBITMAP	hThumbBmp;
	SIZE	size;

	if (image.rows*THUMB_WIDTH/image.cols <= THUMB_HEIGHT)
	{
		size.cx = THUMB_WIDTH;
		size.cy = image.rows * THUMB_WIDTH / image.cols;
	}
	else
	{
		size.cx = image.cols * THUMB_HEIGHT / image.rows;
		size.cy = THUMB_HEIGHT;
	}

	size.cx = size.cx /2 * 2;
	size.cy = size.cy /2 * 2;

	HDC			hDC, hMemDC;
	HBITMAP		hBitmap, hOldBmp;
	BITMAPINFO	BmpInfo;

	hDC = CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, THUMB_WIDTH, THUMB_HEIGHT);
	DeleteDC(hDC);

	hOldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);

	memset(&BmpInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth = image.cols;
	BmpInfo.bmiHeader.biHeight = -image.rows;
	BmpInfo.bmiHeader.biPlanes = 1;
	BmpInfo.bmiHeader.biBitCount = 24;
	
	FillRect(hMemDC, CRect(0, 0, THUMB_WIDTH, THUMB_HEIGHT), m_brushThumbBack);
	


	SetStretchBltMode(hMemDC, HALFTONE);
	StretchDIBits(hMemDC,
		(THUMB_WIDTH-size.cx)/2, (THUMB_HEIGHT-size.cy)/2, size.cx, size.cy,
		0, 0, image.cols, image.rows,
		image.data, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);

	hThumbBmp = (HBITMAP)SelectObject(hMemDC, hOldBmp);

	DeleteDC(hMemDC);

	return hThumbBmp;
}


void CThumbView::AdjustScrollSize(BOOL bRepaint)
{
	int		count = (int)m_ThumbList.size();
	CSize	sizeScroll = GetTotalSize();

	if (m_nThumbCntPerRow > 0)
	{
		sizeScroll.cy = (THUMB_HEIGHT + THUMB_GAP) * ((count + m_nThumbCntPerRow - 1) / m_nThumbCntPerRow) + THUMB_GAP;
	}
	else
	{
		sizeScroll.cy = (THUMB_HEIGHT + THUMB_GAP) + THUMB_GAP;
	}

	SetScrollSizes(MM_TEXT, sizeScroll,
		CSize(THUMB_WIDTH+THUMB_GAP, THUMB_HEIGHT+THUMB_GAP),
		CSize(THUMB_WIDTH+THUMB_GAP, THUMB_HEIGHT+THUMB_GAP));
	m_ThumbMemDC.Create(sizeScroll.cx, sizeScroll.cy);

	Invalidate(bRepaint);
}

void CThumbView::AdjustScrollPos(BOOL bRepaint)
{
	CPoint	scrollPos, selThumbPosTop, selThumbPosBottom;
	CRect	selThumbRect, rtView;
	GetClientRect(&rtView);
	scrollPos = GetScrollPosition();

	int		nRow, nCol;
	nRow = m_nSel / m_nThumbCntPerRow;
	nCol = m_nSel % m_nThumbCntPerRow;
	selThumbPosTop.SetPoint(nCol*(THUMB_WIDTH+THUMB_GAP), nRow*(THUMB_HEIGHT+THUMB_GAP));
	selThumbPosBottom.SetPoint(nCol*(THUMB_WIDTH+THUMB_GAP) + THUMB_WIDTH, nRow*(THUMB_HEIGHT+THUMB_GAP) + THUMB_HEIGHT);
	rtView.OffsetRect(scrollPos);

	if(!rtView.PtInRect(selThumbPosTop) || !rtView.PtInRect(selThumbPosBottom))
	{
		if (selThumbPosTop.y > scrollPos.y)
		{
			if (selThumbPosTop.y < rtView.bottom)
				scrollPos.y = scrollPos.y + (THUMB_HEIGHT+THUMB_GAP*2) - (rtView.bottom - selThumbPosTop.y);
			else
				scrollPos.y = scrollPos.y + (THUMB_HEIGHT+THUMB_GAP*2) + (selThumbPosTop.y - rtView.bottom);
		}
		else
		{
			scrollPos.y = selThumbPosTop.y;
		}
		if (selThumbPosTop.x > scrollPos.x)
		{
			if (selThumbPosTop.x < rtView.right)
				scrollPos.x = scrollPos.x + (THUMB_WIDTH+THUMB_GAP*2) - (rtView.right - selThumbPosTop.x);
			else
				scrollPos.x = scrollPos.x + (THUMB_WIDTH+THUMB_GAP*2) + (selThumbPosTop.x - rtView.right);
		}
		else
		{
			scrollPos.x = selThumbPosTop.x;
		}
		ScrollToPosition(scrollPos);
	}
	Invalidate(bRepaint);
}

void CThumbView::CalcSel(CPoint point)
{
	int	x = point.x / (THUMB_WIDTH + THUMB_GAP),
		y = point.y / (THUMB_HEIGHT + THUMB_GAP);

	m_nSel = -1;

	if (point.x >= x*(THUMB_WIDTH+THUMB_GAP)+THUMB_GAP)
	{
		if (point.y >= y*(THUMB_HEIGHT+THUMB_GAP)+THUMB_GAP)
		{
			m_nSel = y * m_nThumbCntPerRow + x;
			if (m_nSel+1 > (int)m_ThumbList.size())
			{
				m_nSel = -1;
			}
		}
	}
}


void CThumbView::ClearList()
{
	while (!m_ThumbList.empty())
	{
		DeleteObject(m_ThumbList[0].hThumbBmp);
		m_ThumbList.erase(m_ThumbList.begin());
	}
}


void CThumbView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if ((nChar == 37) || (nChar == 38) || (nChar == 39) || (nChar ==40))
	{
		if (m_ThumbList.size() > 0)
		{
			if (nChar == 37)	//Left Arrow
			{
				if (m_nSel > 0)
				{
					m_nSel--;
					m_nSelList.clear();
					m_nSelList.push_back(m_nSel);
					Invalidate(FALSE);
				}
			}
			else if (nChar == 39)	//Right Arrow
			{
				if (m_nSel < (int)m_ThumbList.size()-1)
				{
					m_nSel++;
					m_nSelList.clear();
					m_nSelList.push_back(m_nSel);
					Invalidate(FALSE);
				}
			}
			else if (nChar == 38)	//Top Arrow
			{
				if (m_nSel > 0)
				{
					if ((m_nSel - m_nThumbCntPerRow) >= 0)
					{
						m_nSel = m_nSel - m_nThumbCntPerRow;
						m_nSelList.clear();
						m_nSelList.push_back(m_nSel);
						Invalidate(FALSE);
					}
				}
			}
			else //Bottom Arrow
			{
				if ((m_nSel < (int)m_ThumbList.size()-1) && ((m_nSel + m_nThumbCntPerRow) <= (int)m_ThumbList.size()-1))
				{
						m_nSel = m_nSel + m_nThumbCntPerRow;
						m_nSelList.clear();
						m_nSelList.push_back(m_nSel);
						Invalidate(FALSE);
				}
			}
			AdjustScrollPos();
		}
	}

	SetFocus();
	//CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CThumbView::OnGetDlgCode()
{
	// TODO: Add your message handler code here and/or call default

	return DLGC_WANTARROWS;// CScrollView::OnGetDlgCode();
}


void CThumbView::AddString(vector<char> string)
{
	m_string=string;
}
