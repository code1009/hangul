/////////////////////////////////////////////////////////////////////////////
//
// File: bf_char_bitmap.c
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
#include "bf_context.h"

#include "bf_char_bitmap.h"

//===========================================================================
#include "../krc/krc.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_UNICODE_HANGUL_CHOSEONG(char_code)   (((char_code)-0xAC00u)/(21u*28u))
#define BF_UNICODE_HANGUL_JUNGSEONG(char_code) ((((char_code)-0xAC00u)%(21u*28u))/28u)
#define BF_UNICODE_HANGUL_JONGSEONG(char_code)  (((char_code)-0xAC00u)%28u)





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_cp949_bitmap(
	bf_context_t* ctx,
	const bf_uint16_t char_code,
	bf_font_bitmap_t* font_bitmap
)
{
#if ((1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL) || (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA))
	bf_int32_t  code_index;
#endif
	krc_bool_t  char_conversion;
	krc_wchar_t char_unicode;


	bf_uint32_t char_type;

	bf_uint32_t choseong;
	bf_uint32_t jungseong;
	bf_uint32_t jongseong;


	char_type = krc_code_range_cp949(char_code);
	switch (char_type)
	{
		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_ASCII_LOW:
		bf_get_font_bitmap(ctx->font_ascii_low, char_code, font_bitmap);
		return;

	case KRC_CODE_RANGE_ASCII_HIGH:
		break;


		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_HANGUL_11172:
		char_conversion = krc_cp949_to_unicode_hangul_51_11172(char_code, &char_unicode);
		if (KRC_TRUE == char_conversion)
		{
			choseong  = BF_UNICODE_HANGUL_CHOSEONG (char_unicode);
			jungseong = BF_UNICODE_HANGUL_JUNGSEONG(char_unicode);
			jongseong = BF_UNICODE_HANGUL_JONGSEONG(char_unicode);
			bf_get_font_bitmap_hangul_johab844(ctx->font_hangul_johab844, choseong, jungseong, jongseong, font_bitmap, ctx->font_hangul_johab_bitmap_buffer);
			return;
		}
		break;

	case KRC_CODE_RANGE_HANGUL_JAEUM:
		bf_get_font_bitmap_hangul_johab844_jaeum(ctx->font_hangul_johab844, char_code - 0xA4A1u, font_bitmap);
		return;

	case KRC_CODE_RANGE_HANGUL_MOEUM:
		bf_get_font_bitmap_hangul_johab844_moeum(ctx->font_hangul_johab844, char_code - 0xA4BFu, font_bitmap);
		return;


		//-----------------------------------------------------------------------
#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
	case KRC_CODE_RANGE_SPECIAL_1128:
		code_index = krc_cp949_index_special_1128(char_code);
		if (code_index >= 0)
		{
			bf_get_font_bitmap(ctx->font_cp949_special_1128, code_index, font_bitmap);
			return;
		}
		break;
#endif

		//-----------------------------------------------------------------------
#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
	case KRC_CODE_RANGE_HANJA_4888:
		code_index = krc_cp949_index_hanja_4888(char_code);
		if (code_index >= 0)
		{
			bf_get_font_bitmap(ctx->font_cp949_hanja_4888, code_index, font_bitmap);
			return;
		}
		break;
#endif

		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_UNKNOWN:
	default:
		break;
	}


	//-----------------------------------------------------------------------
	bf_get_font_bitmap(ctx->font_unknown, 0u, font_bitmap);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_get_unicode_bitmap(
	bf_context_t* ctx,
	const bf_uint16_t char_unicode,
	bf_font_bitmap_t* font_bitmap
)
{
#if ((1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL) || (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA))
	bf_int32_t   code_index;

	krc_char16_t char_code;
	krc_bool_t   char_conversion;
#endif

	bf_uint32_t char_type;

	bf_uint32_t choseong;
	bf_uint32_t jungseong;
	bf_uint32_t jongseong;


	char_type = krc_code_range_unicode(char_unicode);
	switch (char_type)
	{
		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_ASCII_LOW:
		bf_get_font_bitmap(ctx->font_ascii_low, char_unicode, font_bitmap);
		return;

	case KRC_CODE_RANGE_ASCII_HIGH:
		break;


		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_HANGUL_11172:
		choseong  = BF_UNICODE_HANGUL_CHOSEONG (char_unicode);
		jungseong = BF_UNICODE_HANGUL_JUNGSEONG(char_unicode);
		jongseong = BF_UNICODE_HANGUL_JONGSEONG(char_unicode);
		bf_get_font_bitmap_hangul_johab844(ctx->font_hangul_johab844, choseong, jungseong, jongseong, font_bitmap, ctx->font_hangul_johab_bitmap_buffer);
		return;

	case KRC_CODE_RANGE_HANGUL_JAEUM:
		bf_get_font_bitmap_hangul_johab844_jaeum(ctx->font_hangul_johab844, char_unicode - 0x3131u, font_bitmap);
		return;

	case KRC_CODE_RANGE_HANGUL_MOEUM:
		bf_get_font_bitmap_hangul_johab844_moeum(ctx->font_hangul_johab844, char_unicode - 0x314Fu, font_bitmap);
		return;


		//-----------------------------------------------------------------------
#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
	case KRC_CODE_RANGE_SPECIAL_1128:
		char_conversion = krc_unicode_to_cp949_special_1128(char_unicode, &char_code);
		if (KRC_TRUE == char_conversion)
		{
			code_index = krc_cp949_index_special_1128(char_code);
			if (code_index >= 0)
			{
				bf_get_font_bitmap(ctx->font_cp949_special_1128, code_index, font_bitmap);
				return;
			}
		}
		break;
#endif


		//-----------------------------------------------------------------------
#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
	case KRC_CODE_RANGE_HANJA_4888:
		char_conversion = krc_unicode_to_cp949_hanja_4888(char_unicode, &char_code);
		if (KRC_TRUE == char_conversion)
		{
			code_index = krc_cp949_index_hanja_4888(char_code);
			if (code_index >= 0)
			{
				bf_get_font_bitmap(ctx->font_cp949_hanja_4888, code_index, font_bitmap);
				return;
			}
		}
		break;
#endif


		//-----------------------------------------------------------------------
	case KRC_CODE_RANGE_UNKNOWN:
	default:
		break;
	}


	//-----------------------------------------------------------------------
	bf_get_font_bitmap(ctx->font_unknown, 0u, font_bitmap);
}
