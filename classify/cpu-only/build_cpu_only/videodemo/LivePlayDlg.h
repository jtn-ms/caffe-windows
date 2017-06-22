#pragma once

#include "MemoryDC.h"
#include <string>
// CLivePlayDlg dialog
class CLivePlayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLivePlayDlg)

public:
	CLivePlayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLivePlayDlg();

	// Dialog Data
	enum { IDD = IDD_LIVE_PLAY_DLG };
	// User-defined
	void Initialize(std::string);
	void Initialize(int width,int height);
	void Play();
	void Disconnect();
	void SetFrame(LPBYTE pFrame = NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int			m_nWidth;
	int			m_nHeight;
	CMemoryDC	m_memDC;
	BITMAPINFO  m_BmpInfo;
	BOOL	m_bIsRecording;
	LPBYTE	m_pCurFrame;
	char	*m_pDrawData;
	CRect	m_oldRect;
	CFont		*m_pFont;
	std::string		m_FileName;
	std::string		m_FilePath;
	std::string		m_OldFileName;

	bool			m_bClose;
public:	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL DestroyWindow();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
