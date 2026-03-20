/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font_bitmap_hangul_johab844.c
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

#include "bf_font_bitmap_hangul_johab844.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static bf_uint32_t bf_hangul_johab844_choseong_set(const bf_uint32_t choseong, const bf_uint32_t jungseong, const bf_uint32_t jongseong)
{
	bf_uint32_t set = 0u;
	bf_uint32_t temp = 0u;


	switch (jungseong)
	{
	case  0u:       // ㅏ
	case  1u:       // ㅐ
	case  2u:       // ㅑ
	case  3u:       // ㅒ
	case  4u:       // ㅓ
	case  5u:       // ㅔ
	case  6u:       // ㅕ
	case  7u:       // ㅖ
	case 20u:       // ㅣ
		set = 0u;
		break;

	case  8u:       // ㅗ
	case 12u:       // ㅛ
	case 18u:       // ㅡ
		set = 1u;
		break;

	case 13u:       // ㅜ
	case 17u:       // ㅠ
		set = 2u;
		break;

	case  9u:       // ㅘ
	case 10u:       // ㅙ
	case 11u:       // ㅚ
	case 19u:       // ㅢ
		set = 3u;
		break;

	case 14u:       // ㅝ
	case 15u:       // ㅞ
	case 16u:       // ㅟ
		set = 4u;
		break;

	default:
		set = 0u;
		break;
	}

	if (jongseong != 0u)
	{
		// 종성이 있으면
		switch (set)
		{
		case 0:
			temp = 5;
			break;

		case 1:
		case 2:
			temp = 6;
			break;

		case 3:
		case 4:
			temp = 7;
			break;

		default:
			temp = set;
			break;
		}

		set = temp;
	}

	return set;
}

static bf_uint32_t bf_hangul_johab844_jungseong_set(const bf_uint32_t choseong, const bf_uint32_t jungseong, const bf_uint32_t jongseong)
{
	bf_uint32_t set = 0u;


	if (jongseong == 0u)
	{
		// 종성이 없는 경우
		if (choseong == 0u || choseong == 1u) // ㄱㅋ
		{
			set = 0u;
		}
		else
		{
			set = 1u;
		}
	}
	else
	{
		// 종성이 있는 경우
		if (choseong == 0u || choseong == 1u) // ㄱㅋ
		{
			set = 2u;
		}
		else
		{
			set = 3u;
		}
	}

	return set;
}

