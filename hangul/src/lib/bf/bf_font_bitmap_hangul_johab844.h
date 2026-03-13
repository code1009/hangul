#ifndef __bf_font_bitmap_hangul_johab844__h__
#define __bf_font_bitmap_hangul_johab844__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font_bitmap_hangul_johab844.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t choseong, const bf_uint32_t jungseong, const bf_uint32_t jongseong,
	bf_font_bitmap_t* font_bitmap,
	bf_uint8_t* font_bitmap_buffer
);

//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844_jamo_choseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jungseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jongseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);

//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844_jamo_moeum(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jaeum(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
);


#endif




