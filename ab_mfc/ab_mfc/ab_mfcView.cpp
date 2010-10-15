// ab_mfcView.cpp : Cab_mfcView ���ʵ��
//

#include "stdafx.h"
#include "ab_mfc.h"

#include "ab_mfcDoc.h"
#include "ab_mfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cab_mfcView

IMPLEMENT_DYNCREATE(Cab_mfcView, CView)

BEGIN_MESSAGE_MAP(Cab_mfcView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_CONSOLE, &Cab_mfcView::OnConsole)
END_MESSAGE_MAP()

// Cab_mfcView ����/����

Cab_mfcView::Cab_mfcView()
{
	// TODO: �ڴ˴���ӹ������
	isConsole = FALSE;
}

Cab_mfcView::~Cab_mfcView()
{
}

BOOL Cab_mfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cab_mfcView ����

void Cab_mfcView::OnDraw(CDC* /*pDC*/)
{
	Cab_mfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cab_mfcView ��ӡ

BOOL Cab_mfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cab_mfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cab_mfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// Cab_mfcView ���

#ifdef _DEBUG
void Cab_mfcView::AssertValid() const
{
	CView::AssertValid();
}

void Cab_mfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cab_mfcDoc* Cab_mfcView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cab_mfcDoc)));
	return (Cab_mfcDoc*)m_pDocument;
}
#endif //_DEBUG


#include <io.h>
#include <cstdio>
#include <FCNTL.H>

void Cab_mfcView::OnConsole()
{
	if(!isConsole)
	{
		AllocConsole();
		freopen( "CONOUT$", "w+t", stdout );// ����д
		freopen( "CONIN$", "r+t", stdin );// �����
	}
	else
	{
		FreeConsole();
	}
	isConsole = !isConsole;
}