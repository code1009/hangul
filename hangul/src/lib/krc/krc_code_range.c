/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_range.c
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

#include "krc_code_range.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static krc_uint16_t krc_code_range_cp949_hanja_4888(const krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	// 0xCAA1 ~ 0xFDFE : 완성형한글.한자4888자
	// 0xCAFE - 0xCAA1 + 1 = 94자
	// 94(=6*16-1-1)자 * 52개영역 = 4888문자
	//-----------------------------------------------------------------------
	if ((0xCAA1u <= ch) && (ch <= 0xFDFEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_HANJA_4888;
	}

	return KRC_CODE_RANGE_UNKNOWN;
}

static krc_uint16_t krc_code_range_cp949_hangul_51(const krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	// 0xA4A1 ~ 0xA4D3: 완성형한글.특수문자1128자 영역에 포함 됨
	// 0xA4D3 - 0xA4A1 + 1 = 51자
	//-----------------------------------------------------------------------
	if ((0xA4A1u <= ch) && (ch <= 0xA4D3u))
	{
		if ((0xA4A1u <= ch) && (ch <= 0xA4BEu))
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_JAEUM;
		}
		if ((0xA4BFu <= ch) && (ch <= 0xA4D3u))
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_MOEUM;
		}
	}

	return KRC_CODE_RANGE_UNKNOWN;
}

static krc_uint16_t krc_code_range_cp949_special_1128(const krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	// 0xA1A1 ~ 0xACFE: 완성형한글.특수문자1128자(완성형한글.한글11172자.추가8822자.확장B-3108자와 영역과 겹침)
	// 0xA1FE - 0xA1A1 + 1 = 94자
	// 94(=6*16-1-1)자 * 12개영역 = 1128문자
	//-----------------------------------------------------------------------
	/*	
	if ((0xA1A1u <= ch) && (ch <= 0xACFE))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	*/
	//-----------------------------------------------------------------------
	if ((0xA1A1u <= ch) && (ch <= 0xA1FEu))
	{
		//문장 부호, 괄호, 연산자 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA2A1u <= ch) && (ch <= 0xA2FEu))
	{
		//숫자, 로마자 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA3A1u <= ch) && (ch <= 0xA3FEu))
	{
		//라틴 대소문자
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA4A1u <= ch) && (ch <= 0xA4FEu))
	{
		//한글 낱글자 (자음/모음 51자)
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA5A1u <= ch) && (ch <= 0xA5FEu))
	{
		//그리스 문자
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA6A1u <= ch) && (ch <= 0xA6FEu))
	{
		//키릴 문자
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA7A1u <= ch) && (ch <= 0xA7FEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA8A1u <= ch) && (ch <= 0xA8FEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xA9A1u <= ch) && (ch <= 0xA9FEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xAAA1u <= ch) && (ch <= 0xAAFEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xABA1u <= ch) && (ch <= 0xABFEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xACA1u <= ch) && (ch <= 0xACFEu))
	{
		//선문자, 단위 기호 등
		return KRC_CODE_RANGE_SPECIAL_1128;
	}

	return KRC_CODE_RANGE_UNKNOWN;
}

static krc_uint16_t krc_code_range_cp949_hangul_11172(const krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	krc_uint16_t rv;


	//-----------------------------------------------------------------------
	// 완성형한글.한글11172자
	//-----------------------------------------------------------------------
	if ((0xB0A1u <= ch) && (ch <= 0xC8FEu))
	{
		// 2350자(EUC-KR)
		// 94(=6*16-1-1)자 * 25영역 = 2350자
		return KRC_CODE_RANGE_HANGUL_11172;
	}
	if ((0x8141u <= ch) && (ch <= 0xA0FEu))
	{
		// 추가8822자.확장A-5696자
		// 178(=12*16-1-6-6-1)자 * 32영역 = 5696자
		return KRC_CODE_RANGE_HANGUL_11172;
	}
	if ((0xA141u <= ch) && (ch <= 0xC5A0u))
	{
		rv = krc_code_range_cp949_special_1128(ch);
		if (KRC_CODE_RANGE_UNKNOWN != rv)
		{
			return rv;
		}

		// 추가8822자.확장B-3108자
		// 84(=7*16-1-6-6-14-1)자 * 37영역 = 3108자
		return KRC_CODE_RANGE_HANGUL_11172;
	}
	if ((0xC641u <= ch) && (ch <= 0xC652u))
	{
		// 추가8822자.확장C-18자
		// 18자 * 1영역 = 18자
		return KRC_CODE_RANGE_HANGUL_11172;
	}

	return KRC_CODE_RANGE_UNKNOWN;
}

