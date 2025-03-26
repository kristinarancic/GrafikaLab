
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

void CIND17383View::Translate(CDC* pDC, float Dx, float Dy, bool rightMultiply)
{
	XFORM xform = { 1, 0, 0, 1, Dx, Dy };
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xform = { cos(angle), sin(angle), -sin(angle), cos(angle)};
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform = { sX, 0, 0, sY, 0, 0 };
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::DrawBranch(CDC* pDC, int width, int height, bool isLight)
{
	CBrush brush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	
	CRect fajl(-width / 2, -height, width / 2, 0);

	HENHMETAFILE mf = isLight ? GetEnhMetaFile(L"Resursi/cactus_part_light.emf") : GetEnhMetaFile(L"RESURSI/cactus_part.emf");
	pDC->PlayMetaFile(mf, fajl);
	

	CRect krug(-0.4 * 25, -0.4 * 25, 0.4 * 25, 0.4 * 25);
	pDC->Ellipse(krug);

	pDC->SelectObject(oldBrush);
	DeleteObject(brush);
}

void CIND17383View::DrawFigure(CDC* pDC)
{
	XFORM oldForm, globalForm, saveForm, saveForm1, saveForm2;
	pDC->GetWorldTransform(&oldForm);

	
	Translate(pDC, 0, 0, false);
	Rotate(pDC, 3.14 / 2, false);
	Translate(pDC, 0 * 25, -20 * 25, false);
	
	
	pDC->GetWorldTransform(&globalForm);


	DrawPot(pDC);


	Translate(pDC, 10 * 25, 17 * 25, false);
	//pDC->GetWorldTransform(&globalForm);//mislim da ne treba

	DrawBranch(pDC, 2 * 25, 3 * 25, false);//prva grana

	//kad hocemo da se izvrsi rotacija zajedno sa svi nadovezanim objektima
	//ne idemo na get i set world jer necemo da se vratimo na prethodnu poziciju
	//vec samo da nastavimo uz ovu trenutnu

	Translate(pDC, 0, -3 * 25, false);
	DrawBranch(pDC, 1 * 25, 3 * 25, false);//druga dark u sredinu grana, izduzena

	pDC->GetWorldTransform(&saveForm);//sacuvana pozicija od srednju izduzenu granu

	Rotate(pDC, -3.14 / 4, false);
	DrawBranch(pDC, 1 * 25, 3 * 25, false);//ulevo izduzena grana

	pDC->SetWorldTransform(&saveForm);//vracam se na srednju izduzenu granu

	//Rotate(pDC, 3.14 / 4, false);
	Rotate(pDC, 3.14 * beta / 180, false);
	DrawBranch(pDC, 1 * 25, 3 * 25, true);//udesno izduzena grana, LIGHT

	Rotate(pDC, -3.14 / 4, false);
	pDC->GetWorldTransform(&globalForm);// cuvam poziciju za LIGHT udesno granu, bez rotacije

	pDC->SetWorldTransform(&saveForm); //vracam se na srednju izduzenu granu

	Translate(pDC, 0, -3 * 25, false);
	DrawBranch(pDC, 2 * 25, 3 * 25, false);//treca uspravna nagore

	Translate(pDC, 0, -3 * 25, false);
	DrawBranch(pDC, 2 * 25, 3 * 25, false);//cetvrta uspravna nagore

	pDC->SetWorldTransform(&globalForm); //vracam se na srednju izduzenu granu, sa rotacijom
	Translate(pDC, 2.1 * 25, -2.1 * 25, false);
	DrawBranch(pDC, 1.5 * 25, 3 * 25, false);//na desnu stranu, prva uspravna
	pDC->GetWorldTransform(&globalForm);//cuva se pozicija desna strana, prva uspravno

	Translate(pDC, 0, -3 * 25, false);
	DrawBranch(pDC, 2 * 25, 3 * 25, false);//na desnu stranu, druga uspravna

	pDC->SetWorldTransform(&globalForm);//vracam poziciju desna strana, prva uspravno

	Rotate(pDC, 3.14 / 2, false);
	DrawBranch(pDC, 2 * 25, 3 * 25, false);//na desnu stranu, polozena udesno

	Translate(pDC, 0, -3 * 25, false);
	Rotate(pDC, 3.14 / 4, false);
	DrawBranch(pDC, 2 * 25, 3 * 25, false);//na desnu stranu, blago nadole

	//Rotate(pDC, -3.14 / 2, false);
	Rotate(pDC, 3.14 * alfa / 180, false);//ROTACIJA na klik
	DrawBranch(pDC, 2 * 25, 3 * 25, true);//na desnu stranu, blago nagore

	/*
	pDC->GetWorldTransform(&saveForm1);//sacuvana pozicija od levu stranu, polozena ulevo grana, mislim da ne treba ni ovo
	Rotate(pDC, 3.14 / 2, false);
	DrawBranch(pDC, 1.5 * 25, 3 * 25, false);//na levu stranu uspravna

	Translate(pDC, 0, -3 * 25, false);
	DrawBranch(pDC, 2.3 * 25, 3 * 25, false);//zadnja na levu stranu, uspravna

	pDC->SetWorldTransform(&saveForm);//vracam se na srednju izduzenu granu
	Translate(pDC, 2.1 * 25, -2.1 * 25, false);
	Rotate(pDC, 3.14 / 2, false);
	DrawBranch(pDC, 1.5 * 25, 3 * 25, false);//na desnu stranu, polozena, udesno

	pDC->GetWorldTransform(&saveForm2);//sacuvana pozicija od desnu stranu, polozena udesno grana

	Rotate(pDC, 3.14 * alfa / 180, false);//ovde je manja ROTACIJA
	DrawBranch(pDC, 1.5 * 25, 3 * 25, true);//na desnu stranu, uspravna LIGHT

	//kad hocemo da se rotira samo jedna figura,
	//moramo da prvo sacuvamo prethodno stanje na get world,
	//pa onda da ga posle rotirajuce figure vratimo sa set world

	pDC->SetWorldTransform(&saveForm2);//vracam se na desnu stranu, polozena udesno grana

	Translate(pDC, 0, -3 * 25, false);
	Rotate(pDC, -3.14 / 4, false);
	DrawBranch(pDC, 1.5 * 25, 3 * 25, false);//skroz desno, blago nagore

	Rotate(pDC, 3.14 / 2, false);
	DrawBranch(pDC, 1.5 * 25, 3 * 25, false);//skroz desno, blago nadole
	*/

	pDC->SetWorldTransform(&oldForm);

	//DrawPot(pDC);
}

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

void CIND17383View::DrawText(CDC* pDC)
{
	COLORREF clrOld = pDC->SetTextColor(RGB(252, 252, 0));
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	CString szMsg = (CString)"17383 Kristina Rancic";
	CFont font;
	CString sFaceName = (CString)"Times New Roman";

	font.CreateFont(30, 15, -900, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, sFaceName);

	CFont* old = pDC->SelectObject(&font);

	pDC->TextOutW(19 * 25, 25, szMsg);
	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nOldMode);

	pDC->SelectObject(old);

	font.DeleteObject();
}

