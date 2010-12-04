// Page4.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page4.h"


// Page4 �Ի���

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


// Page4 ��Ϣ�������
BOOL Page4::OnInitDialog()
{
	CDialog::OnInitDialog();
	//����list_ctrl���
	DWORD dwStyle = ::GetWindowLong(m_list.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//�����·��
	SetWindowLong(m_list.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//������չ���
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyleEx(m_list.m_hWnd,styles,styles);
	ListView_SetBkColor(m_list, RGB(254,240,69));
	ListView_SetTextBkColor(m_list,RGB(254,240,69));

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_TEXT; 

	lvcol.pszText = "ӳ������"; 
	ListView_InsertColumn(m_list, 0, &lvcol); 
	ListView_SetColumnWidth(m_list, 0, 100); 


	lvcol.mask =LVCF_TEXT; 
	lvcol.pszText = "PID"; 
	ListView_InsertColumn(m_list, 1, &lvcol); 
	ListView_SetColumnWidth(m_list, 1, 100); 
	ListView_SetExtendedListViewStyle(m_list,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//������ʾ��ʽ

	return true;
}
void Page4::OnBnClickedRefresh()
{
	UpdateData(TRUE);

    int nRow = m_list.GetItemCount();//�Ӻ���ǰ����
	nRow = m_list.InsertItem(nRow, "12");//������
	m_list.SetItemText(nRow,1, "123");//��������
	UpdateData(FALSE);
}

void Page4::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
