///////////////////////////////////////////////////////////////////////////////
//
// InputPanel.h
//
// Created by MOON, Eui-kwon.
// Created on Apr-04th, 2007.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InputPanel.h"
#include "InputEngine.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// Tables
///////////////////////////////////////////////////////////////////////////////

static POINT                g_keymap_start        = { 10, 50 };

static int                  g_hangul_keymap_count = 40;
static CInputPanel::KEYITEM g_hangul_keymap [40]  = 
{
	{ { 29,208, 29+52,208+22}, _T("한글"),  0x0000 },
	{ { 85,208, 85+52,208+22}, _T("영대"),  0x0000 },
	{ {140,208,140+52,208+22}, _T("영소"),  0x0000 },
	{ {196,208,196+52,208+22}, _T("숫/특"), 0x0000 }, 

	{ {387, 77,387+48, 77+33}, _T("<-"),    0x0008 }, // back space
	{ {387,106,387+48,106+33}, _T("space"), 0x0020 }, // space
	{ {387,135,387+48,135+33}, _T("enter"), 0x0020 }, // enter

	{ {  6, 77,  6+48, 77+33}, _T("ㄱ"), 0x3131 }, // 0
	{ { 49, 77, 49+48, 77+33}, _T("ㄲ"), 0x3132 }, // 1
	{ { 91, 77, 91+48, 77+33}, _T("ㄴ"), 0x3134 }, // 2
	{ {133, 77,133+48, 77+33}, _T("ㄷ"), 0x3137 }, // 3
	{ {176, 77,176+48, 77+33}, _T("ㄸ"), 0x3138 }, // 4
	
	{ {  6,106,  6+48,106+33}, _T("ㄹ"), 0x3139 }, // 5
	{ { 49,106, 49+48,106+33}, _T("ㅁ"), 0x3141 }, // 6
	{ { 91,106, 91+48,106+33}, _T("ㅂ"), 0x3142 }, // 7
	{ {133,106,133+48,106+33}, _T("ㅃ"), 0x3143 }, // 8
	{ {176,106,176+48,106+33}, _T("ㅅ"), 0x3145 }, // 9

	{ {  6,135,  6+48,135+33}, _T("ㅆ"), 0x3146 }, // 10
	{ { 49,135, 49+48,135+33}, _T("ㅇ"), 0x3147 }, // 11
	{ { 91,135, 91+48,135+33}, _T("ㅈ"), 0x3148 }, // 12
	{ {133,135,133+48,135+33}, _T("ㅉ"), 0x3149 }, // 13
	{ {176,135,176+48,135+33}, _T("ㅊ"), 0x314a }, // 14
	
	{ {  6,164,  6+48,164+33}, _T("ㅋ"), 0x314b }, // 15
	{ { 49,164, 49+48,164+33}, _T("ㅌ"), 0x314c }, // 16
	{ { 91,164, 91+48,164+33}, _T("ㅍ"), 0x314d }, // 17
	{ {133,164,133+48,164+33}, _T("ㅎ"), 0x314e }, // 18

	{ {218, 77,218+48, 77+33}, _T("ㅏ"), 0x314f }, // 19
	{ {260, 77,260+48, 77+33}, _T("ㅑ"), 0x3151 }, // 20
	{ {303, 77,303+48, 77+33}, _T("ㅓ"), 0x3153 }, // 21
	{ {345, 77,345+48, 77+33}, _T("ㅕ"), 0x3155 }, // 22

	{ {218,106,218+48,106+33}, _T("ㅗ"), 0x3157 }, // 23
	{ {260,106,260+48,106+33}, _T("ㅛ"), 0x315b }, // 24
	{ {303,106,303+48,106+33}, _T("ㅜ"), 0x315c }, // 25
	{ {345,106,345+48,106+33}, _T("ㅠ"), 0x3160 }, // 26

	{ {218,135,218+48,135+33}, _T("ㅐ"), 0x3150 }, // 27
	{ {260,135,260+48,135+33}, _T("ㅒ"), 0x3152 }, // 28
	{ {303,135,303+48,135+33}, _T("ㅔ"), 0x3154 }, // 29
	{ {345,135,345+48,135+33}, _T("ㅖ"), 0x3156 }, // 30

	{ {218,164,218+48,164+33}, _T("ㅡ"), 0x3161 }, // 31
	{ {260,164,260+48,164+33}, _T("ㅣ"), 0x3163 } // 32
};


