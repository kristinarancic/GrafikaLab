
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
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIND17383View construction/destruction

CIND17383View::CIND17383View() noexcept
{
	// TODO: add construction code here

	part = new DImage[9];

	part[0].Load(CString("res/1.dib"));
	part[1].Load(CString("res/2.dib"));
	part[2].Load(CString("res/3.dib"));
	part[3].Load(CString("res/4.dib"));
	part[4].Load(CString("res/5.dib"));
	part[5].Load(CString("res/6.dib"));
	part[6].Load(CString("res/7.dib"));
	part[7].Load(CString("res/8.dib"));
	part[8].Load(CString("res/9.dib"));

	
	angle = 0;
	rotation = 0;
	mx = 1;
	my = 1;
	
}

CIND17383View::~CIND17383View()
{
	delete[] part;
}

BOOL CIND17383View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND17383View drawing

void CIND17383View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform = { 1,0,0,1,dX,dY };
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform = { sX,0,0,sY,0,0 };
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xform = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0};
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17383View::Mirror(CDC* pDC, int mX, int mY, bool rightMultiply)
{
	XFORM xform = { mX,0,0,mY,0,0 };
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

CBitmap* CIND17383View::DrawTransparent(CDC* pDC, CBitmap* image, int width, int height)
{
	CBitmap* bmpMask = new CBitmap();
	bmpMask->CreateBitmap(width, height, 1, 1, NULL);
	CDC SrcDC, DstDC;
	SrcDC.CreateCompatibleDC(pDC);
	DstDC.CreateCompatibleDC(pDC);
	CBitmap* pOldSrcBmp = SrcDC.SelectObject(image);
	CBitmap* pOldDstBmp = DstDC.SelectObject(bmpMask);

	COLORREF clrTopLeft = SrcDC.GetPixel(0, 0);
	SrcDC.SetBkColor(clrTopLeft);
	DstDC.BitBlt(0, 0, width, height, &SrcDC, 0, 0, SRCCOPY);

	SrcDC.SetBkColor(RGB(0, 0, 0));//crna pozadina
	SrcDC.SetTextColor(RGB(255, 255, 255));//beli foreground

	SrcDC.BitBlt(0, 0, width, height, &DstDC, 0, 0, SRCAND);//u src je crna pozadiina postavljena, tj postala transparentna

	SrcDC.DeleteDC();
	DstDC.DeleteDC();

	return bmpMask;//vracamo masku da bi je koristili kasnije, a image je svakako vec promenjen zbog pokazivaca

}

CBitmap* CIND17383View::MakeImageGrayscale(CDC* pDC, int ind, bool blue)
{
	//ovde se menja u red, ne u blue

	BITMAP b;//sirina, visina i druge info za bitmapu, pokuplja ih
	CBitmap* bitmap = part[ind].GetBitmap();//pokuplja se bitmapa
	bitmap->GetBitmap(&b);//izvlaci se info o bitmapi i smesta se u b

	u_char* bytes = new u_char[b.bmWidthBytes * b.bmHeight];//vraca br bajtova u jednom redu, alociramo memoriju za niz
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bytes);
	//u bytes se smestaju bajtovi iz bitmap(to je slikica)

	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4)
	{
		//da preskocimo zelenu pozadinu na slikici
		
		if (bytes[i + 2] == 255 && bytes[i + 1] == 0 && bytes[i] == 255)
			continue;
		if (bytes[i + 2] == 0 && bytes[i + 1] == 255 && bytes[i] == 0)
			continue;
		if (bytes[i + 2] == 255 && bytes[i + 1] == 119 && bytes[i] == 0)
			continue;
		
		/*
		if (bytes[i + 2] == 255)
			continue;
		if (bytes[i + 1] == 255)
			continue;
		*/

		//racunamo srednju vrednost svih rgb komponenata, da bi dobili sivu boju koja odgovara tom pikselu
		u_char newPixel =40 + (bytes[i] + bytes[i+1] + bytes[i+2]) / 3;

		//smestamo srednju vrednost u odgovarajuce piksele
		//plava ostaje plava, ako treba da bude u blue grayscale
		bytes[i] = blue ? 0 : newPixel;
		bytes[i + 1] = blue ? 0 : newPixel;
		bytes[i + 2] = blue ? bytes[i + 2] : newPixel;
	}

	CBitmap* bitImg = new CBitmap();
	bitImg->CreateCompatibleBitmap(pDC, part[ind].Width(), part[ind].Height());
	bitImg->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bytes);

	delete[] bytes;
	return bitImg;
}

