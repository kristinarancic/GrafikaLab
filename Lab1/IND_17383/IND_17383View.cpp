
// IND_17383View.cpp : implementation of the CIND17383View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17383.h"
#endif

#include "IND_17383Doc.h"
#include "IND_17383View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND17383View

IMPLEMENT_DYNCREATE(CIND17383View, CView)

BEGIN_MESSAGE_MAP(CIND17383View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CIND17383View construction/destruction

CIND17383View::CIND17383View() noexcept
{
	// TODO: add construction code here

}

CIND17383View::~CIND17383View()
{
}

BOOL CIND17383View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND17383View drawing

void CIND17383View::DrawBackground(COLORREF backgroundColor, CDC* pDC)
{
	CBrush brush(backgroundColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->FillRect(CRect(0, 0, 500, 500), &brush);
	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
}

void CIND17383View::DrawGrid(CDC* pDC)
{
	if (!this->isGridOn)
	{
		return;
	}

	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&pen);

	for (int y = 25; y < 500; y += 25)
	{
		pDC->MoveTo(0, y);
		pDC->LineTo(500, y);
	}

	for (int x = 25; x < 500; x += 25)
	{
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, 500);
	}

	pDC->SelectObject(oldPen);

	pen.DeleteObject();
}

void CIND17383View::DrawTrianglePattern(CPoint p1, CPoint p2, CPoint p3, COLORREF color, int pattern, COLORREF patternColor, CDC* pDC)
{
	COLORREF bgColor = pDC->GetBkColor();
	pDC->SetBkColor(color);

	CBrush brush(pattern, patternColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	CPoint pts[3] = { p1, p2, p3 };

	pDC->Polygon(pts, 3);

	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
}

void CIND17383View::DrawTriangle(CPoint p1, CPoint p2, CPoint p3, COLORREF color, CDC* pDC)
{
	DrawTrianglePattern(p1, p2, p3, color, 0, color, pDC);
}

void CIND17383View::DrawQuatro(CPoint p1, CPoint p2, CPoint p3, CPoint p4, COLORREF color, CDC* pDC)
{
	CBrush brush(color);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	CPoint pts[4] = { p1, p2, p3, p4 };
	pDC->Polygon(pts, 4);

	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
}

void CIND17383View::DrawRegularPolygon(CDC* pDC, double cx, double cy, double r, int n, float rotAngle)
{
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	CPoint* pts = new CPoint[n];
	double angle = rotAngle * 3.1415926535 / 180;//prosledjeni ugao u radijanima

	for (int i = 0; i < n; i++)
	{
		pts[i].x = r * cos(angle) + cx;
		pts[i].y = r * sin(angle) + cy;

		angle += 2 * 3.1415926535 / n;// pomeraj ugla u svakoj iteraciji, jednaki su uglovi jer je prailni mnogougao

	}

	pDC->Polygon(pts, n);

	pDC->SelectObject(oldBrush);
}

void CIND17383View::OnDraw(CDC* pDC)
{
	CIND17383Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	

	//////////////Moja priprema

	/*
	//DrawTrianglePattern(CPoint(25, 25), CPoint(150, 25), CPoint(150, 150), RGB(255, 255, 255), HS_FDIAGONAL, RGB(0, 255, 0), pDC);
	//Zuti trougao
	DrawTriangle(CPoint(25, 25), CPoint(475, 25), CPoint(250, 250), RGB(255, 255, 0), pDC);
	//Crveni trougao
	DrawTriangle(CPoint(25, 25), CPoint(5.5*25, 5.5 * 25), CPoint(25, 10*25), RGB(255, 0, 0), pDC);
	//Roze trougao
	DrawTriangle(CPoint(10*25, 10*25), CPoint(19 * 25, 25), CPoint(19*25, 19 * 25), RGB(255, 153, 204), pDC);
	//Zeleni trougao
	DrawTriangle(CPoint(25, 10*25), CPoint(10 * 25, 19 * 25), CPoint(25, 19 * 25), RGB(50, 205, 50), pDC);
	//Ljubicasti kvadrat
	DrawQuatro(CPoint(5.5 * 25, 5.5 * 25), CPoint(10 * 25, 10 * 25), CPoint(5.5 * 25, 14.5 * 25), CPoint(25, 10 * 25), RGB(153, 0, 204), pDC);
	//Narandzasti paralelogram
	DrawQuatro(CPoint(5.5 * 25, 14.5 * 25), CPoint(14.5 * 25, 14.5 * 25), CPoint(19 * 25, 19 * 25), CPoint(10 * 25, 19 * 25), RGB(255, 153, 51), pDC);
	//Trougao sa srafurom
	DrawTrianglePattern(CPoint(10 * 25, 10 * 25), CPoint(14.5 * 25, 14.5 * 25), CPoint(5.5 * 25, 14.5 * 25), RGB(255, 255, 255), HS_BDIAGONAL, RGB(0, 0, 255), pDC);

	CPen pen2(PS_SOLID, 3, RGB(0, 255, 0));
	pDC->SelectObject(&pen2);

	//u zuti trougao
	DrawRegularPolygon(pDC, 10 * 25, 5 * 25, 2 * 25, 5, 0);
	//u crveni trougao
	DrawRegularPolygon(pDC, 2.75 * 25, 5.5 * 25, 25, 7, 0);
	//u roze trougao
	DrawRegularPolygon(pDC, 15.2 * 25, 10 * 25, 2 * 25, 4, 0);
	//u zeleni trougao
	DrawRegularPolygon(pDC, 3.6 * 25, 16.4 * 25, 1.5 * 25, 6, 0);
	//u srafirani trougao
	DrawRegularPolygon(pDC, 10 * 25, 12.5 * 25, 25, 8, 0);
	*/

	this->DrawBackground(RGB(230, 230, 230), pDC);

	CPen pen(PS_SOLID, 6, RGB(0, 0, 255));
	CPen* oldPen = pDC->SelectObject(&pen);

	//roze trougao
	DrawTriangle(CPoint(14.5 * 25, 25), CPoint(14.5 * 25, 7 * 25), CPoint(8.5 * 25, 7 * 25), RGB(255, 153, 204), pDC);
	//zeleni kvadrat
	DrawQuatro(CPoint(8.5 * 25, 7 * 25), CPoint(11.5 * 25, 7 * 25), CPoint(11.5 * 25, 10 * 25), CPoint(8.5 * 25, 10 * 25), RGB(50, 205, 50), pDC);
	//narandzasti trougao
	DrawTriangle(CPoint(8.5 * 25, 7 * 25), CPoint(8.5 * 25, 10 * 25), CPoint(5.5 * 25, 10 * 25), RGB(255, 153, 51), pDC);
	//zuti paralelogram
	DrawQuatro(CPoint(5.5 * 25, 10 * 25), CPoint(8.5 * 25, 10 * 25), CPoint(11.5 * 25, 13 * 25), CPoint(8.5 * 25, 13 * 25), RGB(255, 255, 0), pDC);
	//crveni trougao
	DrawTriangle(CPoint(8.5 * 25, 10 * 25), CPoint(11.5 * 25, 10 * 25), CPoint(11.5 * 25, 13 * 25), RGB(255, 0, 0), pDC);
	//srafirani trougao
	DrawTrianglePattern(CPoint(11.5 * 25, 10 * 25), CPoint(14.5 * 25, 7 * 25), CPoint(14.5 * 25, 13 * 25), RGB(255, 255, 255), HS_VERTICAL, RGB(0, 0, 255), pDC);
	//ljubicasti trougao
	DrawTriangle(CPoint(8.5 * 25, 13 * 25), CPoint(14.5 * 25, 13 * 25), CPoint(14.5 * 25, 19 * 25), RGB(153, 0, 204), pDC);

	

	CPen pen2(PS_SOLID, 3, RGB(0, 0, 255));
	pDC->SelectObject(&pen2);

	//u roze trougao
	DrawRegularPolygon(pDC, 12.8 * 25, 5.2 * 25, 25, 5, 0);
	//u narandzasti trougao
	DrawRegularPolygon(pDC, 7.5 * 25, 9 * 25, 0.5 * 25, 7, 0);
	//u crveni trougao
	DrawRegularPolygon(pDC, 10.5 * 25, 10.9 * 25, 0.4 * 25, 8, 0);
	//u srafirani trougao
	DrawRegularPolygon(pDC, 13.2 * 25, 10 * 25, 0.8 * 25, 6, 0);
	//u ljubicasti trougao
	DrawRegularPolygon(pDC, 12.8 * 25, 14.7 * 25, 25, 4, 0);

	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	this->DrawGrid(pDC);
}


// CIND17383View printing

BOOL CIND17383View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND17383View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND17383View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND17383View diagnostics

#ifdef _DEBUG
void CIND17383View::AssertValid() const
{
	CView::AssertValid();
}

void CIND17383View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND17383Doc* CIND17383View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND17383Doc)));
	return (CIND17383Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND17383View message handlers


void CIND17383View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar >= 'A' && nChar <= 'Z') {
		this->isGridOn = !this->isGridOn;
		Invalidate();
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
