#pragma once
#include "afxwin.h"


// Page4 �Ի���

class Page4 : public CDialog
{
	DECLARE_DYNAMIC(Page4)

public:
	Page4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Page4();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_TAB_DLG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
public:
	afx_msg void OnBnClickedRefresh();
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
