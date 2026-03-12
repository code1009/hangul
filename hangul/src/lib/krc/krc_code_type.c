/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_type.c
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "krc_config.h"
#include "krc_type.h"

#include "krc_api.h"
#include "krc_code_type.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint32_t krc_code_type_cp949(krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	if (0x0000u <= ch && ch <= 0x007Fu)
	{
		return KRC_CODE_TYPE_ASCII_LOW;
	}

	if (0x0080u <= ch && ch <= 0x00FFu)
	{
		return KRC_CODE_TYPE_ASCII_HIGH;
	}


	//-----------------------------------------------------------------------
	if (0xA4A1u <= ch && ch <= 0xA4BEu)
	{
		return KRC_CODE_TYPE_HANGUL_JAMO_JAEUM;
	}
	if (0xA4BFu <= ch && ch <= 0xA4D3u)
	{
		return KRC_CODE_TYPE_HANGUL_JAMO_MOEUM;
	}


	//-----------------------------------------------------------------------
	if (ch >= 0xB0A1u && ch <= 0xC8FEu)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0x8141u && ch <= 0xA0FEu)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0xA141u && ch <= 0xC5A0u)
	{
		//-------------------------------------------------------------------
		if (0xA1A1u <= ch && ch <= 0xACFEu)
		{
			return KRC_CODE_TYPE_SPECIAL_1128;
		}

		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0xC641u && ch <= 0xC652u)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}


	//-----------------------------------------------------------------------
	if (0xA1A1u <= ch && ch <= 0xACFEu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}

	if (0xCAA1u <= ch && ch <= 0xFDFEu)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}


	return KRC_CODE_TYPE_UNKNOWN;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint32_t krc_code_type_unicode(krc_wchar_t ch)
{
	//-----------------------------------------------------------------------
	if (0x0000u <= ch && ch <= 0x007Fu)
	{
		return KRC_CODE_TYPE_ASCII_LOW;
	}

	if (0x0080u <= ch && ch <= 0x00FFu)
	{
		return KRC_CODE_TYPE_ASCII_HIGH;
	}


	//-----------------------------------------------------------------------
	if (0xAC00u <= ch && ch <= 0xD7A3u) // 0xAC00u+11172자
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}

	if (0x1100u <= ch && ch <= 0x11FFu)
	{
		if (0x1100u <= ch && ch <= 0x1112u) // 0x1100u+초성19자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_CHOSEONG;
		}
		if (0x1161u <= ch && ch <= 0x1175u) // 0x1161u+중성21자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JUNGSEONG;
		}
		if (0x11A8u <= ch && ch <= 0x11C2u) // 0x11A8u+종성28자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JONGSEONG;
		}
	}

	if (0x3131u <= ch && ch <= 0x3163u)
	{
		if (0x3131u <= ch && ch <= 0x314Eu) // 0x3131u+자음30자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JAEUM;
		}
		if (0x314Fu <= ch && ch <= 0x3163u) // 0x314Fu+모음21자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_MOEUM;
		}
	}


	//-----------------------------------------------------------------------
	// 0x00A1u '¡' - 0x00FEu 'þ'
	// 0x0111u 'đ' - 0x0451u 'ё'
	// 0x2015u '―' - 0x266Du '♭'
	// 0x3000u '　' - 0x33DDu '㏝'
	// 0xFF01u '！' - 0xFFE6u '￦'
	if (0x00A1u <= ch && ch <= 0x00FEu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x0111u <= ch && ch <= 0x0451u)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x2015u <= ch && ch <= 0x266Du)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x3000u <= ch && ch <= 0x33DDu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0xFF01u <= ch && ch <= 0xFFE6u)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}


	//-----------------------------------------------------------------------
	// 0x4E00u '一' - 0x9F9Cu '龜'
	// 0xF900u '豈' - 0xF9FFu '刺'
	// 0xFA00u '切' - 0xFA0Bu '廓'
	if (0x4E00u <= ch && ch <= 0x9F9Cu)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}
	if (0xF900u <= ch && ch <= 0xF9FFu)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}
	if (0xFA00u <= ch && ch <= 0xFA0Bu)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}


	return KRC_CODE_TYPE_UNKNOWN;
}
