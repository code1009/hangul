// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(__STDAFX_H__INCLUDED__)
#define __STDAFX_H__INCLUDED__

// Change these values to use different versions
#define WINVER        0x0400
#define _WIN32_WINNT  0x0400
#define _WIN32_IE     0x0400
#define _RICHEDIT_VER 0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

// Addtional header files
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlscrl.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__STDAFX_H__INCLUDED__)
