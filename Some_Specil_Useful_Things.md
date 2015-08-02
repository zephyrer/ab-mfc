# Introduction #

这里介绍一些比较有用的方法，如浏览文件夹、拖放文件、显示CPU曲线等等


# 一些常用的方法 #



# 一些比较有用的技巧 #
## 1.遍历文件夹 ##
```
void CPage2::BrowseFile(int CallNum, CString strFile)
{
	CallNum++;
	CFileFind ff;
	CString szDir = strFile; 

	if(szDir.Right(1) != "\\")
		szDir += "\\";

	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{	 
			//如果是一个子目录，用递归继续往深一层找
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileTitle();
			int i =0;
			switch(CallNum)
			{
			case 1:	  
				strHTFir = m_FileTree.InsertItem(strTitle,0,0,NULL);						
				break;
			case 2:
				strHTSec = m_FileTree.InsertItem(strTitle,0,0,strHTFir);											
				break;
			case 3:
				strHtThi = m_FileTree.InsertItem(strTitle,0,0,strHTSec);					
				break;
			case 4:
				strHtFor = m_FileTree.InsertItem(strTitle,0,0,strHtThi);					
				break;
			default:
				strHtFif = m_FileTree.InsertItem(strTitle,0,0,strHtFor);
				break;					
			}
			BrowseFile(CallNum,strPath);
		}
		else if(!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件
			CString strPath;
			CString strTitle;
			strPath = ff.GetFilePath();
			strTitle = ff.GetFileTitle();
			switch(CallNum)
			{
			case 1:
				strRoot = m_FileTree.InsertItem(strTitle,0,0,NULL);
				break;
			case 2:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHTFir);
				break;
			case 3:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHTSec);				
				break;
			case 4:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtThi);
				break;
			case 5:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtFor);
				break;
			default:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtFif);
				break;
			}
		}
	}
	ff.Close();//关闭
}
```
我们可以将其应用到树形控件当中。（这里就是这样做的）

## 2.拖放文件 ##
所以要设置窗口或者控件可以接收文件,然后我们就可以这样做了:
```
void CPage2::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR strFile[MAX_PATH] ;
	UINT cFiles = ::DragQueryFile(hDropInfo, 0xffffffff, 0, 0);     // 取得文件数

	for (UINT count = 0; count < cFiles; count++)
	{
		if (cFiles > 0)
        {		
            ::DragQueryFile(hDropInfo, count, strFile, MAX_PATH);	// 得到拖放的文件名
			AfxMessageBox(strFile);									// 提示拖放的文件名
			BrowseFile(0, strFile);									// 浏览文件目录
        }
	}
	
	CDialog::OnDropFiles(hDropInfo);
}
```

## 3.产生自己想要的不规则窗口 ##
```
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
						RGBQUAD* Linia32RGBA = (RGBQUAD* )p32;

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
								if(x == mousex - rect.left && y== mousey - rect.top)
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
```
当然你也可以用SetWindowsAtrrbuide实现，但是就没有这么灵活了。

## 4.托盘图标 ##
```
//托盘图标
	m_tnid.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnid.hWnd = this->m_hWnd; 
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;

	//用户定义的回调消息 
	CString szToolTip; 
	szToolTip = _T("superkiki~"); 
	_tcscpy(m_tnid.szTip, szToolTip); 
	m_tnid.uID = IDR_MAINFRAME;

	HICON hIcon; 
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_UBUNTU); 
	m_tnid.hIcon = hIcon; 
	::Shell_NotifyIcon(NIM_ADD, &m_tnid); 
	if(hIcon)
		::DestroyIcon(hIcon);
```

## 5.CPU使用率的获得 ##
```
#define SystemBasicInformation 0 
#define SystemPerformanceInformation 2 
#define SystemTimeInformation 3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

typedef struct 
{ 
	DWORD dwUnknown1; 
	ULONG uKeMaximumIncrement; 
	ULONG uPageSize; 
	ULONG uMmNumberOfPhysicalPages; 
	ULONG uMmLowestPhysicalPage; 
	ULONG uMmHighestPhysicalPage; 
	ULONG uAllocationGranularity; 
	PVOID pLowestUserAddress; 
	PVOID pMmHighestUserAddress; 
	ULONG uKeActiveProcessors; 
	BYTE bKeNumberProcessors; 
	BYTE bUnknown2; 
	WORD wUnknown3; 
} SYSTEM_BASIC_INFORMATION;

typedef struct 
{ 
	LARGE_INTEGER liIdleTime; 
	DWORD dwSpare[76]; 
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct 
{ 
	LARGE_INTEGER liKeBootTime; 
	LARGE_INTEGER liKeSystemTime; 
	LARGE_INTEGER liExpTimeZoneBias; 
	ULONG uCurrentTimeZoneId; 
	DWORD dwReserved; 
} SYSTEM_TIME_INFORMATION;

typedef LONG (WINAPI *PROCNTQSI)(UINT, PVOID, ULONG, PULONG);
PROCNTQSI NtQuerySystemInformation;

SYSTEM_BASIC_INFORMATION SysBaseInfo;


BOOL IniteCPUInformationGet(void);	// 初始化CPU信息获得
UINT GetCPUtime(void);			// 得到CPU使用率

BOOL IniteCPUInformationGet(void)
{	
	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");
	if (!NtQuerySystemInformation) 
		return -1;

	LONG status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;
	return 0;
}

UINT GetCPUtime(void) 
{ 
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
	SYSTEM_TIME_INFORMATION SysTimeInfo; 
	double dbIdleTime = 0.0; 
	double dbSystemTime; 
	LONG status; 
	static LARGE_INTEGER liOldIdleTime = {0,0}; 
	static LARGE_INTEGER liOldSystemTime = {0,0};

	// 得到系统中的进程数
	status = NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo,sizeof(SysTimeInfo),0); 
	if (status != NO_ERROR) 
		return -1;

	// 得到系统使用时间 
	status = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;
	
	if (liOldIdleTime.QuadPart != 0) 
	{ 
			dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
			dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);
			dbIdleTime = dbIdleTime / dbSystemTime;
			dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5; 
#ifdef DEBUG
			printf("%3d%", (UINT)dbIdleTime); // DEBUG
#endif
	}

	liOldIdleTime = SysPerfInfo.liIdleTime; 
	liOldSystemTime = SysTimeInfo.liKeSystemTime;

 return (UINT)dbIdleTime;
}
```