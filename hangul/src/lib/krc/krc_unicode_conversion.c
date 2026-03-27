/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_conversion.c
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
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
/*
# Ref. 
RFC 3629/유니코드 표준
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
#define KRC_UTF8_CONTINUATION_MIN 0x80u
#define KRC_UTF8_CONTINUATION_MAX 0xBFu

#define KRC_UTF8_IS_1BYTE_START(ch) (((ch) & 0x80u) == 0x00u)
#define KRC_UTF8_IS_2BYTE_START(ch) (((ch) & 0xE0u) == 0xC0u)
#define KRC_UTF8_IS_3BYTE_START(ch) (((ch) & 0xF0u) == 0xE0u)
#define KRC_UTF8_IS_4BYTE_START(ch) (((ch) & 0xF8u) == 0xF0u)

#define KRC_UTF8_IN_1BYTE_RANGE(ch) ((ch) <= 0x7Fu)
#define KRC_UTF8_IN_2BYTE_RANGE(ch) ((ch) <= 0x7FFu)
#define KRC_UTF8_IN_3BYTE_RANGE(ch) ((ch) <= 0xFFFFu)
#define KRC_UTF8_IN_4BYTE_RANGE(ch) ((ch) <= 0x10FFFFu)
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_utf8_continuation(const krc_uchar8_t c)
{
	if (0x80u <= c && c <= 0xBFu)
	{
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

KRC_API krc_size_t krc_utf8_char_size(const krc_char_t* utf8_pointer)
{
	const krc_uchar8_t* p = (const krc_uchar8_t*)utf8_pointer;


	if      ((p[0] & 0x80u) == 0x00u)
	{
		return 1u;
	}
	else if ((p[0] & 0xE0u) == 0xC0u)
	{
		if (KRC_TRUE == krc_utf8_continuation(p[1]))
		{
			return 2u;
		}
	}
	else if ((p[0] & 0xF0u) == 0xE0u)
	{
		if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
			(KRC_TRUE == krc_utf8_continuation(p[2])))
		{
			return 3u;
		}
	}
	else if ((p[0] & 0xF8u) == 0xF0u)
	{
		if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
			(KRC_TRUE == krc_utf8_continuation(p[2])) &&
			(KRC_TRUE == krc_utf8_continuation(p[3])))
		{
			return 4u;
		}
	}

	return 0u;
}

KRC_API krc_size_t krc_utf8l_char_size(const krc_char_t* utf8_pointer, const krc_size_t utf8_size)
{
	const krc_uchar8_t* p = (const krc_uchar8_t*)utf8_pointer;


	if      (1u <= utf8_size)
	{
		if ((p[0] & 0x80u) == 0x00u)
		{
			return 1u;
		}
	}
	else if (2u <= utf8_size)
	{
		if ((p[0] & 0xE0u) == 0xC0u)
		{
			if (KRC_TRUE == krc_utf8_continuation(p[1]))
			{
				return 2u;
			}
		}
	}
	else if (3u <= utf8_size)
	{
		if ((p[0] & 0xF0u) == 0xE0u)
		{
			if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
				(KRC_TRUE == krc_utf8_continuation(p[2])))
			{
				return 3u;
			}
		}
	}
	else if (4u <= utf8_size)
	{
		if ((p[0] & 0xF8u) == 0xF0u)
		{
			if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
				(KRC_TRUE == krc_utf8_continuation(p[2])) &&
				(KRC_TRUE == krc_utf8_continuation(p[3])))
			{
				return 4u;
			}
		}
	}
	return 0u;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_unicode_to_utf8_char(const krc_uchar32_t unicode, krc_char_t* utf8_pointer)
{
	if      ((unicode <= 0x7Fu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(unicode);
		return 1;
	}
	else if ((unicode <= 0x7FFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xC0u | (unicode >> 6u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 2;
	}
	else if ((unicode <= 0xFFFFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xE0u | (unicode >> 12u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >> 6u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 3;
	}
	else if ((unicode <= 0x10FFFFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xF0u | (unicode >> 18u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >> 12u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u | ((unicode >> 6u) & 0x3Fu));
		*(utf8_pointer + 3) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 4;
	}
	return 0;
}

KRC_API krc_size_t krc_unicode_to_utf8l_char(const krc_uchar32_t unicode, krc_char_t* utf8_pointer, const krc_size_t utf8_size)
{
	if     ((1u <= utf8_size) && (unicode <= 0x7Fu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(unicode);
		return 1;
	}
	else if ((2u <= utf8_size) && (unicode <= 0x7FFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xC0u | (unicode >> 6u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 2;
	}
	else if ((3u <= utf8_size) && (unicode <= 0xFFFFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xE0u |  (unicode >> 12u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >>  6u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u | (unicode & 0x3Fu));
		return 3;
	}
	else if ((4u <= utf8_size) && (unicode <= 0x10FFFFu))
	{
		*(utf8_pointer + 0) = (krc_char_t)(0xF0u |  (unicode >> 18u));
		*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((unicode >> 12u) & 0x3Fu));
		*(utf8_pointer + 2) = (krc_char_t)(0x80u | ((unicode >>  6u) & 0x3Fu));
		*(utf8_pointer + 3) = (krc_char_t)(0x80u |  (unicode & 0x3Fu));
		return 4;
	}
	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_utf8_to_unicode_char(const krc_char_t* utf8_pointer, krc_uchar32_t* unicode)
{
	const krc_uchar8_t* p = (const krc_uchar8_t*)utf8_pointer;


	if      (((p[0] & 0x80u) == 0x00u))
	{
		*unicode = p[0];
		return 1u;
	}
	else if (((p[0] & 0xE0u) == 0xC0u))
	{
		if (KRC_TRUE == krc_utf8_continuation(p[1]))
		{
			*unicode = (p[0] & 0x1Fu) << 6u;
			*unicode |= (p[1] & 0x3Fu);
			return 2u;
		}
	}
	else if (((p[0] & 0xF0u) == 0xE0u))
	{
		if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
			(KRC_TRUE == krc_utf8_continuation(p[2])))
		{
			*unicode = (p[0] & 0xFu) << 12u;
			*unicode |= (p[1] & 0x3Fu) << 6u;
			*unicode |= (p[2] & 0x3Fu);
			return 3u;
		}
	}
	else if (((p[0] & 0xF8u) == 0xF0u))
	{
		if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
			(KRC_TRUE == krc_utf8_continuation(p[2])) &&
			(KRC_TRUE == krc_utf8_continuation(p[3])))
		{
			*unicode = (p[0] & 0x7u) << 18u;
			*unicode |= (p[1] & 0x3Fu) << 12u;
			*unicode |= (p[2] & 0x3Fu) << 6u;
			*unicode |= (p[3] & 0x3Fu);
			return 4u;
		}
	}
	*unicode = 0u;
	return 0u;
}

KRC_API krc_size_t krc_utf8l_to_unicode_char(const krc_char_t* utf8_pointer, const krc_size_t utf8_size, krc_uchar32_t* unicode)
{
	const krc_uchar8_t* p = (const krc_uchar8_t*)utf8_pointer;


	if      (1u <= utf8_size)
	{
		if ((p[0] & 0x80u) == 0x00u)
		{
			*unicode = p[0];
			return 1u;
		}
	}
	else if (2u <= utf8_size)
	{
		if ((p[0] & 0xE0u) == 0xC0u)
		{
			if (KRC_TRUE == krc_utf8_continuation(p[1]))
			{
				*unicode = (p[0] & 0x1Fu) << 6u;
				*unicode |= (p[1] & 0x3Fu);
				return 2u;
			}
		}
	}
	else if (3u <= utf8_size)
	{
		if ((p[0] & 0xF0u) == 0xE0u)
		{
			if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
				(KRC_TRUE == krc_utf8_continuation(p[2])))
			{
				*unicode = (p[0] & 0xFu) << 12u;
				*unicode |= (p[1] & 0x3Fu) << 6u;
				*unicode |= (p[2] & 0x3Fu);
				return 3u;
			}
		}
	}
	else if (4u <= utf8_size)
	{
		if ((p[0] & 0xF8u) == 0xF0u)
		{
			if ((KRC_TRUE == krc_utf8_continuation(p[1])) &&
				(KRC_TRUE == krc_utf8_continuation(p[2])) &&
				(KRC_TRUE == krc_utf8_continuation(p[3])))
			{
				*unicode = (p[0] & 0x7u) << 18u;
				*unicode |= (p[1] & 0x3Fu) << 12u;
				*unicode |= (p[2] & 0x3Fu) << 6u;
				*unicode |= (p[3] & 0x3Fu);
				return 4u;
			}
		}
	}
	*unicode = 0u;
	return 0u;
}
