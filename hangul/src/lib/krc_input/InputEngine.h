#if !defined(__INPUTENGINE__H__)
#define __INPUTENGINE__H__

#pragma once

BOOL BuildInputTextW (BOOL  bSingleLine, BOOL bHangulUnion,
                      WORD *pText,       int  nMaxLength,
                      WORD  nCharCode,   int *pCursor);

#endif 