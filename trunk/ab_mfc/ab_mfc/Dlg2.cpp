// Dlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg2.h"


// CDlg2 �Ի���

IMPLEMENT_DYNAMIC(CDlg2, CDialog)

CDlg2::CDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg2::IDD, pParent)
{

}

CDlg2::~CDlg2()
{
}

void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg2, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlg2 ��Ϣ�������

int CDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	IniteCPUInformationGet();
	SetTimer(1, 500, NULL);
	CRect rect1;
	GetClientRect(rect1);
	//ScreenToClient(rect1);
	rect1.left +=10;
	rect1.right -= 20;
	rect1.bottom -=10;
	rect1.top += 10;
    m_oscopeCtrl1.Create(WS_VISIBLE | WS_CHILD, rect1, this) ; 
	m_oscopeCtrl1.SetRange(0.0, 100.0, 1) ;
	
	m_oscopeCtrl1.SetPerx(20);
	m_oscopeCtrl1.SetPery(10);
	//m_oscopeCtrl1.SetMovex(1);
	//m_oscopeCtrl1.SetMovey(10);

	m_oscopeCtrl1.SetYUnits("CPU") ;
	m_oscopeCtrl1.SetXUnits("Time (Windows Timer: 500 ms)") ;
	m_oscopeCtrl1.SetBackgroundColor(RGB(0, 0, 0)) ;
	m_oscopeCtrl1.SetGridColor(RGB(0, 200, 100)) ;
	m_oscopeCtrl1.SetPlotColor(RGB(255, 100, 0)) ;

	return 0;
}

void CDlg2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int CPUtime = GetCPUtime();

	m_oscopeCtrl1.AppendPoint(CPUtime);

	CDialog::OnTimer(nIDEvent);
}

void CDlg2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(1);
}
