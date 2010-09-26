// SplashWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "SplashWnd.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass= AfxRegisterWndClass(NULL);  // ע�ᴰ���� 
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, lpszWndClass, NULL, 
		WS_POPUP,  
		CW_USEDEFAULT,  CW_USEDEFAULT,  SPLASH_WIDTH,  SPLASH_HEIGHT,  NULL, 
		NULL, NULL);  // �������� 
}

CSplashWnd::~CSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSplashWnd ��Ϣ�������



int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CenterWindow();  // �������洰�ھ��� 
	SetTimer(1, 3000, NULL);  // ����������ʾʱ��Ϊ 3�� 
	AnimateWindow(1200,AW_BLEND);  // ���ڽ��ԣ�ʵ�ֵ���Ч����ʱ�� 1.2 �� 
	return 0;
}

void CSplashWnd::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	AnimateWindow(800,AW_BLEND|AW_HIDE);  // ���ڽ�����ʵ�ֵ���Ч����ʱ�� 0.8 �� 
	CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);  // ���ٶ�ʱ�� 
	PostMessage(WM_CLOSE);  // �ر��������� 
	CWnd::OnTimer(nIDEvent);
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // �������� DC��Ϊ�ڴ� DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //��λͼѡ���ڴ� DC 
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  SPLASH_WIDTH,  SPLASH_HEIGHT,  hmdc, 0, 0, SRCCOPY);  // ���ڴ� DC �е����ݿ������豸 DC����ʾ 
	// ������ֹ�ڴ�й© 
	::SelectObject(hmdc, hbmpOld); 
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 
	return CWnd::OnEraseBkgnd(pDC);
}
