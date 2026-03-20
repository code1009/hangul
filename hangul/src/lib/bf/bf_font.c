/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font.c
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API bf_bool_t bf_font_setup_08x16(bf_font_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size, bf_uint32_t font_type, bf_uint32_t font_count)
{
	bf_uint32_t calc_font_data_size;
	calc_font_data_size = (8 / 8 * 16) * font_count;
	if (font_data_size != calc_font_data_size)
	{
		return BF_FALSE;
	}

	ctx->font_data_pointer = font_data_pointer;
	ctx->font_data_size = font_data_size;
	ctx->font_type = font_type;
	ctx->font_count = font_count;
	ctx->font_cx = 8u;
	ctx->font_cy = 16u;
	ctx->font_glyph_size = ctx->font_data_size / ctx->font_count;
	ctx->font_glyph_stride = ctx->font_cx / 8u;

	return BF_TRUE;
}

BF_API bf_bool_t bf_font_setup_16x16(bf_font_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size, bf_uint32_t font_type, bf_uint32_t font_count)
{
	bf_uint32_t calc_font_data_size;
	calc_font_data_size = (16 / 8 * 16) * font_count;
	if (font_data_size != calc_font_data_size)
	{
		return BF_FALSE;
	}

	ctx->font_data_pointer = font_data_pointer;
	ctx->font_data_size = font_data_size;
	ctx->font_type = font_type;
	ctx->font_count = font_count;
	ctx->font_cx = 16u;
	ctx->font_cy = 16u;
	ctx->font_glyph_size = ctx->font_data_size / ctx->font_count;
	ctx->font_glyph_stride = ctx->font_cx / 8u;

	return BF_TRUE;
}

BF_API bf_bool_t bf_font_setup_hangul_johab844_16x16(bf_font_hangul_johab844_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size)
{
	bf_uint32_t font_count = (19u * 8u) + (21u * 4u) + (27u * 4u);
	bf_uint32_t calc_font_data_size;
	calc_font_data_size = (16 / 8 * 16) * font_count; // 11008
	if (font_data_size != calc_font_data_size)
	{
		return BF_FALSE;
	}


	ctx->font_data_pointer = font_data_pointer;
	ctx->font_data_size = font_data_size;
	ctx->font_type = BF_FONT_TYPE_HANGUL_JOHAB844;
	ctx->font_count = font_count;
	ctx->font_cx = 16u;
	ctx->font_cy = 16u;
	ctx->font_glyph_size = ctx->font_data_size / ctx->font_count;
	ctx->font_glyph_stride = ctx->font_cx / 8u;

	ctx->choseong[0] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 0) + (21 * 0) + (27 * 0))];
	ctx->choseong[1] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 1) + (21 * 0) + (27 * 0))];
	ctx->choseong[2] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 2) + (21 * 0) + (27 * 0))];
	ctx->choseong[3] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 3) + (21 * 0) + (27 * 0))];
	ctx->choseong[4] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 4) + (21 * 0) + (27 * 0))];
	ctx->choseong[5] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 5) + (21 * 0) + (27 * 0))];
	ctx->choseong[6] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 6) + (21 * 0) + (27 * 0))];
	ctx->choseong[7] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 7) + (21 * 0) + (27 * 0))];

	ctx->jungseong[0] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 0) + (27 * 0))];
	ctx->jungseong[1] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 1) + (27 * 0))];
	ctx->jungseong[2] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 2) + (27 * 0))];
	ctx->jungseong[3] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 3) + (27 * 0))];

	ctx->jongseong[0] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 4) + (27 * 0))];
	ctx->jongseong[1] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 4) + (27 * 1))];
	ctx->jongseong[2] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 4) + (27 * 2))];
	ctx->jongseong[3] = &ctx->font_data_pointer[ctx->font_glyph_size * ((19 * 8) + (21 * 4) + (27 * 3))];

	return BF_TRUE;
}


