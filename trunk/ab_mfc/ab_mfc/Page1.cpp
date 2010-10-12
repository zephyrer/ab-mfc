// Page2.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page1.h"


// CPage2 对话框

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
: CDialog(CPage1::IDD, pParent)
, m_comb_add(_T(""))
, m_list_add(_T(""))
, m_list_ctrl_add(_T(""))
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_comb_add);
	DDX_Text(pDX, IDC_EDIT2, m_list_add);
	DDX_Control(pDX, IDC_COMBO1, m_comb);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list_ctrl);
	DDX_Text(pDX, IDC_EDIT3, m_list_ctrl_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datetime);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMB_ADD, &CPage1::OnBnClickedButtonCombAdd)
	ON_BN_CLICKED(IDC_BUTTON_COMB_DEL, &CPage1::OnBnClickedButtonCombDel)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_ADD, &CPage1::OnBnClickedIdcButtonListAdd)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_DEL, &CPage1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_CTRL_ADD, &CPage1::OnBnClickedIdcButtonListCtrlAdd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CPage1::OnNMRclickList2)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_CTRL_DEL, &CPage1::OnBnClickedIdcButtonListCtrlDel)
	ON_COMMAND(ID_ADD, &CPage1::OnAdd)
	ON_COMMAND(ID_DEL, &CPage1::OnDel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPage1::OnNMCustomdrawSlider1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CPage1::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_GETTIME, &CPage1::OnBnClickedButtonGettime)
	ON_BN_CLICKED(IDC_BUTTON_GETIP, &CPage1::OnBnClickedButtonGetip)
END_MESSAGE_MAP()


// CPage2 消息处理程序
BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comb.AddString("superKiki");
	m_comb.SetCurSel(0);

	//设置list_ctrl风格
	DWORD dwStyle = ::GetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//设置新风格
	SetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//设置扩展风格
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyleEx(m_list_ctrl.m_hWnd,styles,styles);
	ListView_SetBkColor(m_list_ctrl,RGB(254,240,69));
	ListView_SetTextBkColor(m_list_ctrl,RGB(254,240,69));

	LV_COLUMN lvcol;
	lvcol.mask =LVCF_TEXT; 

	lvcol.pszText = "Name"; 
	ListView_InsertColumn(m_list_ctrl, 0, &lvcol); 
	ListView_SetColumnWidth(m_list_ctrl, 0, 100); 


	lvcol.mask =LVCF_TEXT; 
	lvcol.pszText = "Password"; 
	ListView_InsertColumn(m_list_ctrl, 1, &lvcol); 
	ListView_SetColumnWidth(m_list_ctrl, 1, 100); 
	ListView_SetExtendedListViewStyle(m_list_ctrl,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置显示样式
	/*	其中LVS_EX_FULLROWSELECT　就是前面说得整行选中
	LVS_EX_GRIDLINES　网格线（只适用与report风格的listctrl）
	LVS_EX_CHECKBOXES　前面加个checkbox
	pListCtrl->SetExtendedStyle( m_listctrl.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);
	这也是一个很重要的属性，这样的话，可以在列表中加ICON，记得windows的
	任务管理器吗，你想做得那样，这个属性也要加哦，这个我以后会讲的～
	*/
	//这是MFC种添加列的方法
	//	m_list.InsertColumn( 0, "ID", LVCFMT_LEFT, 40 );//插入列
	//	m_list.InsertColumn( 1, "NAME", LVCFMT_LEFT, 50 );

	//slider + progress
	m_slider.SetRange(0,100);
	m_progress.SetRange(0,100);
	char szNum[32];
	itoa(m_slider.GetPos(),szNum,10);
	SetDlgItemText(IDC_NUM,szNum);
	//spin
	m_spin.SetBuddy(GetDlgItem(IDC_NUM));
	m_spin.SetRange(0,5);
	m_spin.SetPos(0);
	return TRUE;
}