static bf_uint32_t bf_hangul_johab844_jongseong_set(const bf_uint32_t choseong, const bf_uint32_t jungseong, const bf_uint32_t jongseong)
{
	bf_uint32_t set = 0u;


	switch (jungseong)
	{
	case  0u:       // ㅏ
	case  2u:       // ㅑ
	case  9u:       // ㅘ
	case 20u:       // ㅣ
		set = 0;
		break;

	case  4u:       // ㅓ
	case  6u:       // ㅕ
	case 11u:       // ㅚ
	case 14u:       // ㅝ
	case 16u:       // ㅟ
	case 19u:       // ㅢ
		set = 1;
		break;

	case  1u:       // ㅐ
	case  3u:       // ㅒ
	case  5u:       // ㅔ
	case  7u:       // ㅖ
	case 10u:       // ㅙ
	case 15u:       // ㅞ
		set = 2;
		break;

	case  8u:       // ㅗ
	case 12u:       // ㅛ
	case 13u:       // ㅜ
	case 17u:       // ㅠ
	case 18u:       // ㅡ
		set = 3;
		break;

	default:
		set = 0u;
		break;
	}

	return set;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t choseong, const bf_uint32_t jungseong, const bf_uint32_t jongseong,
	bf_font_bitmap_t* font_bitmap,
	bf_uint8_t* font_bitmap_buffer
)
{
	//-----------------------------------------------------------------------
	bf_uint32_t choseong_set;
	bf_uint32_t jungseong_set;
	bf_uint32_t jongseong_set;


	choseong_set  = bf_hangul_johab844_choseong_set (choseong, jungseong, jongseong);
	jungseong_set = bf_hangul_johab844_jungseong_set(choseong, jungseong, jongseong);
	jongseong_set = bf_hangul_johab844_jongseong_set(choseong, jungseong, jongseong);


	//-----------------------------------------------------------------------
	bf_uint8_t* font_bitmap_pointer_choseong;
	bf_uint8_t* font_bitmap_pointer_jungseong;
	bf_uint8_t* font_bitmap_pointer_jongseong;


	font_bitmap_pointer_choseong  = font->choseong [choseong_set ] + font->font_glyph_size * choseong;
	font_bitmap_pointer_jungseong = font->jungseong[jungseong_set] + font->font_glyph_size * jungseong;
	if (0u == jongseong)
	{
		font_bitmap_pointer_jongseong = BF_NULL_PTR;
	}
	else
	{
		font_bitmap_pointer_jongseong = font->jongseong[jongseong_set] + font->font_glyph_size * (jongseong - 1u);
	}


	//-----------------------------------------------------------------------
	bf_uint8_t* font_bitmap_pointer;
	bf_uint32_t font_bitmap_size;
	bf_uint32_t i;

	font_bitmap_pointer = font_bitmap_buffer;
	font_bitmap_size = font->font_glyph_size;

	if (0u == jongseong)
	{
		for (i = 0u; i < font_bitmap_size; i++)
		{
			font_bitmap_pointer[i] =
				font_bitmap_pointer_choseong [i] |
				font_bitmap_pointer_jungseong[i];
		}
	}
	else
	{
		for (i = 0u; i < font_bitmap_size; i++)
		{
			font_bitmap_pointer[i] =
				font_bitmap_pointer_choseong [i] |
				font_bitmap_pointer_jungseong[i] |
				font_bitmap_pointer_jongseong[i];
		}
	}


	//-----------------------------------------------------------------------
	font_bitmap->font_bitmap_pointer = font_bitmap_buffer;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844_jamo_choseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	font_bitmap->font_bitmap_pointer = font->choseong[0] + font->font_glyph_size * index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jungseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	font_bitmap->font_bitmap_pointer = font->jungseong[0] + font->font_glyph_size * index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jongseong(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	font_bitmap->font_bitmap_pointer = font->jongseong[0] + font->font_glyph_size * index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_font_bitmap_hangul_johab844_jamo_moeum(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	font_bitmap->font_bitmap_pointer = font->jungseong[0] + font->font_glyph_size * index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}

BF_API void bf_get_font_bitmap_hangul_johab844_jamo_jaeum(
	bf_font_hangul_johab844_t* font,
	const bf_uint32_t index,
	bf_font_bitmap_t* font_bitmap
)
{
	//-----------------------------------------------------------------------
	bf_uint8_t* jamo;
	bf_uint32_t jamo_index;


	jamo = BF_NULL_PTR;
	jamo_index = 0u;

	switch (index)
	{
	/*ㄱ*/ case 0x00u: jamo = font->choseong [0]; jamo_index =  0u;      break;
	/*ㄲ*/ case 0x01u: jamo = font->choseong [0]; jamo_index =  1u;      break;
	/*ㄳ*/ case 0x02u: jamo = font->jongseong[0]; jamo_index =  3u - 1u; break;
	/*ㄴ*/ case 0x03u: jamo = font->choseong [0]; jamo_index =  2u;      break;
	/*ㄵ*/ case 0x04u: jamo = font->jongseong[0]; jamo_index =  5u - 1u; break;
	/*ㄶ*/ case 0x05u: jamo = font->jongseong[0]; jamo_index =  6u - 1u; break;
	/*ㄷ*/ case 0x06u: jamo = font->choseong [0]; jamo_index =  3u;      break;
	/*ㄸ*/ case 0x07u: jamo = font->choseong [0]; jamo_index =  4u;      break;
	/*ㄹ*/ case 0x08u: jamo = font->choseong [0]; jamo_index =  5u;      break;
	/*ㄺ*/ case 0x09u: jamo = font->jongseong[0]; jamo_index =  9u - 1u; break;
	/*ㄻ*/ case 0x0Au: jamo = font->jongseong[0]; jamo_index = 10u - 1u; break;
	/*ㄼ*/ case 0x0Bu: jamo = font->jongseong[0]; jamo_index = 11u - 1u; break;
	/*ㄽ*/ case 0x0Cu: jamo = font->jongseong[0]; jamo_index = 12u - 1u; break;
	/*ㄾ*/ case 0x0Du: jamo = font->jongseong[0]; jamo_index = 13u - 1u; break;
	/*ㄿ*/ case 0x0Eu: jamo = font->jongseong[0]; jamo_index = 14u - 1u; break;
	/*ㅀ*/ case 0x0Fu: jamo = font->jongseong[0]; jamo_index = 15u - 1u; break;
	/*ㅁ*/ case 0x10u: jamo = font->choseong [0]; jamo_index =  6u;      break;
	/*ㅂ*/ case 0x11u: jamo = font->choseong [0]; jamo_index =  7u;      break;
	/*ㅃ*/ case 0x12u: jamo = font->choseong [0]; jamo_index =  8u;      break;
	/*ㅄ*/ case 0x13u: jamo = font->jongseong[0]; jamo_index = 18u - 1u; break;
	/*ㅅ*/ case 0x14u: jamo = font->choseong [0]; jamo_index =  9u;      break;
	/*ㅆ*/ case 0x15u: jamo = font->choseong [0]; jamo_index = 10u;      break;
	/*ㅇ*/ case 0x16u: jamo = font->choseong [0]; jamo_index = 11u;      break;
	/*ㅈ*/ case 0x17u: jamo = font->choseong [0]; jamo_index = 12u;      break;
	/*ㅉ*/ case 0x18u: jamo = font->choseong [0]; jamo_index = 13u;      break;
	/*ㅊ*/ case 0x19u: jamo = font->choseong [0]; jamo_index = 14u;      break;
	/*ㅋ*/ case 0x1Au: jamo = font->choseong [0]; jamo_index = 15u;      break;
	/*ㅌ*/ case 0x1Bu: jamo = font->choseong [0]; jamo_index = 16u;      break;
	/*ㅍ*/ case 0x1Cu: jamo = font->choseong [0]; jamo_index = 17u;      break;
	/*ㅎ*/ case 0x1Du: jamo = font->choseong [0]; jamo_index = 18u;      break;
	default:
		return;
	}


	//-----------------------------------------------------------------------
	font_bitmap->font_bitmap_pointer = jamo + font->font_glyph_size * jamo_index;
	font_bitmap->font_bitmap_size = font->font_glyph_size;
	font_bitmap->font_bitmap_stride = font->font_glyph_stride;

	font_bitmap->font_bitmap_cx = font->font_cx;
	font_bitmap->font_bitmap_cy = font->font_cy;
}
