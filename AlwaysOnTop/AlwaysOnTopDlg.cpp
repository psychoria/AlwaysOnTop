
// AlwaysOnTopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlwaysOnTop.h"
#include "AlwaysOnTopDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAlwaysOnTopDlg dialog



CAlwaysOnTopDlg::CAlwaysOnTopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ALWAYSONTOP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlwaysOnTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_PROCESS, m_ListWnd);
}

BEGIN_MESSAGE_MAP(CAlwaysOnTopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CAlwaysOnTopDlg message handlers

BOOL CAlwaysOnTopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_ListWnd.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListWnd.InsertColumn(0, _T("Process"), LVCFMT_LEFT, 300);
	m_ListWnd.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 75);
	m_ListWnd.InsertColumn(2, _T("Handle"), LVCFMT_LEFT, 75);

	::EnumWindows(EnumWindowsProc, (LPARAM)&m_ListWnd);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAlwaysOnTopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAlwaysOnTopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAlwaysOnTopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CAlwaysOnTopDlg::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	wchar_t szCaption[1024] = { 0, };
	DWORD dwPID = 0;

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);

	LONG dwStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	BOOL bVisible = ::IsWindowVisible(hWnd);
	BOOL bToolWindow = (dwStyle & WS_EX_TOOLWINDOW);
	BOOL bAppWindow = (dwStyle & WS_EX_APPWINDOW);
	BOOL bOwned = ::GetWindow(hWnd, GW_OWNER) ? TRUE : FALSE;

	BOOL bAppMain = (bVisible && FALSE == bOwned && ::GetParent(hWnd) == 0 && FALSE != bToolWindow);

	if (!(bVisible && (bAppWindow || (!bToolWindow && !bOwned))))
	// if (!bAppMain)
		return TRUE;

	::GetWindowText(hWnd, szCaption, 1024);
	::GetWindowThreadProcessId(hWnd, &dwPID);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPID);
	::Process32First(hSnapshot, &pEntry);

	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);
	while ((dwPID != pEntry.th32ProcessID))
	{
		BOOL bRes = ::Process32Next(hSnapshot, &pEntry);
		if (FALSE == bRes)
			break;

		hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);
	}

	CListCtrl* pCtrl = reinterpret_cast<CListCtrl*>(lParam);
	int nCount = pCtrl->GetItemCount();
	pCtrl->InsertItem(nCount, szCaption);

	wchar_t szID[20] = { 0, };
	wsprintf(szID, L"%08X", dwPID);
	pCtrl->SetItem(nCount, 1, LVIF_TEXT, szID, -1, -1, -1, -1);
	wsprintf(szID, L"%08X", hWnd);
	pCtrl->SetItem(nCount, 2, LVIF_TEXT, szID, -1, -1, -1, -1);

	return TRUE;
}