#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CPage1 �Ի���

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage1();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_TAB_DLG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCombAdd();
	CString m_comb_add;
	CString m_list_add;
	CComboBox m_comb;
	CListBox m_list;
	afx_msg void OnBnClickedButtonCombDel();
	afx_msg void OnBnClickedIdcButtonListAdd();
	afx_msg void OnBnClickedButton4();
	CListCtrl m_list_ctrl;
	CString m_list_ctrl_add;
	afx_msg void OnBnClickedIdcButtonListCtrlAdd();
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedIdcButtonListCtrlDel();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	CProgressCtrl m_progress;
	CSliderCtrl m_slider;
	CDateTimeCtrl m_datetime;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_spin;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGettime();
	CIPAddressCtrl m_ipaddress;
	afx_msg void OnBnClickedButtonGetip();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnThemechangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};