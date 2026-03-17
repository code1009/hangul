/////////////////////////////////////////////////////////////////////////////
//
// CoreCode.h
//
/////////////////////////////////////////////////////////////////////////////



#if !defined(__CORECODE_H__INCLUDED__)
#define __CORECODE_H__INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define _CRT_SECURE_NO_WARNINGS



///////////////////////////////////////////////////////////////////////////////
//
// Global functions
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetFileSize (LPCTSTR lpFilePath)
{
	HANDLE          hContext;
	WIN32_FIND_DATA FindData;
	
	
	hContext = ::FindFirstFile (lpFilePath, &FindData);
	if (INVALID_HANDLE_VALUE == hContext)
		return 0;
	
	::FindClose(hContext); 
	
	return FindData.nFileSizeLow;
}



///////////////////////////////////////////////////////////////////////////////
//
// Class: CAboutDialog
//
///////////////////////////////////////////////////////////////////////////////
class CAboutDialog : public CDialogImpl<CAboutDialog>
{
public:
	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CAboutDialog)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_ID_HANDLER(IDOK,     OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)

	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};



///////////////////////////////////////////////////////////////////////////////
//
// Class: CFontData
//
///////////////////////////////////////////////////////////////////////////////
class CFontData
{
// ------------------------------------------------------------------------- //
//   Property                                                                //
// ------------------------------------------------------------------------- //
public:
	LPBYTE m_lpFontBuffer;
	DWORD  m_dwFontBufferSize;

	DWORD  m_dwFontWidth;
	DWORD  m_dwFontHeight;
	DWORD  m_dwFontCount;
	DWORD  m_dwFontCurrentIndex;



// ------------------------------------------------------------------------- //
//   Method                                                                  //
// ------------------------------------------------------------------------- //
public:
	// ---------------------------------------------------------------------
	//   Initialize properties
	// ---------------------------------------------------------------------
	CFontData ()
	{
		m_lpFontBuffer       = NULL;
		m_dwFontBufferSize   = 0;

		m_dwFontWidth        = 16;
		m_dwFontHeight       = 16;

		m_dwFontCount        = 0;
		m_dwFontCurrentIndex = 0;
	}

	virtual ~CFontData ()
	{
		if (m_lpFontBuffer != NULL)
			delete []m_lpFontBuffer;
	}

	void ClearData ()
	{
		if (m_lpFontBuffer != NULL)
			delete []m_lpFontBuffer;

		m_lpFontBuffer       = NULL;
		m_dwFontBufferSize   = 0;
		
		m_dwFontWidth        = 16;
		m_dwFontHeight       = 16;
		m_dwFontCount        = 0;
		m_dwFontCurrentIndex = 0;
	}


	// ---------------------------------------------------------------------
	//   Serialize font data
	// ---------------------------------------------------------------------
	BOOL SaveCodeFile (LPTSTR lpFileName)
	{
		HANDLE hFile                  = INVALID_HANDLE_VALUE;
		DWORD  dwNumberOfBytesWritten = 0;
		DWORD  dwIndex                = 0;
		TCHAR  szBuffer [1024];
		DWORD  dwCharSize;

		if (m_lpFontBuffer == NULL || m_dwFontBufferSize==0)
			return FALSE;

		hFile = CreateFile (lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE)
			return FALSE;

		
		lstrcpy (szBuffer, _T("UINT8 g_pFontData[] = {\r\n"));
		if (!WriteFile (hFile, szBuffer, lstrlen (szBuffer)*sizeof(TCHAR), &dwNumberOfBytesWritten, NULL))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		dwCharSize = (m_dwFontWidth/8)*m_dwFontHeight;

		for (dwIndex=0; dwIndex<m_dwFontBufferSize; dwIndex++)
		{
			if (dwIndex != m_dwFontBufferSize-1)
			{
				if      ( 0            == dwIndex % dwCharSize )
					_stprintf (szBuffer, _T("\t0x%02x, "),  m_lpFontBuffer[dwIndex]);

				else if ( dwCharSize-1 == dwIndex % dwCharSize )
					_stprintf (szBuffer, _T("0x%02x,\r\n"), m_lpFontBuffer[dwIndex]);

				else
					_stprintf (szBuffer, _T("0x%02x, "),    m_lpFontBuffer[dwIndex]);
			}
			else
				_stprintf (szBuffer, _T("0x%02x\r\n};\r\n"), m_lpFontBuffer[dwIndex]);


			if (!WriteFile (hFile, szBuffer, lstrlen (szBuffer)*sizeof(TCHAR), &dwNumberOfBytesWritten, NULL))
			{
				CloseHandle (hFile);
				return FALSE;
			}
		}

		CloseHandle(hFile);

		return TRUE;
	}

