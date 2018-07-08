
// AlwaysOnTopDlg.h : header file
//

#pragma once
#include "WindowsListFactory.h"


// CAlwaysOnTopDlg dialog
class CAlwaysOnTopDlg : public CDialog
{
// Construction
public:
	CAlwaysOnTopDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALWAYSONTOP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	void ReplaceCtrl();
	void UpdateWindowList();

private:
	CListCtrl m_ListWnd;
	CButton m_BtnTop;
	WindowsListFactory m_proclist;
	CImageList m_imageList;

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnTop();
};