static krc_uint16_t krc_code_range_cp949_ascii(const krc_char16_t ch)
{
	if (ch <= 0x007Fu)
	{
		return KRC_CODE_RANGE_ASCII_LOW;
	}
	else if (ch <= 0x00FFu)
	{
		return KRC_CODE_RANGE_ASCII_HIGH;
	}

	return KRC_CODE_RANGE_UNKNOWN;
}

KRC_API krc_uint16_t krc_code_range_cp949(const krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	krc_uint16_t rv;


	//-----------------------------------------------------------------------
	rv = krc_code_range_cp949_ascii(ch);
	if (KRC_CODE_RANGE_UNKNOWN != rv)
	{
		return rv;
	}


	//-----------------------------------------------------------------------
	rv = krc_code_range_cp949_hangul_51(ch);
	if (KRC_CODE_RANGE_UNKNOWN != rv)
	{
		return rv;
	}


	//-----------------------------------------------------------------------
	/*
	rv = krc_code_range_cp949_special_1128(ch);
	if (KRC_CODE_RANGE_UNKNOWN != rv)
	{
		return rv;
	}
	*/


	//-----------------------------------------------------------------------
	rv = krc_code_range_cp949_hangul_11172(ch);
	if (KRC_CODE_RANGE_UNKNOWN != rv)
	{
		return rv;
	}


	//-----------------------------------------------------------------------
	rv = krc_code_range_cp949_hanja_4888(ch);
	if (KRC_CODE_RANGE_UNKNOWN != rv)
	{
		return rv;
	}


	return KRC_CODE_RANGE_UNKNOWN;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint16_t krc_code_range_unicode(const krc_wchar_t ch)
{
	//-----------------------------------------------------------------------
	if (/*(0x0000u <= ch) &&*/ (ch <= 0x007Fu))
	{
		return KRC_CODE_RANGE_ASCII_LOW;
	}


	//-----------------------------------------------------------------------
	if ((0xAC00u <= ch) && (ch <= 0xD7A3u)) // 0xAC00u+11172자
	{
		return KRC_CODE_RANGE_HANGUL_11172;
	}

	if ((0x1100u <= ch) && (ch <= 0x11FFu))
	{
		if ((0x1100u <= ch) && (ch <= 0x1112u)) // 0x1100u+초성19자
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_CHOSEONG;
		}
		if ((0x1161u <= ch) && (ch <= 0x1175u)) // 0x1161u+중성21자
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_JUNGSEONG;
		}
		if ((0x11A8u <= ch) && (ch <= 0x11C2u)) // 0x11A8u+종성28자
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_JONGSEONG;
		}
	}

	if ((0x3131u <= ch) && (ch <= 0x3163u))
	{
		if ((0x3131u <= ch) && (ch <= 0x314Eu)) // 0x3131u+자음30자
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_JAEUM;
		}
		if ((0x314Fu <= ch) && (ch <= 0x3163u)) // 0x314Fu+모음21자
		{
			return KRC_CODE_RANGE_HANGUL_JAMO_MOEUM;
		}
	}


	//-----------------------------------------------------------------------
	// 0x00A1u '¡' - 0x00FEu 'þ'
	// 0x0111u 'đ' - 0x0451u 'ё'
	// 0x2015u '―' - 0x266Du '♭'
	// 0x3000u '　' - 0x33DDu '㏝'
	// 0xFF01u '！' - 0xFFE6u '￦'
	if ((0x00A1u <= ch) && (ch <= 0x00FEu))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0x0111u <= ch) && (ch <= 0x0451u))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0x2015u <= ch) && (ch <= 0x266Du))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0x3000u <= ch) && (ch <= 0x33DDu))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}
	if ((0xFF01u <= ch) && (ch <= 0xFFE6u))
	{
		return KRC_CODE_RANGE_SPECIAL_1128;
	}


	//-----------------------------------------------------------------------
	// 0x4E00u '一' - 0x9F9Cu '龜'
	// 0xF900u '豈' - 0xF9FFu '刺'
	// 0xFA00u '切' - 0xFA0Bu '廓'
	if ((0x4E00u <= ch) && (ch <= 0x9F9Cu))
	{
		return KRC_CODE_RANGE_HANJA_4888;
	}
	if ((0xF900u <= ch) && (ch <= 0xF9FFu))
	{
		return KRC_CODE_RANGE_HANJA_4888;
	}
	if ((0xFA00u <= ch) && (ch <= 0xFA0Bu))
	{
		return KRC_CODE_RANGE_HANJA_4888;
	}


	return KRC_CODE_RANGE_UNKNOWN;
}
