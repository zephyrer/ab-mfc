// Dlg3.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg3.h"

BOOL bend = FALSE;
CWinThread *pThread;

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
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg3::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg3::OnBnClickedButton4)
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

	/*bmpDraw.LoadBitmap(IDB_SPLASH);			// 装入位图资源
	BITMAP bm;
	bmpDraw.GetBitmap(&bm);					// 获取位图的尺寸

	memDC.CreateCompatibleDC(&dc);			//创建兼容DC

	CBitmap *pbmpOld = memDC.SelectObject(&bmpDraw);

	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCAND);
	memDC.SelectObject(pbmpOld);*/
	if (!m_imNewPicture.IsNull()) 
	{
		m_imNewPicture.Draw(dc,0,0);}
	}

int CDlg3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	pThread = NULL;
	bend = FALSE;

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
	hBitmap = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH)); 
	cBitmap.Attach(hBitmap);
	cBitmap.GetBitmap(&bm);

	memDC.CreateCompatibleDC(((CWnd*)pParam)->GetDC());
	memDC.SelectObject(&cBitmap);

	for (int i = 0; (i <= bm.bmHeight) && (!bend); i++ )   
	{
		for (int j = 0; (j <= bm.bmHeight - i) && (!bend); j++ )  
			dc.StretchBlt(0, j, bm.bmWidth, 1, &memDC, 0, bm.bmHeight - i, bm.bmWidth, 1, SRCCOPY);  

		Sleep(20); 
	}

	::DeleteObject(hBitmap);
	::DeleteDC(memDC);

	if(!bend)
		((CWnd*)pParam)->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

	return 0;
}

void CDlg3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码 
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);			// 使按钮灰化
	pThread = AfxBeginThread(DrawIt, this);					// 使用多线程以防止绘图时窗口不动
	pThread->m_bAutoDelete = FALSE;							// 线程为手动删除 
	CString strFilter;

	//Invalidate(); // 强制调用OnDraw
}

void CDlg3::OnDestroy()
{
	bend = TRUE;											// 改变变量，线程结束
	WaitForSingleObject(pThread->m_hThread, INFINITE);		// 等待线程结束
	delete pThread;											// 删除线程 

	CDialog::OnDestroy();									// 为什么放在
	// TODO: 在此处添加消息处理程序代码
}

void CDlg3::Bilinear(float k)
{  

    int nBpp = m_imPicture.GetBPP ();   
    int widthNew, heightNew;//新图像的宽度和高度
    float widthScale = (float)(1.0 / k), heightScale = (float)(1.0 / k);
    float xx, yy;
    int a, b;
    int rr, gg, bb;//保存R、G、B分量

    //得到新图像的宽度和高度

    widthNew=(int)(m_imPicture.GetWidth()*k);
    heightNew =(int)(m_imPicture.GetHeight()*k);

    //利用新图像的宽度和高度来创建新图像

    m_imNewPicture.Destroy();
    m_imNewPicture.Create(widthNew, heightNew, nBpp);

    //得到新、老图像的每行的字节数

    int nPitch = m_imPicture.GetPitch();
    int nPitchNew = m_imNewPicture.GetPitch();

    //得到新、老图像的数据指针

    LPBYTE pBitsNew = (LPBYTE)m_imNewPicture.GetBits();
    LPBYTE pBits = (LPBYTE)m_imPicture.GetBits();  

    if(m_imPicture.GetBPP() != 24)
	{
        MessageBox("必须是24位图像或8位图像");
        m_imNewPicture.Destroy();
        Invalidate();
        return ;
    }  

    for(int x = (int)k; x < widthNew - k; x++)
	{
        for(int y = (int)k; y < heightNew - k; y++)
		{
            xx = x * widthScale ;
            yy = y * heightScale ;

            if(xx <= 1e-8)
			{
                xx = 0;

            }

            if(xx > m_imPicture.GetWidth() - 2)
                xx = (float)(m_imPicture.GetWidth() - 2);

            if(yy <= 1e-8)
                yy = 0;

            if(yy > m_imPicture.GetHeight() - 2)
                yy = (float)(m_imPicture.GetHeight() - 2);

			a = (int)xx;
			b = (int)yy;         

            //分别得到对应像素的R、G、B值并用双线性插值得到新像素的R、G、B值

            int r11,r12,r21,r22;

            r11 = *(pBits + b * nPitch + 3 * a + 2);
            r12 = *(pBits + b * nPitch + 3 * (a + 1) + 2);
            r21 = *(pBits + (b + 1) * nPitch + 3 * a + 2);
            r22 = *(pBits + (b + 1) * nPitch + 3 * (a + 1) + 2);
            rr = (int)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a+1 - xx) * (yy - b)
                + r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));


            int g11, g12, g21, g22;

            g11 = *(pBits + b * nPitch + 3 * a + 1);
            g12 = *(pBits + b * nPitch + 3 * (a + 1) + 1);
            g21 = *(pBits + (b + 1) * nPitch + 3 * a + 1);
            g22 = *(pBits + (b + 1) * nPitch + 3 * (a + 1) + 1);
            gg = (int)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy- b)
				+ g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));
 

            int b11, b12, b21, b22;

            b11 = *(pBits + b * nPitch + 3 * a);
            b12 = *(pBits + b * nPitch + 3 * (a + 1));
            b21 = *(pBits + (b + 1) * nPitch + 3 * a);
            b22 = *(pBits + (b + 1) * nPitch + 3 * (a+1));
            bb = (int)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
                + b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));

            //将得到的新R、G、B值写到新图像中          

            *(pBitsNew + y * nPitchNew + x * 3) = min(255, bb);
            *(pBitsNew + y * nPitchNew + x * 3 + 1) = min(255, gg);
            *(pBitsNew + y * nPitchNew + x * 3 + 2) = min(255, rr);

        }      

    }      
    m_imPicture.Destroy();

    Invalidate();
}