void CPage1::OnBnClickedButtonCombAdd()
{
	UpdateData(TRUE);
	if(m_comb_add == "")
	{
		MessageBox("没有内容！","oops!",MB_OK);
		return;
	}
	m_comb.AddString(m_comb_add);
	m_comb.SetCurSel(m_comb.GetCount()-1);
	m_comb_add = "";
	UpdateData(FALSE);
}

void CPage1::OnBnClickedButtonCombDel()
{
	if(m_comb.GetCount() == 0)
	{
		MessageBox("没东西了！","oops!",MB_OK);
	}
	UpdateData(TRUE);
	int curSel = m_comb.GetCurSel();
	m_comb.DeleteString(curSel);
	if(m_comb.GetCount() == 0)
	{
		SetDlgItemText(IDC_COMBO1,"");
	}
	else
		m_comb.SetCurSel(m_comb.GetCount()-1);
	UpdateData(FALSE);
}

void CPage1::OnBnClickedIdcButtonListAdd()
{
	UpdateData(TRUE);
	if(m_list_add == "")
	{
		MessageBox("没有内容！","oops!",MB_OK);
		return;
	}
	m_list.AddString(m_list_add);
	m_list_add = "";
	UpdateData(FALSE);
}

void CPage1::OnBnClickedButton4()
{
	int curSel = m_list.GetCurSel();
	if (LB_ERR != curSel)
	{
		m_list.DeleteString(curSel); //删除 m_mlist 选中的项目。
	}
	else
	{
		MessageBox("没有要删除的项目！","oops!",MB_OK);
	}
}

void CPage1::OnBnClickedIdcButtonListCtrlAdd()
{
	UpdateData(TRUE);
	if (m_list_ctrl_add == "")
	{
		MessageBox("没有内容！","oops!",MB_OK);
		return;
	}
    int nRow = m_list_ctrl.GetItemCount();//从后往前插入
	nRow = m_list_ctrl.InsertItem(nRow,m_list_ctrl_add);//插入行
	m_list_ctrl.SetItemText(nRow,1,m_list_ctrl_add);//设置数据
	UpdateData(FALSE);
}

void CPage1::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_MENU_LIST) );     
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	}

	*pResult = 0;
}

void CPage1::OnBnClickedIdcButtonListCtrlDel()
{
	int nChoice=m_list_ctrl.GetNextItem(-1,LVNI_SELECTED);//获得选择项. 
	if(nChoice!=-1)//当存在选择项时
	{
		m_list_ctrl.DeleteItem(nChoice);//删除项.
	}
	else
	{
		AfxMessageBox(_T("没有选择项存在，请先进行选择！"));
	}
}

void CPage1::OnAdd()
{
	OnBnClickedIdcButtonListCtrlAdd();
}

void CPage1::OnDel()
{
	OnBnClickedIdcButtonListCtrlDel();
}

void CPage1::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int curPos = m_slider.GetPos();
	m_progress.SetPos(curPos);
	char szNum[32];
	itoa(curPos,szNum,10);
	SetDlgItemText(IDC_NUM,szNum);
	*pResult = 0;
}

void CPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)//不好用
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int curPos = m_spin.GetPos();
	m_slider.SetPos(curPos);
	m_progress.SetPos(curPos);
// 	char szNum[32];
// 	itoa(curPos,szNum,10);
// 	SetDlgItemText(IDC_NUM,szNum);
	*pResult = 0;
}

void CPage1::OnBnClickedButtonGettime()//不好用
{
// 	LPSYSTEMTIME st;
// 	m_datetime.GetTime(st);
// 	char szTime[128];
// 	sprintf(szTime,"%d年%d月%d日",st->wYear,st->wMonth,st->wDay);
// 	MessageBox(szTime,"日期",MB_OK);
}

void CPage1::OnBnClickedButtonGetip()
{
	BYTE b1,b2,b3,b4;
	m_ipaddress.GetAddress(b1,b2,b3,b4);
	char szIp[128];
	sprintf(szIp,"%d.%d.%d.%d",b1,b2,b3,b4);
	MessageBox(szIp,"IP:",MB_OK);
}
