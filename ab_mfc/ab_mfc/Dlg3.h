#pragma once


// CDlg3 �Ի���

class CDlg3 : public CDialog
{
	DECLARE_DYNAMIC(CDlg3)
	friend UINT DrawIt(LPVOID pParam);
public:
	CDlg3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg3();
void  UpdateLayered();
// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HBITMAP m_hmemBmp;
	BYTE* m_pBits;
	DWORD m_dwSize;
	DWORD m_dwEffWidth;
	BOOL  m_bEscape;
	CRect m_rect;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnPaint();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedButton1();
};