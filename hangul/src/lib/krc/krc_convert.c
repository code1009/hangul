/////////////////////////////////////////////////////////////////////////////
//
// File: krc_convert.c
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
#include "krc_convert.h"

#include "krc_code_convert.h"
#include "krc_stream.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_unicode_to_cp949(krc_wchar_t* wcs_string, krc_int32_t wcs_length, krc_char_t* mbcs_string, krc_int32_t mbcs_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, mbcs_string, mbcs_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;

	
	krc_wchar_t* src;
	krc_int32_t index;
	krc_int32_t count;


	src = wcs_string;
	count = wcs_length;
	for (index = 0; index < count; index++)
	{
		wcs = *(src + index);

		
		if (wcs == 0x00u)
		{
			krc_mbcs_ostream_put_char8(&o, 0u);
			return o.length;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			krc_mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			krc_mbcs_ostream_put_char8(&o, ch1);
		}

		else if (krc_unicode_to_cp949_hangul_51_11172(wcs, &mbcs) == KRC_TRUE)
		{		
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_unicode_to_cp949_special_1128(wcs, &mbcs) == KRC_TRUE)
		{		
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_unicode_to_cp949_hanja_4888(wcs, &mbcs) == KRC_TRUE)
		{		
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else
		{
			krc_mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}
	krc_mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}

//===========================================================================
KRC_API krc_int32_t krc_cp949_to_unicode(krc_char_t* mbcs_string, krc_int32_t mbcs_length, krc_wchar_t* wcs_string, krc_int32_t wcs_length)
{
	krc_wcs_ostream_t o;


	krc_wcs_ostream_init(&o, wcs_string, wcs_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	krc_char8_t  ch2;

	krc_char_t* src;
	krc_int32_t index;
	krc_int32_t count;


	src = mbcs_string;
	count = mbcs_length;
	for (index = 0; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (ch1 == 0x00u)
		{
			krc_wcs_ostream_put_wchar(&o, 0u);
			return o.length;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			krc_wcs_ostream_put_wchar(&o, ch1);
		}
		else
		{
			if ((index + 1) <= count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1));
				mbcs = (ch1 << 8u) | ch2;


				if (krc_cp949_to_unicode_hangul_51_11172(mbcs, &wcs) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_cp949_to_unicode_special_1128(mbcs, &wcs) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_cp949_to_unicode_hanja_4888(mbcs, &wcs) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else
				{
					krc_wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
				}
			}
			else
			{
				krc_wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
			}
		}
	}
	krc_wcs_ostream_put_wchar(&o, 0u);


	return o.length;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static krc_int32_t utf8_to_unicode(krc_char_t* utf8_pointer, krc_int32_t utf8_size, krc_wchar32_t* unicode_pointer)
{
	krc_int32_t result;


	result = 0;


	if (utf8_size <= 0)
	{
		*unicode_pointer = 0;
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
		if (1<=utf8_size)
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

	*unicode_pointer = wc;

	return result;
}

static krc_int32_t unicode_to_utf8(krc_wchar32_t unicode, krc_char_t* utf8_pointer, krc_int32_t utf8_size)
{
	krc_wchar32_t wc;


	wc = unicode;

	if (0 <= wc && wc <= 0x7f)
	{
		if (1 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(wc);

			return 1;
		}
	}
	else if (0x80 <= wc && wc <= 0x7ff)
	{
		if (2 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xc0 | (wc >> 6));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 2;
		}
	}
	else if (0x800 <= wc && wc <= 0xffff)
	{
		if (3 <= utf8_size)
		{
			*(utf8_pointer + 0) = (krc_char_t)(0xe0 | (wc >> 12));
			*(utf8_pointer + 1) = (krc_char_t)(0x80 | ((wc >> 6) & 0x3f));
			*(utf8_pointer + 2) = (krc_char_t)(0x80 | (wc & 0x3f));

			return 3;
		}
	}
	else if (0x10000 <= wc && wc <= 0x1fffff)
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
	else if (0x200000 <= wc && wc <= 0x3ffffff)
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
	else if (0x4000000 <= wc && wc <= 0x7fffffff)
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

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static krc_mbcs_ostream_put_utf8_from_unicode(krc_mbcs_ostream_t* o, krc_wchar_t wcs)
{
	krc_char_t buffer[6];
	krc_int32_t wsize;


	wsize = unicode_to_utf8(wcs, buffer, 6);
	switch (wsize)
	{
	case 1:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		break;

	case 2:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		krc_mbcs_ostream_put_char8(o, buffer[1]);
		break;

	case 3:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		krc_mbcs_ostream_put_char8(o, buffer[1]);
		krc_mbcs_ostream_put_char8(o, buffer[2]);
		break;

	case 4:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		krc_mbcs_ostream_put_char8(o, buffer[1]);
		krc_mbcs_ostream_put_char8(o, buffer[2]);
		krc_mbcs_ostream_put_char8(o, buffer[3]);
		break;

	case 5:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		krc_mbcs_ostream_put_char8(o, buffer[1]);
		krc_mbcs_ostream_put_char8(o, buffer[2]);
		krc_mbcs_ostream_put_char8(o, buffer[3]);
		krc_mbcs_ostream_put_char8(o, buffer[4]);
		break;

	case 6:
		krc_mbcs_ostream_put_char8(o, buffer[0]);
		krc_mbcs_ostream_put_char8(o, buffer[1]);
		krc_mbcs_ostream_put_char8(o, buffer[2]);
		krc_mbcs_ostream_put_char8(o, buffer[3]);
		krc_mbcs_ostream_put_char8(o, buffer[4]);
		krc_mbcs_ostream_put_char8(o, buffer[5]);
		break;

	default:
		krc_mbcs_ostream_put_char8(o, 0x3Fu); // ?
		break;
	}
}

KRC_API krc_int32_t krc_cp949_to_utf8(krc_char_t* cp949_string, krc_int32_t cp949_length, krc_char_t* utf8_string, krc_int32_t utf8_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, utf8_string, utf8_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	krc_char8_t  ch2;

	krc_char_t* src;
	krc_int32_t index;
	krc_int32_t count;


	src = cp949_string;
	count = cp949_length;
	for (index = 0; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (ch1 == 0x00u)
		{
			krc_mbcs_ostream_put_char8(&o, 0u);
			return o.length;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			krc_mbcs_ostream_put_char8(&o, ch1);
		}
		else
		{
			if ((index + 1) <= count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1));
				mbcs = (ch1 << 8u) | ch2;


				if (krc_cp949_to_unicode_hangul_51_11172(mbcs, &wcs) == KRC_TRUE)
				{
					krc_mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else if (krc_cp949_to_unicode_special_1128(mbcs, &wcs) == KRC_TRUE)
				{
					krc_mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else if (krc_cp949_to_unicode_hanja_4888(mbcs, &wcs) == KRC_TRUE)
				{
					krc_mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else
				{
					krc_mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
				}
			}
			else
			{
				krc_mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
			}
		}
	}
	krc_mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_utf8_to_cp949(krc_char_t* utf8_string, krc_int32_t utf8_length, krc_char_t* cp949_string, krc_int32_t cp949_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, cp949_string, cp949_length);


	krc_wchar32_t wcs32;
	krc_wchar_t wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	
	krc_char_t* utf8_pointer;
	krc_int32_t utf8_size;
	krc_int32_t utf8_read;


	krc_char_t* src;
	krc_int32_t index;
	krc_int32_t count;


	src = utf8_string;
	count = utf8_length;
	for (index = 0; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (0x00u == ch1)
		{
			krc_mbcs_ostream_put_char8(&o, 0u);
			return o.length;
		}


		utf8_pointer = (src + index);
		utf8_size = count - index;
		utf8_read = utf8_to_unicode(utf8_pointer, utf8_size, &wcs32);
		switch (utf8_read)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			index += (utf8_read-1);
			break;
		}
		wcs = (krc_wchar_t)wcs32; // ucs32 -> ucs16


		if (wcs == 0x00)
		{
			krc_mbcs_ostream_put_char8(&o, 0u);
			return o.offset;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			krc_mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			krc_mbcs_ostream_put_char8(&o, ch1);
		}

		else if (krc_unicode_to_cp949_hangul_51_11172(wcs, &mbcs) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_unicode_to_cp949_special_1128(wcs, &mbcs) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_unicode_to_cp949_hanja_4888(wcs, &mbcs) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, mbcs);
		}

		else
		{
			krc_mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}
	krc_mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}
