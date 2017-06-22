
// testclassifier-guiDlg.h : header file
//

#pragma once

#include "ThumbView.h"
// CtestclassifierguiDlg dialog
class CtestclassifierguiDlg : public CDialogEx
{
// Construction
public:
	CtestclassifierguiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTCLASSIFIERGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
private:
	CView *m_ImageView;
	CThumbView *m_pThumbView;
	char *m_curFullPath;
	char *m_curFileName;
	cv::Mat m_img;
	vector<cv::Mat> m_ImgList;
	vector<CString> m_fnList;
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton1();
};
