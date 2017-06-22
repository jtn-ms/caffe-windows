#pragma once

// CMemoryDC command target

class CMemoryDC : public CObject
{
public:
	CMemoryDC(CWnd *pOwner = NULL);
	virtual ~CMemoryDC();

	BOOL Create(int nWidth, int nHeight);
	CDC *GetDC()	{ return &m_dcMem; };
	int GetWidth()	{ return m_size.cx; }
	int GetHeight()	{ return m_size.cy; }

protected:
	CWnd *m_pOwner;
	CDC m_dcMem;
	CBitmap m_Bitmap;
	CBitmap *m_pOldBmp;
	CSize m_size;

	void Delete();
};