static int                  g_english_keymap_count = 33;
static CInputPanel::KEYITEM g_english_keymap [33]  = 
{
	{ { 29,208, 29+52,208+22}, _T("한글"),  0x0000 },
	{ { 85,208, 85+52,208+22}, _T("영대"),  0x0000 },
	{ {140,208,140+52,208+22}, _T("영소"),  0x0000 },
	{ {196,208,196+52,208+22}, _T("숫/특"), 0x0000 }, 

	{ {387, 77,387+48, 77+33}, _T("<-"),    0x0008 }, // back space
	{ {387,106,387+48,106+33}, _T("space"), 0x0020 }, // space
	{ {387,135,387+48,135+33}, _T("enter"), 0x0020 }, // enter

	{ {  6, 77,  6+48, 77+33}, _T("A"), 'A' }, // 0
	{ { 49, 77, 49+48, 77+33}, _T("B"), 'B' }, // 1
	{ { 91, 77, 91+48, 77+33}, _T("C"), 'C' }, // 2
	{ {133, 77,133+48, 77+33}, _T("D"), 'D' }, // 3
	{ {176, 77,176+48, 77+33}, _T("E"), 'E' }, // 4
	{ {218, 77,218+48, 77+33}, _T("F"), 'F' }, // 5
	{ {260, 77,260+48, 77+33}, _T("G"), 'G' }, // 6
	{ {303, 77,303+48, 77+33}, _T("H"), 'H' }, // 7
	{ {345, 77,345+48, 77+33}, _T("I"), 'I' }, // 8

	{ {  6,106,  6+48,106+33}, _T("J"), 'J' }, // 9
	{ { 49,106, 49+48,106+33}, _T("K"), 'K' }, // 10
	{ { 91,106, 91+48,106+33}, _T("L"), 'L' }, // 11
	{ {133,106,133+48,106+33}, _T("M"), 'M' }, // 12
	{ {176,106,176+48,106+33}, _T("N"), 'N' }, // 13
	{ {218,106,218+48,106+33}, _T("O"), 'O' }, // 14
	{ {260,106,260+48,106+33}, _T("P"), 'P' }, // 15
	{ {303,106,303+48,106+33}, _T("Q"), 'Q' }, // 16
	{ {345,106,345+48,106+33}, _T("R"), 'R' }, // 17

	{ {  6,135,  6+48,135+33}, _T("S"), 'S' }, // 18
	{ { 49,135, 49+48,135+33}, _T("T"), 'T' }, // 19
	{ { 91,135, 91+48,135+33}, _T("U"), 'U' }, // 20
	{ {133,135,133+48,135+33}, _T("V"), 'V' }, // 21
	{ {176,135,176+48,135+33}, _T("W"), 'W' }, // 22
	{ {218,135,218+48,135+33}, _T("X"), 'X' }, // 23
	{ {260,135,260+48,135+33}, _T("Y"), 'Y' }, // 24
	{ {303,135,303+48,135+33}, _T("Z"), 'Z' }, // 25
};


