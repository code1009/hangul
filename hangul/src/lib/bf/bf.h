#ifndef __bf_h__
#define __bf_h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "bf_config.h"
#include "bf_type.h"

#include "bf_api.h"

#include "bf_font.h"
#include "bf_font_bitmap.h"
#include "bf_context.h"
#include "bf_char_bitmap.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*

# 주의사항
BF_API void bf_get_font_bitmap(
	bf_font_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_font_bitmap_hangul_johab844_XXX(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	...
);

함수들의 index파라메터는 의도적으로 범위검사를 하지 않음.
호출하는 쪽에서 범위검사를 해야함.

*/




#endif




