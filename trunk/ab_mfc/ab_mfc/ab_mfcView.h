// ab_mfcView.h : Cab_mfcView ��Ľӿ�
//


#pragma once


class Cab_mfcView : public CView
{
protected: // �������л�����
	Cab_mfcView();
	DECLARE_DYNCREATE(Cab_mfcView)

// ����
public:
	Cab_mfcDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cab_mfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ab_mfcView.cpp �еĵ��԰汾
inline Cab_mfcDoc* Cab_mfcView::GetDocument() const
   { return reinterpret_cast<Cab_mfcDoc*>(m_pDocument); }
#endif

