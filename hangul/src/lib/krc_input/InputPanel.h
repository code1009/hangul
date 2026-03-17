///////////////////////////////////////////////////////////////////////////////
//
// InputPanel.h
//
// Created by MOON, Eui-kwon.
// Created on Apr-04th, 2007.
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(__INPUTPANEL__H__)
#define __INPUTPANEL__H__

#pragma once

class CInputPanel : public CWnd
{
public:
	CInputPanel();
	virtual ~CInputPanel();

public:
	typedef struct tagKEYITEM 
	{
		RECT    rc;
		LPCTSTR text;
		WCHAR   code;
	} KEYITEM;

protected:
	int      m_nCurrentKey;
	int      m_nActiveKey;
	BOOL     m_bTracking;

	int      m_nKeyMapType;				// 어떤 KeyMap을 사용하는냐? 0:한글, 1:영어, 2:숫자
	KEYITEM *m_lpKeyMap;
	int      m_nKeyMapCount;

	BOOL     m_bHangulUnion;
	int      m_nCursor;

	ULONG    m_uMaxLength;
	WCHAR    m_szText[1024];
	CBitmap		m_bmBG;
	CBitmap		m_bmKey;
	CBitmap		m_bmSelect;

public:
	void SetKeyMap    (int nKeyMap);
	void SetMaxLength (ULONG length);
	void GetInputText (LPTSTR text, ULONG size);

	BOOL HitTest      (CPoint pt, int &index, WCHAR &code);

public:
	void Render (CDC *pDC);

public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPanel)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputPanel)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__INPUTPANEL__H__)