void CDlg3::FilterImage(int nType)
{

	if (m_imNewPicture.IsNull())
		return;

	int smoothGauss[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1}; // 高斯模板
	int sharpLaplacian[9] = {-1, -1, -1, -1, 9, -1, -1, -1, -1}; // 拉普拉斯模板
	int opTemp[9];

	float aver; // 系数

	if( nType > 1)
		nType = 0;

	switch(nType)
	{
	case 0: // 高斯模板 平滑
		aver = (float)(1.0 / 16.0);
		memcpy(opTemp, smoothGauss, 9 * sizeof(int));
		break;

	case 1: // 拉普拉斯模板 锐化
		aver = 1.0;
		memcpy(opTemp, sharpLaplacian, 9 * sizeof(int));
		break;
	}

	int i,j;
	int nWidth = m_imNewPicture.GetWidth();
	int nHeight = m_imNewPicture.GetHeight();

	for (i = 1; i < nWidth-1; i++)
	{
		for (j = 1; j < nHeight-1; j++)
		{
			int rr = 0, gg = 0, bb = 0;
			int index = 0;
			for (int col = -1; col <= 1; col++)
			{
				for (int row = -1; row <= 1; row++)
				{
					COLORREF clr = m_imNewPicture.GetPixel(i + row, j + col);
					rr += GetRValue(clr) * opTemp[index];
					gg += GetGValue(clr) * opTemp[index];
					bb += GetBValue(clr) * opTemp[index];
					index++;
				}
			}

			rr = (int)(rr * aver);
			gg = (int)(gg * aver);
			bb = (int)(bb * aver);

			// 处理溢出点

			if ( rr > 255 ) rr = 255;
			else if ( rr < 0 ) rr = -rr;

			if ( gg > 255 ) gg = 255;
			else if ( gg < 0 ) gg = -gg;

			if ( bb > 255 ) bb = 255;
			else if ( bb < 0 ) bb = -bb;

			// 错位重写以避免前一个像素被新的像素覆盖
			m_imNewPicture.SetPixel( i - 1, j - 1, RGB(rr, gg, bb));
		}
	}

	Invalidate(); // 强制调用OnDraw
}

void CDlg3::OnBnClickedButton2()
{
	HRESULT hResult;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "全部文件|*.*|jpg|*.jpg|bmp|*.bmp|png|*.png|gif|*.gif");

	if(IDOK != dlg.DoModal())
		return;

	m_imPicture.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = m_imPicture.Load(dlg.GetFileName());
	if (FAILED(hResult)) 
	{
		MessageBox(_T("调用图像文件失败！"));
		return;
	}

	// 设置主窗口标题栏内容

	CString str;
	str.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(str + _T(" - ") +dlg.GetFileName());
	Bilinear(0.5);
}

void CDlg3::OnBnClickedButton3()
{
	FilterImage(0);
}

void CDlg3::OnBnClickedButton4()
{
	FilterImage(1);
}
