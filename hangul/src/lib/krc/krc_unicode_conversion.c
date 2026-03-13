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
#define KRC_UTF8_1BYTE_MAX  0x7Fu
#define KRC_UTF8_2BYTE_MAX  0x7FFu
#define KRC_UTF8_3BYTE_MAX  0xFFFFu
#define KRC_UTF8_4BYTE_MAX  0x10FFFFu





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_unicode_to_utf8_char(const krc_wchar32_t unicode, krc_char_t* utf8_pointer, const krc_size_t utf8_size)
{
	if      ((unicode <= KRC_UTF8_1BYTE_MAX) && (1u <= utf8_size))
	{
		*(utf8_pointer + 0) = (krc_char_t)(unicode);
		return 1u;
	}
	else if ((unicode <= KRC_UTF8_2BYTE_MAX) && (2u <= utf8_size))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xC0u | (unicode >> 6u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 2u;
	}
	else if ((unicode <= KRC_UTF8_3BYTE_MAX) && (3u <= utf8_size))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xE0u |  (unicode >> 12u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >>  6u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u |  (unicode         & 0x3Fu));
		return 3u;
	}
	else if ((unicode <= KRC_UTF8_4BYTE_MAX) && (4u <= utf8_size))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xF0u |  (unicode >> 18u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >> 12u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u | ((unicode >>  6u) & 0x3Fu));
		*(utf8_pointer + 3) = (krc_char_t)(0x80u |  (unicode         & 0x3Fu));
		return 4;
	}

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_utf8_to_unicode_char(const krc_char_t* utf8_pointer, const krc_size_t utf8_size, krc_wchar32_t* unicode)
{
	krc_char8_t* p = (krc_char8_t*)utf8_pointer;
	krc_wchar32_t wc;


	if      (((p[0] & 0x80u) == 0x00u) && (1u <= utf8_size))
	{
		wc = p[0];
		*unicode = wc;
		return 1;
	}
	else if (((p[0] & 0xE0u) == 0xC0u) && (2u <= utf8_size))
	{
		if ((p[1] & 0xC0u) != 0x80u)
		{
			return 0;
		}

		wc  = (p[0] & 0x1Fu) << 6;
		wc |= (p[1] & 0x3Fu);
		*unicode = wc;
		return 2;
	}
	else if (((p[0] & 0xF0u) == 0xE0u) && (3u <= utf8_size))
	{
		if (((p[1] & 0xC0u) != 0x80u) || 
			((p[2] & 0xC0u) != 0x80u) )
		{
			return 0;
		}

		wc  = (p[0] & 0x0Fu) << 12;
		wc |= (p[1] & 0x3Fu) << 6;
		wc |= (p[2] & 0x3Fu);
		*unicode = wc;
		return 3;
	}
	else if (((p[0] & 0xF8u) == 0xF0u) && (4u <= utf8_size))
	{
		if (((p[1] & 0xC0u) != 0x80u) || 
			((p[2] & 0xC0u) != 0x80u) || 
			((p[3] & 0xC0u) != 0x80u) )
		{
			return 0;
		}

		wc  = (p[0] & 0x07u) << 18;
		wc |= (p[1] & 0x3Fu) << 12;
		wc |= (p[2] & 0x3Fu) << 6;
		wc |= (p[3] & 0x3Fu);
		*unicode = wc;
		return 4;
	}

	*unicode = 0;
	return 0;
}