void CIND17383View::DrawPot(CDC* pDC)
{
	CBrush brush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(7.5 * 25, 17.2 * 25, 12.5 * 25, 18 * 25);

	CPoint pts[4] = { CPoint(8 * 25, 18 * 25), CPoint(12 * 25, 18 * 25), CPoint(11.7 * 25, 19.99 * 25), CPoint(8.3 * 25, 19.99 * 25) };

	pDC->Polygon(pts, 4);

	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
}

void CIND17383View::OnDraw(CDC* pDC)
{
	CIND17383Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	int mode = pDC->SetGraphicsMode(GM_ADVANCED);

	this->DrawBackground(RGB(135, 206, 235), pDC);

	//Translate(pDC, -10 * 25, -10 * 25, false);
	//Rotate(pDC, 3.14 / 2, false);

	DrawFigure(pDC);

	this->DrawGrid(pDC);

	pDC->SetGraphicsMode(mode);

	this->DrawText(pDC);
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
	if (nChar == 'G') {
		this->isGridOn = !this->isGridOn;
		//Invalidate();
	}
	else if (nChar == 'A')
		alfa -= 3;
	else if (nChar == 'D')
		alfa += 3;
	else if (nChar == 'W')
		beta -= 3;
	else if (nChar == 'S')
		beta += 3;

	Invalidate();

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
