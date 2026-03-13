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

	if (/*(0U <= wc) &&*/ (wc <= 0x7Fu))
	{
		if (1u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(wc);

			return 1u;
		}
	}
	else if ((0x80u <= wc) && (wc <= 0x7FFu))
	{
		if (2u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xC0u | (wc >> 6u));
			*(utf8_pointer + 1) = (krc_char_t)(0x80u | (wc & 0x3Fu));

			return 2u;
		}
	}
	else if (0x800u <= wc && wc <= 0xFFFFu)
	{
		if (3u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xE0u |  (wc >> 12u));
			*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((wc >>  6u) & 0x3Fu));
			*(utf8_pointer + 2) = (krc_char_t)(0x80u |  (wc         & 0x3Fu));

			return 3u;
		}
	}
	else if (0x10000u <= wc && wc <= 0x1FFFFFu)
	{
		if (4u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xF0u |  (wc >> 18u));
			*(utf8_pointer + 1) = (krc_char_t)(0x80u | ((wc >> 12u) & 0x3Fu));
			*(utf8_pointer + 2) = (krc_char_t)(0x80u | ((wc >>  6u) & 0x3Fu));
			*(utf8_pointer + 3) = (krc_char_t)(0x80u |  (wc         & 0x3Fu));

			return 4;
		}
	}
	else if (0x200000u <= wc && wc <= 0x3FFFFFFu)
	{
		if (5u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xF8 |  (wc >> 24u));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 18u) & 0x3Fu));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | ((wc >> 12u) & 0x3Fu));
			*(utf8_pointer + 3) = (krc_char_t)(0x80 | ((wc >>  6u) & 0x3Fu));
			*(utf8_pointer + 4) = (krc_char_t)(0x80 |  (wc         & 0x3Fu));

			return 5;
		}
	}
	else if (0x4000000u <= wc && wc <= 0x7FFFFFFFu)
	{
		if (6u <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xfc |  (wc >> 30u));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 24u) & 0x3Fu));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | ((wc >> 18u) & 0x3Fu));
			*(utf8_pointer + 3) = (krc_char_t)(0x80 | ((wc >> 12u) & 0x3Fu));
			*(utf8_pointer + 4) = (krc_char_t)(0x80 | ((wc >>  6u) & 0x3Fu));
			*(utf8_pointer + 5) = (krc_char_t)(0x80 |  (wc         & 0x3Fu));

			return 6;
		}
	}

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
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
			wc = (p[0] & 0x1F) << 6;
			wc |= (p[1] & 0x3F);
			result = 2;
		}
	}
	else if ((c & 0xF0) == 0xE0)
	{
		if (3 <= utf8_size)
		{
			wc = (p[0] & 0xF) << 12;
			wc |= (p[1] & 0x3F) << 6;
			wc |= (p[2] & 0x3F);
			result = 3;
		}
	}
	else if ((c & 0xF8) == 0xF0)
	{
		if (4 <= utf8_size)
		{
			wc = (p[0] & 0x7) << 18;
			wc |= (p[1] & 0x3F) << 12;
			wc |= (p[2] & 0x3F) << 6;
			wc |= (p[3] & 0x3F);
			result = 4;
		}
	}
	else if ((c & 0xFC) == 0xF8)
	{
		if (5 <= utf8_size)
		{
			wc = (p[0] & 0x3) << 24;
			wc |= (p[1] & 0x3F) << 18;
			wc |= (p[2] & 0x3F) << 12;
			wc |= (p[3] & 0x3F) << 6;
			wc |= (p[4] & 0x3F);
			result = 5;
		}
	}
	else if ((c & 0xFE) == 0xFC)
	{
		if (6 <= utf8_size)
		{
			wc = (p[0] & 0x1) << 30;
			wc |= (p[1] & 0x3F) << 24;
			wc |= (p[2] & 0x3F) << 18;
			wc |= (p[3] & 0x3F) << 12;
			wc |= (p[4] & 0x3F) << 6;
			wc |= (p[5] & 0x3F);
			result = 6;
		}
	}

	*unicode = wc;

	return result;
}


