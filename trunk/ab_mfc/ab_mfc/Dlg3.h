#pragma once


// CDlg3 �Ի���

class CDlg3 : public CDialog
{
	DECLARE_DYNAMIC(CDlg3)

public:
	CDlg3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg3();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnPaint();
};
