// Dlg3.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg3.h"


// CDlg3 对话框

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
// CDlg3 消息处理程序

BOOL CDlg3::OnEraseBkgnd(CDC* pDC)
{
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // 创建兼容 DC作为内存 DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //将位图选入内存 DC 
	BITMAP bm;
	::GetObject(hbmp, sizeof(bm), &bm);
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  bm.bmWidth,  bm.bmHeight,  hmdc, 0, 0, SRCCOPY);  // 将内存 DC 中的内容拷贝到设备 DC以显示 
	// 清理，防止内存泄漏
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 

	return CWnd::OnEraseBkgnd(pDC);
}

void CDlg3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//CDC cdc = dc.;
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	HDC hmdc = ::CreateCompatibleDC(dc.m_hDC);  // 创建兼容 DC作为内存 DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //将位图选入内存 DC 
	BITMAP bm;
	::GetObject(hbmp, sizeof(bm), &bm);
	SetPixel(hmdc, 0, 0, RGB(200, 0, 9));
	::BitBlt(dc.m_hDC, 0,  0,  bm.bmWidth,  bm.bmHeight,  hmdc, 0, 0, SRCCOPY);
	
	// 清理，防止内存泄漏
	//UpdateLayeredWindow
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 

}