void CIND17383View::DrawPart(CDC* pDC, int ind, int dX, int dY, double angle, bool blue)
{
	int w = part[ind].Width();
	int h = part[ind].Height();

	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);

	CBitmap* bmpImage = MakeImageGrayscale(pDC, ind, blue);

	CBitmap* mask = DrawTransparent(pDC, bmpImage, w, h);

	Translate(pDC, dX, dY);
	Rotate(pDC, PI * angle / 180);

	
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(-w / 2, -h / 2, w, h, memDC, 0, 0, SRCAND);//pDC bela background, crna foreground
	//posle ovoga background iz mask postaje bela(1) i radi AND sa trenutnom pozadinom u pDC,
	//dakle pozadina je sad sta god je bilo pre toga jer 1 i bilo sta drugo, daju to drugo
	//ofreground iz mask postaje foreground iz pDC, tj crna(0) i radi se AND sa slagalicom
	//dakle 0 i bilo sta drugo ce dati 0, tj crnu boju
	//u ovom trenutku je bela pozadina(nebitno sad, ili bilo koja druga pozadina) i crna slagalica
	memDC->SelectObject(bmpImage);
	//bmpImage ima crnu pozadinu i normalnu slagalicu, i to je u memDC
	//pDC je trenutno bela pozadina i crna slagalica
	pDC->BitBlt(-w / 2, -h / 2, w, h, memDC, 0, 0, SRCPAINT);
	//za pozadinu: radi se OR, crna pozadina iz memDC(0) ILI bela pozadina iz pDC(1) daje belu pozadinu (ili bilo koju drugu zadatu pozadinu
	//za slagalicu: radi se OR, crna slagalica iz pDC(0) ILI normalna slagalica iz memDC daje normalnu slagalicu jer
	//0 ILI bilo sta drugo daje to drugo
	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;

	pDC->SetWorldTransform(&oldTransform);
	pDC->SetGraphicsMode(prevMode);

}

void CIND17383View::AntiFlicker(CDC* pDC, CRect rect)
{
	//pDC->SetBkColor(RGB(255, 255, 255));
	CBrush* bk = new CBrush(RGB(255, 255, 255));

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	pMemDC->BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC->SelectObject(&bmp);
	pMemDC->FillRect(rect, bk);
	DrawGrid(pMemDC);

	int i = 0;

	DrawPart(pMemDC, i, 4.2 * 25, 3.8 * 25, (double)32 + angle);//prvi red, prvi
	i++;
	DrawPart(pMemDC, i, 9.8 * 25, 4.35 * 25, (double)182 + angle);//prvi red, srednji
	i++;
	DrawPart(pMemDC, i, 15.75 * 25, 4.3 * 25, (double)-156 + angle);//prvi red, treci
	i++;
	DrawPart(pMemDC, i, 4 * 25, 10.3 * 25, (double)148 + angle);
	i++;
	DrawPart(pMemDC, i, 10.3 * 25, 9.9 * 25, (double)54 + angle/*, true*/);
	i++;
	DrawPart(pMemDC, i, 15.95 * 25, 10.35 * 25, (double)161 + angle);
	i++;
	DrawPart(pMemDC, i, 4.3 * 25, 16 * 25, (double)76 + angle);
	i++;
	DrawPart(pMemDC, i, 10.2 * 25, 16.2 * 25, (double)115 + angle);
	i++;
	DrawPart(pMemDC, i, 16.1 * 25, 15.7 * 25, (double)4 + angle);
	i++;

	int orgmode = pDC->SetGraphicsMode(GM_ADVANCED);
	Translate(pDC, 250, 250);
	Rotate(pDC, PI / 2);
	Rotate(pDC, rotation * PI / 2);
	Mirror(pDC, mx, my);
	Translate(pDC, -250, -250);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);

	pDC->SetGraphicsMode(orgmode);

	pMemDC->DeleteDC();
	delete pMemDC;
}


void CIND17383View::DrawGrid(CDC* pDC)
{

	CPen* pen = new CPen(PS_SOLID, 1, RGB(121, 121, 121));
	CPen* old = pDC->SelectObject(pen);

	for (int i = 0; i <= 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(old);
	delete pen;
}

void CIND17383View::OnDraw(CDC* pDC)
{
	CIND17383Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect(0, 0, 500, 500);
	AntiFlicker(pDC, rect);
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


void CIND17383View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'A') {

		angle += 10;
		Invalidate();
	}
	else if (nChar == 'D') {
		angle -= 10;
		Invalidate();
	}
	else if (nChar == 'Q') {
		mx = (-1) * mx;
		Invalidate();
	}
	else if (nChar == 'E') {
		my = (-1) * my;
		Invalidate();
	}
	else if (nChar == 'R') {
		rotation++;
		//rotation = (rotation + 1) % 4;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CIND17383View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
