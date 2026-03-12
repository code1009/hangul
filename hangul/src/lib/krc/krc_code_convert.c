/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_convert.c
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
#include "krc_code_convert.h"

#include "krc_cp949_index.h"
#include "krc_code_table.h"
#include "krc_code_range.h"





/////////////////////////////////////////////////////////////////////////////
// 
// cp949_to_unicode 
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_cp949_to_unicode_hangul_51_11172(const krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int16_t index;

	
	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if ((0xA4A1u <= cp949) && (cp949 <= 0xA4D3u))
	{
		*unicode = 0x3131u + cp949-0xA4A1u;
		return KRC_TRUE;
	}


	index = krc_cp949_index_hangul_11172(cp949);
	if (0 > index)
	{
		return KRC_FALSE;
	}

	*unicode = _krc_cp949_to_unicode_hangul_11172[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_cp949_to_unicode_special_1128(const krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int16_t index;


	index = krc_cp949_index_special_1128(cp949);
	if (0 > index)
	{
		return KRC_FALSE;
	}

	*unicode = _krc_cp949_to_unicode_special_1128[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_cp949_to_unicode_hanja_4888(const krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int16_t index;


	index = krc_cp949_index_hanja_4888(cp949);
	if (0 > index)
	{
		return KRC_FALSE;
	}

	*unicode = _krc_cp949_to_unicode_hanja_4888[index];

	return KRC_TRUE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// unicode_to_cp949
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_unicode_to_cp949_hangul_51_11172(krc_wchar_t unicode, krc_char16_t* cp949)
{
	krc_int16_t index;


	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if ((0x3131u <= unicode) && (unicode <= 0x3163u))
	{
		*cp949 = 0xA4A1u + unicode-0x3131u;
		return KRC_TRUE;
	}


	// 유니코드 한글 11172자: 0xAC00u '가' - 0xD7A3u '힣'
	if ((0xAC00u <= unicode) && (unicode <= 0xD7A3u))
	{
		index = unicode - 0xAC00u;
		*cp949 = _krc_unicode_to_cp949_hangul_11172[index];
		return KRC_TRUE;
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_unicode_to_cp949_special_1128(krc_wchar_t unicode, krc_char16_t* cp949)
{
	if (KRC_CODE_RANGE_SPECIAL_1128 != krc_code_range_unicode(unicode))
	{
		return KRC_FALSE;
	}


	krc_int16_t index;


	// 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	for (index=0; index<1128; index++)
	{
		if (unicode == _krc_cp949_to_unicode_special_1128[index])
		{
			*cp949 = 0xA1A1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_unicode_to_cp949_hanja_4888(krc_wchar_t unicode, krc_char16_t* cp949)
{
	if (KRC_CODE_RANGE_HANJA_4888 != krc_code_range_unicode(unicode))
	{
		return KRC_FALSE;
	}


	krc_int16_t index;


	// 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	for (index=0; index<4888; index++)
	{
		if (unicode == _krc_cp949_to_unicode_hanja_4888[index])
		{
			*cp949 = 0xCAA1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}
