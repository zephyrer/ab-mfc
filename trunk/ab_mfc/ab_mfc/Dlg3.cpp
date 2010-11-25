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
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg3::OnBnClickedButton1)
END_MESSAGE_MAP()

class Wave
{
public:


};
// CDlg3 消息处理程序

BOOL CDlg3::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CDlg3::OnPaint()
{
	CPaintDC dc(this);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_SPLASH);			// 装入位图资源
	BITMAP bm;
	bmpDraw.GetBitmap(&bm);					// 获取位图的尺寸

	CDC memDC;								//定义一个兼容DC
	memDC.CreateCompatibleDC(&dc);			//创建DC

	CBitmap *pbmpOld = memDC.SelectObject(&bmpDraw);

	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCAND);
	memDC.SelectObject(pbmpOld);
}

int CDlg3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDlg3::OnSize(UINT nType, int cx, int cy)
{

	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


UINT DrawIt(LPVOID pParam)
{
	CClientDC dc((CWnd*)pParam);
	CDC memDC;

	HBITMAP hBitmap;
	CBitmap cBitmap;
	BITMAP bm;

	HINSTANCE hApp = ::GetModuleHandle(NULL);				// 得到应用程序的模块句柄 
	hBitmap=::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH)); 
	cBitmap.Attach(hBitmap);
	cBitmap.GetBitmap(&bm);

	memDC.CreateCompatibleDC(((CWnd*)pParam)->GetDC());
	memDC.SelectObject(&cBitmap);
	for (int i = 0; i <= bm.bmHeight; i++ )   
	{
		for (int j = 0; j <= bm.bmHeight - i; j++ )  
			dc.StretchBlt(0, j, bm.bmWidth, 1, &memDC, 0, bm.bmHeight - i, bm.bmWidth, 1, SRCCOPY);  
		Sleep(20); 
	}

	return 0;
}
void CDlg3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	AfxBeginThread(DrawIt, this, THREAD_PRIORITY_NORMAL);	// 使用多线程以防止绘图时窗口不动
}
