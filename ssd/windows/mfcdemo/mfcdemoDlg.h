
// mfcdemoDlg.h : header file
//

#pragma once

#include "LivePlayDlg.h"
#include <opencv/cv.h>  
// CmfcdemoDlg dialog
class CmfcdemoDlg : public CDialogEx
{
// Construction
public:
	CmfcdemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CLivePlayDlg* m_pLivePlay;
	CView *m_ImageView;
	char *m_curFullPath;
	char *m_curFileName;
	cv::Mat m_Mat;
public:
	void createWnd();
	void moveWnd();
	void showWnd();
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
