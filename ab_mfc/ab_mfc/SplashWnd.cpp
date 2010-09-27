// SplashWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "SplashWnd.h"
#define ALLOC_UNIT  100	

// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);  // 注册窗口类 
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, lpszWndClass, NULL, 
		WS_POPUP,  
		CW_USEDEFAULT,  CW_USEDEFAULT,  SPLASH_WIDTH,  SPLASH_HEIGHT,  NULL, 
		NULL, NULL);  // 创建窗口 
	SetLayeredWindowAttributes(0, 0, 3);
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



HRGN BitmapToRegion (HWND hWnd, HBITMAP hBmp, COLORREF cTransparentColor, int mousex, int mousey)
{//本来黙认是黑色，但考虑到多方面，便改成了取第一点
	HRGN hRgn = NULL;
	if(hBmp)
	{
		HDC hMemDC = CreateCompatibleDC(NULL);
		if(hMemDC)
		{
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);// 获得图片大小
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};

			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if(hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);	
				HDC hDC = CreateCompatibleDC(hMemDC);
				if(hDC)
				{	
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;	
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);				
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pObject = (RGNDATA *)GlobalLock(hData);
					pObject->rdh.dwSize = sizeof(RGNDATAHEADER);
					pObject->rdh.iType = RDH_RECTANGLES;
					pObject->rdh.nCount = pObject->rdh.nRgnSize = 0;
					SetRect(&pObject->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);				
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
//关键：
					for(int y = 0; y < bm.bmHeight; y++)
					{
						for(int x = 0; x < bm.bmWidth; x++)
						{
							int x0 = x;
							LONG *p = (LONG *)p32 + x;

							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								BYTE c = *p32;

								if(x != 0)
									b = (GetRValue(*p) + GetRValue(*(p - 1)) + GetRValue(*(p + 1))) / 3;
								// 简单模糊渲染
								if(cTransparentColor > -1)
								{
									if(b == cTransparentColor)
									break;
								}
								else if(c == b)// 这个点是“透明”的     			
									break;
								RECT rect;
								GetWindowRect(hWnd, &rect);
								if(x == mousex- rect.left && y== mousey - rect.top)
									break;//请注意，这一点是点透的关键
								p++;
								x++;
							}
							if(x > x0)
							{	
								if(pObject->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pObject = (RGNDATA *)GlobalLock(hData);
								}

								RECT *pr = (RECT *)&pObject->Buffer;
								SetRect(&pr[pObject->rdh.nCount], x0, y, x, y+1);
								if(x0 < pObject->rdh.rcBound.left)
									pObject->rdh.rcBound.left = x0;
								if(y < pObject->rdh.rcBound.top)
									pObject->rdh.rcBound.top = y;
								if(x > pObject->rdh.rcBound.right)
									pObject->rdh.rcBound.right = x;
								if(y+1 > pObject->rdh.rcBound.bottom)
									pObject->rdh.rcBound.bottom = y+1;
                                pObject->rdh.nCount++;
								if(pObject->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pObject);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pObject->rdh.nCount = 0;
									SetRect(&pObject->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						p32 -= bm32.bmWidthBytes;
					}

					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pObject);
					if (hRgn)
					{
				
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}
				DeleteObject(SelectObject(hMemDC, holdBmp));
			}
			DeleteDC(hMemDC);//注意一定要释放
		}	
	}
	return hRgn;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRGN hrgn = ::BitmapToRegion(this->GetSafeHwnd(), hbmp, -2, -1, -1);
	if (hrgn)
		SetWindowRgn(hrgn, TRUE);
	// TODO:  在此添加您专用的创建代码
	CenterWindow();  // 启动画面窗口居中
	
	SetTimer(1, 3000, NULL);  // 启动画面显示时间为 3秒 
	AnimateWindow(1200,AW_BLEND);  // 窗口渐显，实现淡入效果，时间 1.2 秒 
	return 0;
}

void CSplashWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	AnimateWindow(800,AW_BLEND|AW_HIDE);  // 窗口渐隐，实现淡出效果，时间 0.8 秒 
	CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);  // 销毁定时器 
	PostMessage(WM_CLOSE);  // 关闭启动画面 
	CWnd::OnTimer(nIDEvent);
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // 创建兼容 DC作为内存 DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //将位图选入内存 DC 
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  SPLASH_WIDTH,  SPLASH_HEIGHT,  hmdc, 0, 0, SRCCOPY);  // 将内存 DC 中的内容拷贝到设备 DC以显示 
	// 清理，防止内存泄漏 
	::SelectObject(hmdc, hbmpOld); 
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 
	return CWnd::OnEraseBkgnd(pDC);
}