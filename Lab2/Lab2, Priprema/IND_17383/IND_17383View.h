
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

// Operations
public:

// Overrides
public:
	bool isGridOn = false;
	int alfa = -90;
	int beta = 0;

	void Translate(CDC* pDC, float Dx, float Dy, bool rightMultiply);

	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawBranch(CDC* pDC, int width, int height, bool isLight);

	void DrawFigure(CDC* pDC);

	void DrawBackground(COLORREF backgroundColor, CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawText(CDC* pDC);
	void DrawPot(CDC* pDC);
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
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_17383View.cpp
inline CIND17383Doc* CIND17383View::GetDocument() const
   { return reinterpret_cast<CIND17383Doc*>(m_pDocument); }
#endif

