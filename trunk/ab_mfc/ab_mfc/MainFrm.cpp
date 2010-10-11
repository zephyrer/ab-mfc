// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ab_mfc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	flag = true;
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//����ͼ��
	m_tnid.cbSize=sizeof(NOTIFYICONDATA); 
	m_tnid.hWnd=this->m_hWnd; 
	m_tnid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	m_tnid.uCallbackMessage=MYWM_NOTIFYICON; 
	//�û�����Ļص���Ϣ 
	CString szToolTip; 
	szToolTip=_T("superkiki~"); 
	_tcscpy(m_tnid.szTip, szToolTip); 
	m_tnid.uID=IDR_MAINFRAME; 
	HICON hIcon; 
	hIcon=AfxGetApp()->LoadIcon(IDI_ICON_UBUNTU); 
	m_tnid.hIcon=hIcon; 
	::Shell_NotifyIcon(NIM_ADD,&m_tnid); 
	if(hIcon)::DestroyIcon(hIcon); 
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	 
	switch(message)
	{ 
		case MYWM_NOTIFYICON: 
		//������û��������Ϣ 
		if(lParam==WM_LBUTTONDBLCLK)
		{ 
		//���˫��ʱ�����ڳ��� 
			if (flag == true)
			{
				flag = !flag;
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); 
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
				flag = !flag;
			}
		} 
		else if(lParam==WM_RBUTTONDOWN)
		{ //����Ҽ���������ѡ�� 
			CMenu menu; 
			menu.LoadMenu(IDR_MENU_ICON); //�������ȶ����ѡ�� 
			CMenu* pMenu=menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos); 
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd()); 
		} 
		break; 
		case WM_SYSCOMMAND: 
		//�����ϵͳ��Ϣ 
		if(wParam == SC_MINIMIZE)
		{ 
		//���յ���С����Ϣʱ���������� 
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); 
			return 0; 
		} 
		break;
	} 
	return CFrameWnd::WindowProc(message, wParam, lParam); 
} 

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);//�رմ���ʱɾ��ͼ��

}
