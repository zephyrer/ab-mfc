# Introduction #

控件是对数据和方法的封装。控件可以有自己的属性和方法。属性是控件数据的简单访问者。方法则是控件 的一些简单而可见的功能。
[参照此处](http://baike.baidu.com/view/185331.htm)


# Details #
## 1.对话框(Dialog) ##

在资源视图的树形的Dialog项里右键，从弹出的菜单中选择“插入Dialog”
然后我们就可以在该对话框编辑了，我们可以在选中该对话框后查看其属性，然后对其属性进行修改
这时你会问如何使这个对话框与菜单关联起来，请参照第一个wiki里所说的做。

## 2.按钮(Button) ##
我们首先在资源视图的对话框里增加按钮（从工具窗口里拖一个Button到对话框的任一个位置）
然后可以双击该控件或者点击查看事件（针对一个控件的事件是比较多的，请视情况选择自己想要的事件）以添加OnOK事件，然后我们就可以在新生成的代码里进行编辑了，如下面的代码所示：
```
void CAboutDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}
```
我们同样可以在类视图里看到CAboutDlg里有成员函数OnOK
请注意，现在的代码里只有一行CDialog::OnOK()它会调用基类CDialog的OnOK事件，结束本对话框。
当然，我们也可以写别的代码以完成我们所想要的功能，如添加
```
        AfxMessageBox(_T("oops!"));
```
这样我们在按了OK按钮后就可以弹出一个显示了oops的提示窗口。
Button的事件一般会反映在其所在窗口的消息映射表上，如
```
BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDC_OK, &CPage1::OnOK)
END_MESSAGE_MAP()
```

同样一个Button控件也是有属性的，我们同样可以在其属性窗口中对其进行修改。当然我们也可以通过代码来对其进行属性的更改，别的控件也类似，如：
```
        CString strNum;
	strNum.Format("%d", curPos);
	SetDlgItemText(IDC_OK, strNum);
```
这里用到了CString这么一个东东，它也是MFC一个重要的东东，我们后面会详细讲的，这里仅要注意它是通过Format来转换数据样式的，本例中是将curpos这么一个整数转换顾strNum这么一个CString对象。用\_itoa的童鞋有点掉了哦！（当然某些特定情况也还是要用到\_itoa等一系列标准C中的字符处理函数）
而SetDlgItemText是CDialog的成员函数，重载了API函数，它可以设置相关控件上的文字。

## 3.编辑框(Edit Contrl) ##
同样是从工具箱窗口中拖出编辑框控件到对话框上，然后可以更改其属性以指定其能接受的最长字节以及接受的是文字还是数字。
它作为一个接受外部数据的一个通道，一般而言是要定义一个变量与之关联的，也就是右键，为其增加一个变量，在类里就会有一个数据与之对应，请注意该变量的数据类型。
然后我们就可以通过GetItemText来得到框中的数据，或者用SetItemText更改框中的数据。

同样它也有事件，如右键事件，这个跟Button基本类似。

## 4.静态文字(Static Text) ##
除去不能直接去编辑，其它的跟编辑框都是类似的。

## 5.组合框(Combo Box) ##
如何创建一个组合框这里就不再赘述了，我们主要关心它是怎么初始化以及事件关联的。
我们可以在对话框时对其初始化(别的控件也一样)，如：
```
BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comb.AddString("superKiki");
	m_comb.SetCurSel(0);

	return TRUE;
}
```
往其下拉菜单里加成员可以这样写：
```
        CString m_comb_add = "oops";
        m_comb.AddString(m_comb_add);
	m_comb.SetCurSel(m_comb.GetCount()-1);
```
删除一个成员可以这样写：
```
	int curSel = m_comb.GetCurSel();
	m_comb.DeleteString(curSel);
```
而其编辑框里的事件与一般的编辑框是基本一样的。

## 6.列表框(ListBox) ##
列表框相对前面那些控件都要复杂些。
例如其初始化，可能如下：
```
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
	   ListView_SetExtendedListViewStyle(m_list_ctrl,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
```

而通过其它控件往其加数据也是有很多方式的，例如通过按钮以及编辑框往里面增删数据：
```
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
		MessageBox("没有要删除的项目！", "oops!", MB_OK);
	}
}
```
还可以通过右键菜单达到同样效果：
```
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
	int nChoice = m_list_ctrl.GetNextItem(-1, LVNI_SELECTED);// 获得选择项. 
	if (nChoice != -1)					 // 当存在选择项时
	{
		m_list_ctrl.DeleteItem(nChoice);	         // 删除项.
	}
	else
	{
		AfxMessageBox(_T("没有选择项存在，请先进行选择！"));
	}
}
```

## 7.滑块(Slider) + 进度条(Progress) ##
滑块经常用于控制一个连续变量，如音量什么的，双击它，然后为其关联事件：
```
void CPage1::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int curPos = m_slider.GetPos();
	m_progress.SetPos(curPos);

	CString strNum;
	strNum.Format("%d", curPos);
	SetDlgItemText(IDC_NUM, strNum);
	*pResult = 0;
}
```
这里通过int curPos = m\_slider.GetPos();得到了滑块现在所在的位置，然后通过m\_progress.SetPos(curPos);将进度条的位置也定为相同位置。后面三句则是将其数值显示在静态文本上。这两个控件都有方向，但并不影响其使用。

## 8.翻页按钮(Spin) ##
它经常用于一个变量的增加或减少。
下面的代码中pNMUpDown->iDelta = pNMUpDown->iDelta;是其用方，它会得到-1或1以标志其按的是哪个键，要注意的是，它同样有上下或左右之分。
```
void CPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)// 已修改
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR); 
	pNMUpDown->iDelta = pNMUpDown->iDelta;			// Spin Contrl的用法（很简单吧~）
	m_spinval.Format("%d", pNMUpDown->iDelta);		// 转换数据格式，int -> CString
	SetDlgItemText(IDC_SPINVAL,m_spinval);			// 将数据显示在静态控件上

	*pResult = 0;
}
```
==9.日期控件(DataTimeControl)
这是一个时间控件，但它得到的并不是系统时间，而是它上面的时间，得到它的时间的方式如下：
`m_datetime.GetTime(&st);`
其中m\_datatime是一个时间控件的对象。
同样可以通过SetTime来设置控件上的时间。

可能你会问如何得到系统时间，那么我们来一个完整的例子吧：
```
void CPage1::OnBnClickedButtonGettime()// 已修改
{
	SYSTEMTIME st;
 	m_datetime.GetTime(&st);
 	CString strTime;

	CString strGmt = CTime::GetCurrentTime().FormatGmt("当前日期：%Y/%m/%d\n 当前时间：%H:%M:%S\n");   // 得到当前时间
	strTime.Format("从控件上得到的时间：%d年%d月%d日", st.wYear, st.wMonth, st.wDay);
 	MessageBox(strGmt + strTime, "日期", MB_OK);
}
```

## 10.IP控件(IP Control) ##
我们可以通过
`m_ipaddress.SetAddress(b1, b2, b3, b4);`
来设置该控件里显示的IP，同样我们也可以用GetAddress获取该控件里所填充的IP地址
下面是一个完整的例子以得到本机IP：
```
void CPage1::OnBnClickedButtonGetip()
{
	int b1, b2, b3, b4;								// IP的四段,想一想为什么是int型而不是BYTE型呢？
	CString strIP;
	WSADATA wsd;
	PHOSTENT hostinfo;

	if (WSAStartup(0x202, &wsd) !=  0) 
	{ 
		AfxMessageBox( "网络初始化失败 "); 
	} 
	else 
	{
		if (gethostname(strIP.GetBuffer(128), 128) == 0) 
		{ 
			//获取主机相关信息
			MessageBox(strIP, "第一步，得到您的机器名：", MB_OK);
			if ((hostinfo = gethostbyname(strIP))  !=  NULL) 
			{ 
				strIP = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);// 得到IP字符串 
			} 
			MessageBox(strIP, "第二步，得到您的IP：", MB_OK);
		}
	}
	WSACleanup(); 

	MessageBox("确定完成", "第三步，将您的IP字符串转成IP格式：", MB_OK);
	sscanf(strIP, "%d.%d.%d.%d", &b1, &b2, &b3, &b4); // 将字符串转成IP
	m_ipaddress.SetAddress(b1, b2, b3, b4);
}
```

## 11.滚动条(Scroll Control) ##
这是相对前面那些控件都要难以使用的一个控件，因为我们不仅要关心滑块所在的位置还要关心鼠标所点击的位置，而且它是有方向之分的。请看下面使用它的具体例子：
```
void CPage1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;

	if (pScrollBar != GetDlgItem(IDC_SCROLLBAR1))	  // 判断是不是本控件，如果不是，则直接返回
		return ;			          // $$$$这是必要的, 为防止其它控件产生WM_HSCROLL消息干扰

	pScrollBar->SetScrollRange(0, 100);			// 设置滚条范围
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();			// 得到实际滚动条范围

	// 得到滚动条的当前位置
	int curpos = pScrollBar->GetScrollPos();		// 得到当前滑块所在位置
	CWnd *HwndHstr = GetDlgItem(IDC_Hstr);			// 得到显示横坐标标签的句柄

	RECT rect;										  
	pScrollBar->GetWindowRect(&rect);		        // 得到本控件的区域范围
	int per = (rect.right - rect.left) / maxpos;	        // 得到它滚动一格的距离 $$：必要的
	HwndHstr->GetWindowRect(&rect); 		        // 得到显示横坐标标签的区域
	ScreenToClient(&rect);					// 计算相对坐标

	// 根据消息对滑块的位置以及标签的位置进行计算
	switch (nSBCode)
	{
		case SB_LEFT:		
			curpos = minpos;			// 到最左处
		break;

		case SB_RIGHT:      				// 到最右处
			curpos = maxpos;
		break;
								// 释放滑块
		case SB_ENDSCROLL:		
		break;

		case SB_LINELEFT:				// 点击左区域或左翻页时滑块及标签移动
		case SB_PAGELEFT:
			if (curpos > minpos)
			{
				curpos--;
				rect.left -= per;
				rect.right -= per;
			}
		break;

		case SB_LINERIGHT:   			         // 点击右区域或右翻页时滑块及标签移动
		case SB_PAGERIGHT:
			if (curpos < maxpos - 1)		 // 想一想为什么要减1？
			{
				curpos++;
				rect.left += per;
				rect.right += per;
			}
		break;

		case SB_THUMBPOSITION: 				//点击滑块进行拖曳时滑块及标签同时移动
		case SB_THUMBTRACK:   
			rect.left += (nPos - curpos) * per;
			rect.right += (nPos - curpos) * per;
			curpos = nPos;     
		break;
	}

	// 将滑块及标签移动新的位置
	pScrollBar->SetScrollPos(curpos); 
	HwndHstr->MoveWindow(&rect);
	
	CString strNum;
	strNum.Format("%d", curpos);
	SetDlgItemText(IDC_Hstr, strNum);			// 发给显示标签当前位置数值

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
```
上面的代码是横向的，还可以是纵向的，原理基本一样，这里就不再赘述了。

## 12.页面控件(Tab Page Control) ##
这个控件可以灵活切换窗口显示视图。但其实现也是相对较麻烦的，其初始化可以如下：
```
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("tab1")); 
	m_tab.InsertItem(1, _T("tab2")); 
	m_page1.Create(IDD_TAB_DLG1, &m_tab); 
	m_page2.Create(IDD_TAB_DLG2, &m_tab); 
	//设置画面窗口大小在控件内
	CRect rc; m_tab.GetClientRect(rc); 
	rc.top += 20; 
	rc.bottom -= 0; 
	rc.left += 0; 
	rc.right -= 0; 
	m_page1.MoveWindow(&rc); 
	m_page2.MoveWindow(&rc); 

	pDialog[0] = &m_page1; 
	pDialog[1] = &m_page2; 
	//显示初始画面
	pDialog[0]->ShowWindow(SW_SHOW); 
	pDialog[1]->ShowWindow(SW_HIDE);

	//保存当前点 选项
	m_CurSelTab = 0; 
	return true;
}
```
前面那些控件都可以放到上面。
其选择显示页的代码如下：
```
void CAboutDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
    //把当前的页面隐藏起来 
    pDialog[m_CurSelTab]->ShowWindow(SW_HIDE); 
    //得到新的页面索引 
    m_CurSelTab = m_tab.GetCurSel(); 
    //把新的页面显示出来 
    pDialog[m_CurSelTab]->ShowWindow(SW_SHOW); 
    *pResult = 0;
}
```
要注意的是，如果你在上面直接回车可能会导致其页面消失，解决方案是增加一个OnOK事件：
```
void CPage1::OnOK()
{
	return;
}
```

上面那些关联的控件所在的类定义如下：
```
class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_TAB_DLG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	CString m_spinval;
	CString m_comb_add;
	CString m_list_add;
	CComboBox m_comb;
	CListBox m_list;
	CListCtrl m_list_ctrl;
	CString m_list_ctrl_add;
	CProgressCtrl m_progress;
	CSliderCtrl m_slider;
	CSpinButtonCtrl m_spin;
	CDateTimeCtrl m_datetime;
	CIPAddressCtrl m_ipaddress;

	afx_msg void OnBnClickedButtonCombDel();
	afx_msg void OnBnClickedIdcButtonListAdd();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonCombAdd();
	afx_msg void OnBnClickedIdcButtonListCtrlAdd();
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedIdcButtonListCtrlDel();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnOK();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGettime();
	afx_msg void OnBnClickedButtonGetip();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
```

## 13.树形控件(TreeView Control) ##
这是一个好看但写起来却也很麻烦的一个控件，其初始化可以如下：
```
        m_iImageList.Create(24, 24, TRUE, 1, 0);
	m_FileTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT |  TVS_HASLINES | TVS_INFOTIP);  // 带加号、虚线

	HICON hIcon = NULL;
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(IDI_ICON_UBUNTU), IMAGE_ICON, 24, 24, 0);
	m_iImageList.Add(hIcon);
	m_FileTree.SetImageList ( &m_iImageList,TVSIL_NORMAL );
```