static int                  g_number_keymap_count = 43;
static CInputPanel::KEYITEM g_number_keymap [43]  = 
{
	{ { 29,208, 29+52,208+22}, _T("한글"),  0x0000 },
	{ { 85,208, 85+52,208+22}, _T("영대"),  0x0000 },
	{ {140,208,140+52,208+22}, _T("영소"),  0x0000 },
	{ {196,208,196+52,208+22}, _T("숫/특"), 0x0000 }, 

	{ {387, 77,387+48, 77+33}, _T("<-"),    0x0008 }, // back space
	{ {387,106,387+48,106+33}, _T("space"), 0x0020 }, // space
	{ {387,135,387+48,135+33}, _T("enter"), 0x0020 }, // enter

	{ {260,135,260+48,135+33}, _T("1"), '1' }, // 0
	{ {303,135,303+48,135+33}, _T("2"), '2' }, // 1
	{ {345,135,345+48,135+33}, _T("3"), '3' }, // 2
	{ {260,106,260+48,106+33}, _T("4"), '4' }, // 3
	{ {303,106,303+48,106+33}, _T("5"), '5' }, // 4
	{ {345,106,345+48,106+33}, _T("6"), '6' }, // 5
	{ {260, 77,260+48, 77+33}, _T("7"), '7' }, // 6
	{ {303, 77,303+48, 77+33}, _T("8"), '8' }, // 7
	{ {345, 77,345+48, 77+33}, _T("9"), '9' }, // 8
	{ {260,164,260+48,164+33}, _T("#"), '#' }, // 9
	{ {303,164,303+48,164+33}, _T("0"), '0' }, // 10
	{ {345,164,345+48,164+33}, _T("*"), '*' }, // 11

	{ {  6, 77,  6+48, 77+33}, _T("("), '(' }, // 12
	{ { 49, 77, 49+48, 77+33}, _T(")"), ')' }, // 13
	{ { 91, 77, 91+48, 77+33}, _T("["), '[' }, // 14
	{ {133, 77,133+48, 77+33}, _T("]"), ']' }, // 15
	{ {176, 77,176+48, 77+33}, _T("{"), '{' }, // 16
	{ {218, 77,218+48, 77+33}, _T("}"), '}' }, // 17

	{ {  6,106,  6+48,106+33}, _T("<"), '<' }, // 18
	{ { 49,106, 49+48,106+33}, _T(">"), '>' }, // 19
	{ { 91,106, 91+48,106+33}, _T(":"), ':' }, // 20
	{ {133,106,133+48,106+33}, _T(";"), ';' }, // 21
	{ {176,106,176+48,106+33}, _T(","), ',' }, // 22
	{ {218,106,218+48,106+33}, _T("."), '.' }, // 23

	{ {  6,135,  6+48,135+33}, _T("\'"), '\'' }, // 24
	{ { 49,135, 49+48,135+33}, _T("\""), '\"' }, // 25
	{ { 91,135, 91+48,135+33}, _T("+"), '+' }, // 26
	{ {133,135,133+48,135+33}, _T("-"), '-' }, // 27
	{ {176,135,176+48,135+33}, _T("/"), '/' }, // 28
	{ {218,135,218+48,135+33}, _T("%"), '%' }, // 29

	{ {  6,164,  6+48,164+33}, _T("?"), '?' }, // 30
	{ { 49,164, 49+48,164+33}, _T("!"), '!' }, // 31
	{ { 91,164, 91+48,164+33}, _T("^"), '^' }, // 32
	{ {133,164,133+48,164+33}, _T("$"), '$' }, // 33
	{ {176,164,176+48,164+33}, _T("\\"), '\\' }, // 34
	{ {218,164,218+48,164+33}, _T("@"), '@' }, // 35
};


static int                  g_englishs_keymap_count = 33;
static CInputPanel::KEYITEM g_englishs_keymap [33]  = 
{
	{ { 29,208, 29+52,208+22}, _T("한글"),  0x0000 },
	{ { 85,208, 85+52,208+22}, _T("영대"),  0x0000 },
	{ {140,208,140+52,208+22}, _T("영소"),  0x0000 },
	{ {196,208,196+52,208+22}, _T("숫/특"), 0x0000 }, 

	{ {387, 77,387+48, 77+33}, _T("<-"),    0x0008 }, // back space
	{ {387,106,387+48,106+33}, _T("space"), 0x0020 }, // space
	{ {387,135,387+48,135+33}, _T("enter"), 0x0020 }, // enter

	{ {  6, 77,  6+48, 77+33}, _T("a"), 'a' }, // 0
	{ { 49, 77, 49+48, 77+33}, _T("b"), 'b' }, // 1
	{ { 91, 77, 91+48, 77+33}, _T("c"), 'c' }, // 2
	{ {133, 77,133+48, 77+33}, _T("d"), 'd' }, // 3
	{ {176, 77,176+48, 77+33}, _T("e"), 'e' }, // 4
	{ {218, 77,218+48, 77+33}, _T("f"), 'f' }, // 5
	{ {260, 77,260+48, 77+33}, _T("g"), 'g' }, // 6
	{ {303, 77,303+48, 77+33}, _T("h"), 'h' }, // 7
	{ {345, 77,345+48, 77+33}, _T("i"), 'i' }, // 8

	{ {  6,106,  6+48,106+33}, _T("j"), 'j' }, // 9
	{ { 49,106, 49+48,106+33}, _T("k"), 'k' }, // 10
	{ { 91,106, 91+48,106+33}, _T("l"), 'l' }, // 11
	{ {133,106,133+48,106+33}, _T("m"), 'm' }, // 12
	{ {176,106,176+48,106+33}, _T("n"), 'n' }, // 13
	{ {218,106,218+48,106+33}, _T("o"), 'o' }, // 14
	{ {260,106,260+48,106+33}, _T("p"), 'p' }, // 15
	{ {303,106,303+48,106+33}, _T("q"), 'q' }, // 16
	{ {345,106,345+48,106+33}, _T("r"), 'r' }, // 17

	{ {  6,135,  6+48,135+33}, _T("s"), 's' }, // 18
	{ { 49,135, 49+48,135+33}, _T("t"), 't' }, // 19
	{ { 91,135, 91+48,135+33}, _T("u"), 'u' }, // 20
	{ {133,135,133+48,135+33}, _T("v"), 'v' }, // 21
	{ {176,135,176+48,135+33}, _T("w"), 'w' }, // 22
	{ {218,135,218+48,135+33}, _T("x"), 'x' }, // 23
	{ {260,135,260+48,135+33}, _T("y"), 'y' }, // 24
	{ {303,135,303+48,135+33}, _T("z"), 'z' }, // 25
};


