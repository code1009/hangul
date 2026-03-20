#ifndef __bf_context__h__
#define __bf_context__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf_context.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _bf_context_t
{
	bf_font_t* font_unknown;

	bf_font_t* font_ascii_low;

	bf_font_hangul_johab844_t* font_hangul_johab844;
	bf_uint8_t                 font_hangul_johab_bitmap_buffer[32]; // 16x16 = (16/2) * 16 = 32

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
	bf_font_t* font_cp949_special_1128;
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
	bf_font_t* font_cp949_hanja_4888;
#endif
}
bf_context_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_context_default_setup(void);
BF_API bf_context_t* bf_context_default_get(void);





#endif




