// Page4.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page4.h"


// Page4 对话框

IMPLEMENT_DYNAMIC(Page4, CDialog)

Page4::Page4(CWnd* pParent /*=NULL*/)
	: CDialog(Page4::IDD, pParent)
{

}

Page4::~Page4()
{
}

void Page4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(Page4, CDialog)
	ON_BN_CLICKED(IDC_REFRESH, &Page4::OnBnClickedRefresh)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Page4::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// Page4 消息处理程序
BOOL Page4::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设置list_ctrl风格
	DWORD dwStyle = ::GetWindowLong(m_list.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//设置新风格
	SetWindowLong(m_list.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//设置扩展风格
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyleEx(m_list.m_hWnd,styles,styles);
	ListView_SetBkColor(m_list, RGB(254,240,69));
	ListView_SetTextBkColor(m_list,RGB(254,240,69));

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_TEXT; 

	lvcol.pszText = "映象名称"; 
	ListView_InsertColumn(m_list, 0, &lvcol); 
	ListView_SetColumnWidth(m_list, 0, 100); 


	lvcol.mask =LVCF_TEXT; 
	lvcol.pszText = "PID"; 
	ListView_InsertColumn(m_list, 1, &lvcol); 
	ListView_SetColumnWidth(m_list, 1, 100); 
	ListView_SetExtendedListViewStyle(m_list,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置显示样式

	return true;
}
void Page4::OnBnClickedRefresh()
{
	UpdateData(TRUE);

    int nRow = m_list.GetItemCount();//从后往前插入
	nRow = m_list.InsertItem(nRow, "12");//插入行
	m_list.SetItemText(nRow,1, "123");//设置数据
	UpdateData(FALSE);
}

void Page4::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