/////////////////////////////////////////////////////////////////////////////
// CInputPanel

CInputPanel::CInputPanel() :
	m_bTracking  (FALSE),
	m_nCurrentKey(-1),
	m_nActiveKey (-1),
	
	m_nKeyMapType (0),
	m_lpKeyMap    (&g_hangul_keymap[0]),
	m_nKeyMapCount(g_hangul_keymap_count),

	m_bHangulUnion(FALSE),
	m_nCursor     (0),
	m_uMaxLength  (32)
{
}

CInputPanel::~CInputPanel()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInputPanel, CWnd)
	//{{AFX_MSG_MAP(CInputPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CInputPanel::SetMaxLength (ULONG length)
{
	if (length>32)
		length = 32;

	m_uMaxLength = length;
}

void CInputPanel::GetInputText (LPTSTR text, ULONG size)
{
	//_tcsncpy (text, m_szText, size);
}

BOOL CInputPanel::HitTest (CPoint pt, int &index, WCHAR &code)
{
	CRect rcKey;
	int   i;


	for (i=0; i<m_nKeyMapCount; i++)
	{
		rcKey = m_lpKeyMap[i].rc;
//		rcKey.OffsetRect (g_keymap_start.x, g_keymap_start.y);

		if (rcKey.PtInRect (pt))
		{
			index = i;
			code  = m_lpKeyMap[i].code;
			return TRUE;
		}
	}

	return FALSE;
}

void CInputPanel::SetKeyMap (int nKeyMap)
{
	switch (nKeyMap)
	{
	case 0:
		if (m_bmKey.m_hObject!=NULL)
			m_bmKey.DeleteObject ();
		m_bmKey.LoadBitmap(IDB_BMP_KOREA);

		m_lpKeyMap    =&g_hangul_keymap [0];
		m_nKeyMapCount=g_hangul_keymap_count; 
		m_nKeyMapType =0;
		break;

	case 1:
		if (m_bmKey.m_hObject!=NULL)
			m_bmKey.DeleteObject ();
		m_bmKey.LoadBitmap(IDB_BMP_ALPHA);

		m_lpKeyMap    =&g_english_keymap [0];
		m_nKeyMapCount=g_english_keymap_count; 
		m_nKeyMapType =1;
		break;

	case 2:
		if (m_bmKey.m_hObject!=NULL)
			m_bmKey.DeleteObject ();
		m_bmKey.LoadBitmap(IDB_BMP_ALPHAS);

		m_lpKeyMap    =&g_englishs_keymap [0];
		m_nKeyMapCount=g_englishs_keymap_count;
		m_nKeyMapType =2;
		break;

	case 3:
		if (m_bmKey.m_hObject!=NULL)
			m_bmKey.DeleteObject ();
		m_bmKey.LoadBitmap(IDB_BMP_NUM);

		m_lpKeyMap    =&g_number_keymap[0];
		m_nKeyMapCount=g_number_keymap_count;
		m_nKeyMapType =3;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////

int CInputPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	ZeroMemory (m_szText, sizeof(WCHAR[1024]));
	m_bHangulUnion = FALSE;
	m_nCursor      = 0;

	m_bmBG.LoadBitmap(IDB_BMP_BG);
	m_bmKey.LoadBitmap(IDB_BMP_KOREA);
	m_bmSelect.LoadBitmap(IDB_BMP_SELECT);

	return 0;
}

void CInputPanel::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CInputPanel::Render (CDC *pDC)
{
	CDC      MemDC;
	CBitmap *pOldBitmap;

	MemDC.CreateCompatibleDC (pDC);

	pOldBitmap = (CBitmap*)MemDC.SelectObject (m_bmBG); 
	pDC->BitBlt (0,0,480,234, &MemDC, 0, 0, SRCCOPY);
	
	MemDC.SelectObject (m_bmKey); 
	pDC->BitBlt (6,76,429,122, &MemDC, 0, 0, SRCCOPY);	


	CRect rcKey;
	int   i;

	for (i=0; i<m_nKeyMapCount; i++)
	{
		rcKey = m_lpKeyMap[i].rc;

		if (m_nCurrentKey==i) 
		{
			MemDC.SelectObject (m_bmSelect); 

			:: TransparentBlt(pDC->GetSafeHdc(), rcKey.left,rcKey.top, rcKey.Width(),rcKey.Height(),
			                  MemDC.GetSafeHdc(), 0,0, 52,33,RGB(255,0,255));
		}
	}

//	if (pOldBitmap)
//		MemDC.SelectObject (pOldBitmap); 


	// Text
	CRect  rcText (15,50,425,75);
	CFont  TextFont;
	CFont *pOldFont;


	TextFont.CreatePointFont (120, _T("굴림"),  pDC);
	
	pDC->SetTextColor(RGB(0xff,0xff,0xff)); 
	pDC->SetBkMode   (TRANSPARENT);

	pOldFont = pDC->SelectObject (&TextFont);

	pDC->DrawText     (m_szText, -1, rcText, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	pDC->SelectObject (pOldFont);
}

void CInputPanel::OnPaint() 
{
	CPaintDC dc(this);
	CRect    rc;
	CBitmap  Bitmap;
	CBitmap *pOldBitmap;
	CDC      MemDC;

	GetClientRect (&rc);


	MemDC.CreateCompatibleDC     (&dc);
	Bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	pOldBitmap = MemDC.SelectObject (&Bitmap);
	
	Render (&MemDC);

	dc.BitBlt (0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject (pOldBitmap);
}

BOOL CInputPanel::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	//return CWnd::OnEraseBkgnd(pDC);
}

///////////////////////////////////////////////////////////////////////////////

void CInputPanel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int   nIndex;
	WCHAR nCharCode;

	if (HitTest(point, nIndex, nCharCode))
	{
		m_nActiveKey  = nIndex; 
		m_nCurrentKey = nIndex;
		m_bTracking   = TRUE;

		SetCapture ();

		Invalidate (FALSE);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CInputPanel::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rtExit(435,0,480,40);
	
	if(rtExit.PtInRect(point)) {
		::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSE, 0, 0);
	}
	if (m_bTracking)
	{
		m_bTracking = FALSE;
		ReleaseCapture ();
	

		int   nIndex;
		WCHAR nCharCode;

		if (HitTest(point, nIndex, nCharCode))
		{
			if (m_nCurrentKey == nIndex)
			{
				if (nIndex >= 0 && nIndex <= 3)
				{
					SetKeyMap (nIndex);
				}
				else
				{
					m_bHangulUnion = BuildInputTextW (TRUE,     m_bHangulUnion,
													  m_szText, m_uMaxLength,
													  nCharCode, &m_nCursor);
				}
			}
		}

		m_nCurrentKey = -1;
		m_nActiveKey  = -1;

		Invalidate (FALSE);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CInputPanel::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		int   nIndex;
		WCHAR nCharCode;

		if (!HitTest(point, nIndex, nCharCode))
			nIndex = -1;

		if (m_nActiveKey != -1)
		{
			if      (m_nCurrentKey == -1           && nIndex == m_nActiveKey)
			{
				m_nCurrentKey = m_nActiveKey;
				Invalidate (FALSE);
			}
			else if (m_nCurrentKey == m_nActiveKey && nIndex != m_nActiveKey)
			{
				m_nCurrentKey = -1;
				Invalidate (FALSE);
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}