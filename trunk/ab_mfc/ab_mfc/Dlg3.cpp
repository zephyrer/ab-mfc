// Dlg3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg3.h"


// CDlg3 �Ի���

IMPLEMENT_DYNAMIC(CDlg3, CDialog)

CDlg3::CDlg3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg3::IDD, pParent)
{

}

CDlg3::~CDlg3()
{
}

void CDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg3, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

class Wave
{
public:


};
// CDlg3 ��Ϣ�������

BOOL CDlg3::OnEraseBkgnd(CDC* pDC)
{
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // �������� DC��Ϊ�ڴ� DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //��λͼѡ���ڴ� DC 
	BITMAP bm;
	::GetObject(hbmp, sizeof(bm), &bm);
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  bm.bmWidth,  bm.bmHeight,  hmdc, 0, 0, SRCCOPY);  // ���ڴ� DC �е����ݿ������豸 DC����ʾ 
	// ������ֹ�ڴ�й©
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 

	return CWnd::OnEraseBkgnd(pDC);
}

void CDlg3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//CDC cdc = dc.;
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	HDC hmdc = ::CreateCompatibleDC(dc.m_hDC);  // �������� DC��Ϊ�ڴ� DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //��λͼѡ���ڴ� DC 
	BITMAP bm;
	::GetObject(hbmp, sizeof(bm), &bm);
	SetPixel(hmdc, 0, 0, RGB(200, 0, 9));
	::BitBlt(dc.m_hDC, 0,  0,  bm.bmWidth,  bm.bmHeight,  hmdc, 0, 0, SRCCOPY);
	
	// ������ֹ�ڴ�й©
	//UpdateLayeredWindow
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 

}
