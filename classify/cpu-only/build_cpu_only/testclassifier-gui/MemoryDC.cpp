// MemoryDC.cpp : implementation file
//

#include "stdafx.h"
#include "testclassifier-gui.h"
#include "MemoryDC.h"


// CMemoryDC

CMemoryDC::CMemoryDC(CWnd *pOwner)
	: m_pOwner(pOwner)
	, m_pOldBmp(NULL)
	, m_size(0, 0)
{
}

CMemoryDC::~CMemoryDC()
{
	Delete();
}


// CMemoryDC member functions

BOOL CMemoryDC::Create(int nWidth, int nHeight)
{
	Delete();

	CDC		dc;
	BOOL	bRet;

	if (!dc.CreateIC(_T("DISPLAY"), NULL, NULL, NULL))
	{
		return FALSE;
	}

	bRet = m_dcMem.CreateCompatibleDC(&dc);

	if (!bRet)
	{
		dc.DeleteDC();

		return FALSE;
	}

	bRet = m_Bitmap.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	dc.DeleteDC();

	if (!bRet)
	{
		m_dcMem.DeleteDC();

		return FALSE;
	}

	m_pOldBmp = m_dcMem.SelectObject(&m_Bitmap);
	if (!m_pOldBmp)
	{
		m_Bitmap.DeleteObject();
		m_dcMem.DeleteDC();

		return FALSE;
	}

	m_dcMem.SetBkMode(TRANSPARENT);

	m_size.SetSize(nWidth, nHeight);

	if (m_pOwner)
	{
		m_dcMem.SelectObject(m_pOwner->GetFont());
	}

	return TRUE;
}


void CMemoryDC::Delete()
{
	if (m_pOldBmp)
	{
		m_dcMem.SelectObject(m_pOldBmp);
		m_Bitmap.DeleteObject();
		m_pOldBmp = NULL;
		m_dcMem.DeleteDC();
		m_size.SetSize(0, 0);
	}
}
