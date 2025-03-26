#include "DImage.h"
#define PI 3.14159265
// IND_17383View.h : interface of the CIND17383View class
//

#pragma once


class CIND17383View : public CView
{
protected: // create from serialization only
	CIND17383View() noexcept;
	DECLARE_DYNCREATE(CIND17383View)

// Attributes
public:
	CIND17383Doc* GetDocument() const;

protected:
	DImage* part;
	int angle = 0;
	int rotation;
	int mx;
	int my;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, int mX, int mY, bool rightMultiply = false);
	//void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);
	CBitmap* DrawTransparent(CDC* pDC, CBitmap* image, int width, int height);
	CBitmap* MakeImageGrayscale(CDC* pDC, int ind, bool blue);
	void DrawPart(CDC* pDC, int ind, int dX, int dY, double angle, bool blue = false);
	void AntiFlicker(CDC* pDC, CRect rect);
	void DrawGrid(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND17383View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in IND_17383View.cpp
inline CIND17383Doc* CIND17383View::GetDocument() const
   { return reinterpret_cast<CIND17383Doc*>(m_pDocument); }
#endif

