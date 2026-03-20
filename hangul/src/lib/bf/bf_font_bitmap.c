/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font_bitmap.c
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "bf_config.h"
#include "bf_type.h"
#include "bf_api.h"

#include "bf_font.h"

#include "bf_font_bitmap.h"






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_font_bitmap(
	bf_font_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	font_bitmap->font_bitmap_pointer = font->font_data_pointer + font->font_glyph_size * index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}