	BOOL ReadFontFile (LPTSTR lpFileName)
	{
		HANDLE hFile      = INVALID_HANDLE_VALUE;
		DWORD  dwFileSize = 0;


		ClearData ();

		dwFileSize = GetFileSize (lpFileName);
		if (dwFileSize == 0)
			return FALSE;


		m_lpFontBuffer = new BYTE[dwFileSize];
		if (m_lpFontBuffer == NULL)
			return FALSE;


		hFile = CreateFile (lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE)
			return FALSE;

		if (!ReadFile (hFile, m_lpFontBuffer, dwFileSize, &m_dwFontBufferSize, NULL))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		CloseHandle(hFile);


		return TRUE;
	}


	// ---------------------------------------------------------------------
	//   Action
	// ---------------------------------------------------------------------
	void RecalcProperty ()
	{
		RecalcProperty (m_dwFontWidth, m_dwFontHeight);
	}

	void RecalcProperty (DWORD dwFontWidth, DWORD dwFontHeight)
	{
		m_dwFontCurrentIndex = 0;

		m_dwFontWidth  = dwFontWidth;
		m_dwFontHeight = dwFontHeight;

		m_dwFontCount  = m_dwFontBufferSize/((m_dwFontWidth/8)*m_dwFontHeight);
	}

	
	void MoveFirst ()
	{
		if (m_dwFontCount>0 && m_dwFontCurrentIndex>0)
			m_dwFontCurrentIndex = 0;
	}

	void MovePrev ()
	{
		if (m_dwFontCount>0 && m_dwFontCurrentIndex>0)
			m_dwFontCurrentIndex --;
	}

	void MoveNext ()
	{
		if (m_dwFontCount>0 && m_dwFontCurrentIndex<(m_dwFontCount-1))
			m_dwFontCurrentIndex ++;
	}

	void MoveLast ()
	{
		if (m_dwFontCount>0 && m_dwFontCurrentIndex<(m_dwFontCount-1))
			m_dwFontCurrentIndex = m_dwFontCount-1;
	}

	// ---------------------------------------------------------------------
	//   Verify
	// ---------------------------------------------------------------------
	BOOL IsEmpty ()
	{
		if ( (m_lpFontBuffer     == NULL) ||
		     (m_dwFontBufferSize == 0   ) )
			return TRUE;

		return FALSE;
	}

	BOOL CanMoveFirst ()
	{
		if (IsEmpty ())
			return FALSE;

		if (m_dwFontCurrentIndex > 0)
			return TRUE;

		return FALSE;
	}

	BOOL CanMovePrev ()
	{
		return CanMoveFirst ();
	}

	BOOL CanMoveNext ()
	{
		if (IsEmpty ())
			return FALSE;

		if (m_dwFontCurrentIndex < m_dwFontCount-1)
			return TRUE;

		return FALSE;
	}

	BOOL CanMoveLast ()
	{
		return CanMoveNext ();
	}
};



