// Page2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page2.h"


// CPage2 �Ի���

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CPage2::IDD, pParent)
{

}

CPage2::~CPage2()
{
	
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_FileTree);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILESEARCH, &CPage2::OnBnClickedButtonFilesearch)
	ON_WM_PAINT()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPage1 ��Ϣ�������

void CPage2::OnBnClickedButtonFilesearch()
{
	BrowseFile(0, "�ɼ���");//����"�ɼ���"�ļ����ڵ�����Ŀ¼
}

BOOL CPage2::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_iImageList.Create(24, 24, TRUE, 1, 0);
	m_FileTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT |  TVS_HASLINES | TVS_INFOTIP);  // ���Ӻš�����

	HICON hIcon = NULL;
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(IDI_ICON_UBUNTU), IMAGE_ICON, 24, 24, 0);
	m_iImageList.Add(hIcon);
	m_FileTree.SetImageList ( &m_iImageList,TVSIL_NORMAL );

	//HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	//HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 

	//HRGN hrgn = ::BitmapToRegion(GetDlgItem(IDC_PIC)->m_hWnd, hbmp, -2, -1, -1);
	//if (hrgn)
		//SetWindowRgn(hrgn, TRUE);

	return TRUE;
}

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
			//�����һ����Ŀ¼���õݹ��������һ����
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
			//��ʾ��ǰ���ʵ��ļ�
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
	ff.Close();//�ر�
}
void CPage2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


void CPage2::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR strFile[MAX_PATH] ;
	UINT cFiles = ::DragQueryFile(hDropInfo, 0xffffffff, 0, 0);     // ȡ���ļ���

	for (UINT count = 0; count < cFiles; count++)
	{
		if (cFiles > 0)
        {		
            ::DragQueryFile(hDropInfo, count, strFile, MAX_PATH);	// �õ��Ϸŵ��ļ���
			AfxMessageBox(strFile);									// ��ʾ�Ϸŵ��ļ���
			BrowseFile(0, strFile);									// ����ļ�Ŀ¼
        }
	}
	
	CDialog::OnDropFiles(hDropInfo);
}
