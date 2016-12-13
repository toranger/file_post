
// TestPlatView.cpp : CTestPlatView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TestPlat.h"
#endif

#include "TestPlatDoc.h"
#include "TestPlatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestPlatView

IMPLEMENT_DYNCREATE(CTestPlatView, CView)

BEGIN_MESSAGE_MAP(CTestPlatView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestPlatView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTestPlatView 构造/析构

CTestPlatView::CTestPlatView()
{
	// TODO: 在此处添加构造代码

}

CTestPlatView::~CTestPlatView()
{
}

BOOL CTestPlatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestPlatView 绘制

void CTestPlatView::OnDraw(CDC* /*pDC*/)
{
	CTestPlatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTestPlatView 打印


void CTestPlatView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestPlatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestPlatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestPlatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTestPlatView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestPlatView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestPlatView 诊断

#ifdef _DEBUG
void CTestPlatView::AssertValid() const
{
	CView::AssertValid();
}

void CTestPlatView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestPlatDoc* CTestPlatView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestPlatDoc)));
	return (CTestPlatDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestPlatView 消息处理程序
