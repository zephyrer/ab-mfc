// ab_mfcView.h : Cab_mfcView 类的接口
//


#pragma once


class Cab_mfcView : public CView
{
protected: // 仅从序列化创建
	Cab_mfcView();
	DECLARE_DYNCREATE(Cab_mfcView)

// 属性
public:
	Cab_mfcDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cab_mfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ab_mfcView.cpp 中的调试版本
inline Cab_mfcDoc* Cab_mfcView::GetDocument() const
   { return reinterpret_cast<Cab_mfcDoc*>(m_pDocument); }
#endif