///////////////////////////////////////////////////////////////////////////////
//
// Class: CFontView
//
///////////////////////////////////////////////////////////////////////////////
class CFontView : public CScrollWindowImpl<CFontView>
{
// ------------------------------------------------------------------------- //
//   Macro                                                                   //
// ------------------------------------------------------------------------- //
public:
	DECLARE_WND_CLASS(NULL)


	BEGIN_MSG_MAP(CFontView)

		MESSAGE_HANDLER(WM_LBUTTONUP,  OnLButtonUp)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

		CHAIN_MSG_MAP(CScrollWindowImpl<CFontView>);

	ALT_MSG_MAP(1)

		COMMAND_ID_HANDLER(ID_FONTVIEW_FIRST, OnMovePosition)
		COMMAND_ID_HANDLER(ID_FONTVIEW_PREV,  OnMovePosition)
		COMMAND_ID_HANDLER(ID_FONTVIEW_NEXT,  OnMovePosition)
		COMMAND_ID_HANDLER(ID_FONTVIEW_LAST,  OnMovePosition)
		COMMAND_ID_HANDLER(ID_FONTVIEW_TYPE,  OnFontViewType)

	END_MSG_MAP()



// ------------------------------------------------------------------------- //
//   Property                                                                //
// ------------------------------------------------------------------------- //
public:
	CFontData *m_pDocument;
	CSize      m_ViewSize;
	CBitmap    m_MemoryBitmap;



// ------------------------------------------------------------------------- //
//   Method                                                                  //
// ------------------------------------------------------------------------- //
public:
	CFontView()
	{
		m_pDocument   = NULL;
		m_ViewSize.cx = 1;
		m_ViewSize.cy = 1;
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	void DrawCurrentFont (CDC *pDC)
	{
		int    nScale = 20;
		int    nDepth = 20;

		// Font 
		CRect Peice (0,0,0,0);
		BYTE  byData;
		DWORD dwIndex;
		DWORD dwBit;
		DWORD dwX;
		DWORD dwCharFontSize;


		dwCharFontSize = (m_pDocument->m_dwFontWidth/8)*m_pDocument->m_dwFontHeight;
		Peice.top      = nDepth - nScale;
		
		for (dwIndex = dwCharFontSize*(m_pDocument->m_dwFontCurrentIndex  );
		     dwIndex < dwCharFontSize*(m_pDocument->m_dwFontCurrentIndex+1);
			 dwIndex ++)
		{
			byData = m_pDocument->m_lpFontBuffer[dwIndex];
			
			if  ((dwIndex%(m_pDocument->m_dwFontWidth/8))==0)
			{
				Peice.top += nScale;
				dwX        = 0;
			}
			else
				dwX += (8*nScale);
				
				
			for (dwBit=0; dwBit<8; dwBit++)
			{
				Peice.left   = nDepth + dwX + dwBit*nScale;
				Peice.right  = Peice.left + nScale;
					
				Peice.bottom = Peice.top  + nScale;
				
				if (byData & (0x80 >> dwBit) )
				{
					pDC->FillSolidRect (&Peice, RGB(0x00,0x00,0x00));
				}
			}
		}


		// Grid
		DWORD dwRow,   dwCol;
		CPen  LinePen, OldPen;

		
		LinePen.CreatePen (PS_SOLID, 1, RGB(0x80, 0x80, 0xff));

		OldPen = pDC->SelectPen (LinePen);

		for (dwRow=0; dwRow<=m_pDocument->m_dwFontHeight; dwRow++)
		{
			pDC->MoveTo (nDepth + 0,                                 nDepth + dwRow*nScale);
			pDC->LineTo (nDepth + m_pDocument->m_dwFontWidth*nScale, nDepth + dwRow*nScale);
		}

		for (dwCol=0; dwCol<=m_pDocument->m_dwFontWidth; dwCol++)
		{
			pDC->MoveTo (nDepth + dwCol*nScale, nDepth + 0);
			pDC->LineTo (nDepth + dwCol*nScale, nDepth + m_pDocument->m_dwFontHeight*nScale);
		}

		pDC->SelectPen (LinePen);


		// Text
		TCHAR szBuffer [4096];
		CRect TextRect ;
		CFont TextFont, OldFont;

		TextFont.CreatePointFont (90, _T("FixedSys"), pDC->m_hDC);

		TextRect.left   = nDepth;
		TextRect.top    = nDepth +  nScale*m_pDocument->m_dwFontHeight + nDepth;
		TextRect.right  = m_ViewSize.cx - 1;
		TextRect.bottom = m_ViewSize.cy - 1;		

		_stprintf (szBuffer, _T("Font Count : %d\r\nFont Index : %d\nOffset     : %d\nWidth      : %d\nHeight     : %d\n"),
		           m_pDocument->m_dwFontCount,
		           m_pDocument->m_dwFontCurrentIndex,
		           m_pDocument->m_dwFontCurrentIndex*dwCharFontSize,
		           m_pDocument->m_dwFontWidth,
		           m_pDocument->m_dwFontHeight		
				   );

		OldFont = pDC->SelectFont (TextFont);
		pDC->SetBkMode  (TRANSPARENT);
		pDC->DrawText   (szBuffer, -1, &TextRect, 0);
		pDC->SelectFont (OldFont);
	}

	void DrawFontList (CDC *pDC)
	{
		int   nScale     = 20;
		int   nDepth     = 20;
		int   nGridWidth = nScale*m_pDocument->m_dwFontWidth;
		DWORD dwColCount = 32;

		int nStartX    = nDepth + nGridWidth + nDepth;
		int nStartY    = nDepth;


		// Cursor
		pDC->FillSolidRect (nStartX + m_pDocument->m_dwFontCurrentIndex%dwColCount*m_pDocument->m_dwFontWidth,
		                    nStartY + m_pDocument->m_dwFontCurrentIndex/dwColCount*m_pDocument->m_dwFontHeight,
		                    m_pDocument->m_dwFontWidth,
							m_pDocument->m_dwFontHeight, RGB(0x80,0x80,0xff));

		
		// Grid
		DWORD dwRow,   dwCol;
		DWORD dwLine;
		CPen  LinePen, OldPen;

		
		if ((m_pDocument->m_dwFontCount%dwColCount)==0)
			dwLine = m_pDocument->m_dwFontCount/dwColCount;
		else
			dwLine = m_pDocument->m_dwFontCount/dwColCount+1;

		LinePen.CreatePen (PS_SOLID, 1, RGB(0xa0, 0xa0, 0xa0));
		OldPen = pDC->SelectPen (LinePen);

		for (dwCol=0; dwCol<=dwColCount; dwCol++)
		{
			pDC->MoveTo (nStartX + dwCol*m_pDocument->m_dwFontWidth, nStartY + 0);
			pDC->LineTo (nStartX + dwCol*m_pDocument->m_dwFontWidth, nStartY + dwLine*m_pDocument->m_dwFontHeight);
		}
		for (dwRow=0; dwRow<=dwLine; dwRow++)
		{
			pDC->MoveTo (nStartX + 0,                                     nStartY + dwRow*m_pDocument->m_dwFontHeight);
			pDC->LineTo (nStartX + dwColCount*m_pDocument->m_dwFontWidth, nStartY + dwRow*m_pDocument->m_dwFontHeight);
		}

		pDC->SelectPen (LinePen);


		// List
		int nCharX, nCharY;
		int nFontY, nFontX;

		DWORD dwFontIndex;

		DWORD dwBufferIndex;
		DWORD dwCharFontSize;
		DWORD dwBit;
		BYTE  byData;


		dwCharFontSize = (m_pDocument->m_dwFontWidth/8)*m_pDocument->m_dwFontHeight;

		for (dwFontIndex=0; dwFontIndex<m_pDocument->m_dwFontCount; dwFontIndex++)
		{
			nCharX = nStartX + dwFontIndex%dwColCount*m_pDocument->m_dwFontWidth;
			nCharY = nStartY + dwFontIndex/dwColCount*m_pDocument->m_dwFontHeight;

			nFontX = 0;
			nFontY = -1;

			for (dwBufferIndex = dwCharFontSize*(dwFontIndex  );
			     dwBufferIndex < dwCharFontSize*(dwFontIndex+1);
			     dwBufferIndex ++)
			{
				byData = m_pDocument->m_lpFontBuffer[dwBufferIndex];
				
				
				if  ( (dwBufferIndex%(m_pDocument->m_dwFontWidth/8)) == 0 )
				{
					nFontY++;
					nFontX  = 0;
				}
				
				for (dwBit=0; dwBit<8; dwBit++)
				{
					nFontX ++;
					
					if (byData & (0x80 >> dwBit) )
					{
						pDC->SetPixel (nFontX + nCharX,
						               nFontY + nCharY,
						               RGB(0x00,0x00,0x00));
					}
				}

			}
		}
	}

	void Draw (CDC *pDC)
	{
		pDC->FillSolidRect (0, 0, m_ViewSize.cx, m_ViewSize.cy, RGB(0xff,0xff,0xff));

		if (!m_pDocument->IsEmpty ())
		{
			DrawCurrentFont (pDC);
			DrawFontList    (pDC);
		}
	}

	void CalcViewSize (SIZE &ViewSize)
	{
		int nScale    = 20;
		int nDepth    = 20;
		int nColCount = 32;

		int nGridWidth;
		int nGridHeight;
		int nListHeight;
		int nListWidth;


		if (!m_pDocument->IsEmpty())
		{
			nGridWidth  = nScale*m_pDocument->m_dwFontWidth;
			nGridHeight = nScale*m_pDocument->m_dwFontHeight + nDepth + 5*20;
			
			nListWidth  = m_pDocument->m_dwFontWidth*nColCount;
			if (m_pDocument->m_dwFontCount%nColCount)
				nListHeight = (m_pDocument->m_dwFontCount/nColCount  )*m_pDocument->m_dwFontHeight;
			else
				nListHeight = (m_pDocument->m_dwFontCount/nColCount+1)*m_pDocument->m_dwFontHeight;

			ViewSize.cx = nDepth + nGridWidth  + nDepth + nListWidth + nDepth;
			ViewSize.cy = max (nDepth + nListHeight + nDepth, nGridHeight);
		}
		else
		{
			ViewSize.cx = 1;
			ViewSize.cy = 1;
		}
	}

	void DoPaint(CDCHandle dc)
	{
		//CBitmap  MemoryBitmap;
		CRect    ClientRect;
		CDC      MemoryDC;
		int      nDCSaved;

		if (m_pDocument != NULL && (!m_pDocument->IsEmpty()))
		{
			if (!m_MemoryBitmap.IsNull())
			{
			// We double buffer the drawing preparing the memory CDC
			MemoryDC.CreateCompatibleDC (dc.m_hDC);
			nDCSaved = MemoryDC.SaveDC();
			
			// Create GDI and select objects
			//MemoryBitmap.CreateCompatibleBitmap (dc.m_hDC, m_ViewSize.cx, m_ViewSize.cy);
			MemoryDC.SelectBitmap (m_MemoryBitmap);
			
			// Drawing
			//Draw      (&MemoryDC);			
			dc.BitBlt (0, 0, m_ViewSize.cx, m_ViewSize.cy, MemoryDC, 0, 0, SRCCOPY);
			
			// Free GDI resources
			MemoryDC.RestoreDC(nDCSaved);
			}
		}
		else
		{
			GetClientRect    (&ClientRect);
			dc.FillSolidRect (&ClientRect, RGB(0xff,0xff,0xff));
		}
	}

	LRESULT OnLButtonUp (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		POINT ptMouse;

		ptMouse.x = LOWORD(lParam);  // horizontal position of cursor 
		ptMouse.y = HIWORD(lParam);  // vertical position of cursor


		return 0;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect     rect;
		int       x  = 0;
		int       y  = 0;
		CDCHandle dc = (HDC)wParam;


		GetClientRect(&rect);

		if(!m_pDocument->IsEmpty())
		{
			x = m_ViewSize.cx;
			y = m_ViewSize.cy; 

			if(rect.right > m_sizeAll.cx)
			{
				RECT rectRight   = rect;
				rectRight.left   = x;
				rectRight.bottom = y;
				dc.FillRect(&rectRight, COLOR_WINDOW);
			}
			if(rect.bottom > m_sizeAll.cy)
			{
				RECT rectBottom = rect;
				rectBottom.top  = y;
				dc.FillRect(&rectBottom, COLOR_WINDOW);
			}
		}
		else
			dc.FillRect(&rect, COLOR_WINDOW);

		return 0;
	}
	
	LRESULT OnMovePosition (WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		switch (wID)
		{
		case ID_FONTVIEW_FIRST:
			m_pDocument->MoveFirst();
			break;

		case ID_FONTVIEW_PREV:
			m_pDocument->MovePrev ();
			break;

		case ID_FONTVIEW_NEXT:
			m_pDocument->MoveNext ();
			break;

		case ID_FONTVIEW_LAST:
			m_pDocument->MoveLast ();
			break;
		}

		CreateViewBuffer();
		Invalidate (FALSE);

		return 0;
	}

	void CreateViewBuffer (void)
	{
		// Make the bitmap
		CDC MemoryDC;
		int nDCSaved;
		HDC hScreenDC;

		if (m_pDocument != NULL && (!m_pDocument->IsEmpty()))
		{
			hScreenDC = GetDC();


			if (!m_MemoryBitmap.IsNull())
				m_MemoryBitmap.DeleteObject ();

			// We double buffer the drawing preparing the memory CDC
			MemoryDC.CreateCompatibleDC (hScreenDC);
			nDCSaved = MemoryDC.SaveDC();
			
			// Create GDI and select objects
			m_MemoryBitmap.CreateCompatibleBitmap (hScreenDC, m_ViewSize.cx, m_ViewSize.cy);
			MemoryDC.SelectBitmap (m_MemoryBitmap);
			
			// Drawing
			Draw (&MemoryDC);			
			
			// Free GDI resources
			MemoryDC.RestoreDC(nDCSaved);	


			ReleaseDC (hScreenDC);
		}
	}

	LRESULT OnFontViewType (WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (m_pDocument->m_dwFontWidth == 16)
			m_pDocument->RecalcProperty ( 8, 16);
		else
			m_pDocument->RecalcProperty (16, 16);

		RecalcLayout ();

		return 0;
	}

	void RecalcLayout ()
	{
		m_pDocument->RecalcProperty  ();

		CalcViewSize    (m_ViewSize);
		SetScrollOffset (0, 0, FALSE);

		CreateViewBuffer();

		SetScrollSize   (m_ViewSize);
	}
};





///////////////////////////////////////////////////////////////////////////////
//
// Class: CMainFrame
//
///////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWindowImpl<CMainFrame>,
                   public CUpdateUI<CMainFrame>,
                   public CMessageFilter,
				   public CIdleHandler
{
// ------------------------------------------------------------------------- //
//   Macro                                                                   //
// ------------------------------------------------------------------------- //
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)


