// SplashWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "SplashWnd.h"
#define ALLOC_UNIT  100	

// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);  // ע�ᴰ���� 
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, lpszWndClass, NULL, 
		WS_POPUP,  
		CW_USEDEFAULT,  CW_USEDEFAULT,  SPLASH_WIDTH,  SPLASH_HEIGHT,  NULL, 
		NULL, NULL);  // �������� 
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
{//�����a���Ǻ�ɫ�������ǵ��෽�棬��ĳ���ȡ��һ��
	HRGN hRgn = NULL;
	if(hBmp)
	{
		HDC hMemDC = CreateCompatibleDC(NULL);
		if(hMemDC)
		{
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);// ���ͼƬ��С
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
//�ؼ���
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
								// ��ģ����Ⱦ
								if(cTransparentColor > -1)
								{
									if(b == cTransparentColor)
									break;
								}
								else if(c == b)// ������ǡ�͸������     			
									break;
								RECT rect;
								GetWindowRect(hWnd, &rect);
								if(x == mousex- rect.left && y== mousey - rect.top)
									break;//��ע�⣬��һ���ǵ�͸�Ĺؼ�
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
			DeleteDC(hMemDC);//ע��һ��Ҫ�ͷ�
		}	
	}
	return hRgn;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRGN hrgn = ::BitmapToRegion(this->GetSafeHwnd(), hbmp, -2, -1, -1);
	if (hrgn)
		SetWindowRgn(hrgn, TRUE);
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