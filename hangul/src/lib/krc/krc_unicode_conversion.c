/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_conversion.c
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

#include "krc_unicode_conversion.h"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_unicode_to_utf8_char(const krc_wchar32_t unicode, krc_char_t* utf8_pointer, const krc_size_t utf8_size)
{
	krc_wchar32_t wc;


	wc = unicode;

	if (/*(0 <= wc) &&*/ (wc <= 0x7f))
	{
		if (1 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(wc);

			return 1;
		}
	}
	else if ((0x80 <= wc) && (wc <= 0x7ff))
	{
		if (2 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xc0 | (wc >> 6));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 2;
		}
	}
	else if ((0x800 <= wc) && (wc <= 0xffff))
	{
		if (3 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xe0 | (wc >> 12));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 6) & 0x3f));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 3;
		}
	}
	else if ((0x10000 <= wc) && (wc <= 0x1fffff))
	{
		if (4 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xf0 | (wc >> 18));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 12) & 0x3f));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | ((wc >> 6) & 0x3f));
			*(utf8_pointer + 3) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 4;
		}
	}
	/*
	else if ((0x200000 <= wc) && (wc <= 0x3ffffff))
	{
		if (5 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xf8 | (wc >> 24));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 18) & 0x3f));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | ((wc >> 12) & 0x3f));
			*(utf8_pointer + 3) = (krc_char_t)(0x80 | ((wc >> 6) & 0x3f));
			*(utf8_pointer + 4) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 5;
		}
	}
	else if ((0x4000000 <= wc) && (wc <= 0x7fffffff))
	{
		if (6 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xfc | (wc >> 30));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 24) & 0x3f));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | ((wc >> 18) & 0x3f));
			*(utf8_pointer + 3) = (krc_char_t)(0x80 | ((wc >> 12) & 0x3f));
			*(utf8_pointer + 4) = (krc_char_t)(0x80 | ((wc >> 6) & 0x3f));
			*(utf8_pointer + 5) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 6;
		}
	}
	*/

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static krc_bool_t krc_utf8_is_valid_continuation_byte(const krc_char8_t c)
{
	if (0x80u <= c && c <= 0xBFu)
	{
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

KRC_API krc_size_t krc_utf8_to_unicode_char(const krc_char_t* utf8_pointer, const krc_size_t utf8_size, krc_wchar32_t* unicode)
{
	krc_size_t result;


	result = 0;


	if (utf8_size <= 0)
	{
		*unicode = 0;
		return result;
	}



	krc_char8_t* p;
	krc_char8_t c;
	krc_wchar32_t wc;


	p = (krc_char8_t*)utf8_pointer;
	c = *p;
	wc = 0;


	if ((c & 0x80) == 0)
	{
		if (1 <= utf8_size)
		{
			wc = c;
			result = 1;
		}
	}
	else if ((c & 0xE0) == 0xC0)
	{
		if (2 <= utf8_size)
		{
			if (KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[1]))
			{
				wc = (p[0] & 0x1F) << 6;
				wc |= (p[1] & 0x3F);
				result = 2;
			}
		}
	}
	else if ((c & 0xF0) == 0xE0)
	{
		if (3 <= utf8_size)
		{
			if ((KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[1])) && (KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[2])))
			{
				wc = (p[0] & 0xF) << 12;
				wc |= (p[1] & 0x3F) << 6;
				wc |= (p[2] & 0x3F);
				result = 3;
			}
		}
	}
	else if ((c & 0xF8) == 0xF0)
	{
		if (4 <= utf8_size)
		{
			if ((KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[1])) && (KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[2])) && (KRC_TRUE == krc_utf8_is_valid_continuation_byte(p[3])))
			{
				wc = (p[0] & 0x7) << 18;
				wc |= (p[1] & 0x3F) << 12;
				wc |= (p[2] & 0x3F) << 6;
				wc |= (p[3] & 0x3F);
				result = 4;
			}
		}
	}
	// RFC 3629: 5바이트 및 6바이트 시퀀스는 유효한 UTF-8이 아님
	// Unicode 최대 코드포인트 U+10FFFF는 4바이트로 표현되므로 5~6바이트는 항상 오류 처리
	

	*unicode = wc;

	return result;
}