	BEGIN_MSG_MAP(CMainFrame)
		
		MESSAGE_HANDLER(WM_CREATE,  OnCreate)

		COMMAND_ID_HANDLER(ID_APP_EXIT,  OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnHelpAbout)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)

		CHAIN_COMMANDS_ALT_MEMBER((m_View), 1)

	END_MSG_MAP()


	BEGIN_UPDATE_UI_MAP(CMainFrame)

		UPDATE_ELEMENT(ID_FILE_SAVE,      UPDUI_MENUPOPUP)

		UPDATE_ELEMENT(ID_FONTVIEW_FIRST, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FONTVIEW_PREV,  UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FONTVIEW_NEXT,  UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FONTVIEW_LAST,  UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FONTVIEW_TYPE,  UPDUI_TOOLBAR)

	END_UPDATE_UI_MAP()



// ------------------------------------------------------------------------- //
//   Property                                                                //
// ------------------------------------------------------------------------- //
	CCommandBarCtrl m_CmdBar;
	CFontView       m_View;
	CFontData       m_Document;


// ------------------------------------------------------------------------- //
//   Method                                                                  //
// ------------------------------------------------------------------------- //
	CMainFrame ()
	{
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_View.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UpdateUIAll ();
		return FALSE;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		UINT uResID     = IDR_MAINFRAME;
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

		m_CmdBar.AttachMenu(GetMenu());
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		SetMenu            (NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, uResID, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar    (ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand   (hWndCmdBar);
		AddSimpleReBarBand   (hWndToolBar, NULL, TRUE);
		CreateSimpleStatusBar();

		
		m_View.m_pDocument = &m_Document;
		m_hWndClient       = m_View.Create(m_hWnd, rcDefault, NULL,
		                     WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);


		UIAddToolBar (hWndToolBar);


		CMessageLoop* pLoop = _Module.GetMessageLoop();

		ATLASSERT(pLoop != NULL);

		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler  (this);


		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);

		return 0;
	}

	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDialog dlg;

		dlg.DoModal (m_hWnd);

		return 0;
	}
	
	LRESULT OnFileOpen (WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static TCHAR szFilter[] = { "Bitmap Font File (*.FNT)\0*.FNT\0All Files (*.*)\0*.*\0\0" };
		CFileDialog  FileOpen (TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, m_hWnd); 


		if (FileOpen.DoModal () == IDOK)
		{
			if (!m_Document.ReadFontFile (FileOpen.m_szFileName))
			{
				m_Document.ClearData  ();
				MessageBox (MAKEINTRESOURCE(IDS_EMSG_READFILE));
			}
			else
			{
				m_View.RecalcLayout ();
			}
		}

		return 0;
	}

	LRESULT OnFileSave (WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static TCHAR szFilter[] = { "C Source file (*.C)\0*.C\0All Files (*.*)\0*.*\0\0" };
		CFileDialog  FileOpen (FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, m_hWnd); 


		if (FileOpen.DoModal () == IDOK)
		{
			if (!m_Document.SaveCodeFile (FileOpen.m_szFileName))
				MessageBox (MAKEINTRESOURCE(IDS_EMSG_WRITEFILE));
		}

		return 0;
	}

	void UpdateUIAll()
	{
		if (m_Document.m_dwFontWidth == 16)
			UISetCheck (ID_FONTVIEW_TYPE, TRUE);
		else
			UISetCheck (ID_FONTVIEW_TYPE, FALSE);


		if (m_Document.IsEmpty())
		{
			UIEnable (ID_FILE_SAVE,      FALSE);

			UIEnable (ID_FONTVIEW_FIRST, FALSE);
			UIEnable (ID_FONTVIEW_PREV,  FALSE);
			UIEnable (ID_FONTVIEW_NEXT,  FALSE);
			UIEnable (ID_FONTVIEW_LAST,  FALSE);
		}
		else
		{
			UIEnable (ID_FILE_SAVE, TRUE);

			UIEnable (ID_FONTVIEW_FIRST, m_Document.CanMoveFirst());
			UIEnable (ID_FONTVIEW_PREV,  m_Document.CanMovePrev ());
			UIEnable (ID_FONTVIEW_NEXT,  m_Document.CanMoveNext ());
			UIEnable (ID_FONTVIEW_LAST,  m_Document.CanMoveLast ());
		}


		UIUpdateToolBar ();
	}		
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CORECODE_H__INCLUDED__)
