#pragma once


// WindowsListCtrl

class WindowsListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(WindowsListCtrl)

public:
	WindowsListCtrl();
	virtual ~WindowsListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};


