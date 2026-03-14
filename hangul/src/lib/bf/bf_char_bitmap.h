#ifndef __bf_char_bitmap__h__
#define __bf_char_bitmap__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf_char_bitmap.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_cp949_bitmap(
	bf_context_t* ctx,
	const bf_uint16_t char_code,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_unicode_bitmap(
	bf_context_t* ctx,
	const bf_uint16_t char_unicode,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_utf8_bitmap(
	bf_context_t* ctx,
	const bf_uint8_t* char_poiner,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_utf8l_bitmap(
	bf_context_t* ctx,
	const bf_uint8_t* char_pointer,
	const bf_uint32_t char_length,
	bf_font_bitmap_t* font_bitmap
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif




