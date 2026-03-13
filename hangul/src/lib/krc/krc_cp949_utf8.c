/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_utf8.c
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

#include "krc_cp949_unicode.h"

#include "krc_code_conversion.h"
#include "krc_unicode_conversion.h"
#include "krc_stream.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static krc_mbcs_ostream_put_utf8_from_unicode(krc_mbcs_ostream_t* o, const krc_wchar_t wcs)
{
	krc_char_t buffer[6];
	krc_size_t wsize;


	wsize = krc_unicode_to_utf8_char(wcs, buffer, 6);
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

KRC_API krc_size_t krc_cp949_to_utf8(const krc_char_t* cp949_string, const krc_size_t cp949_length, krc_char_t* utf8_string, const krc_size_t utf8_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, utf8_string, utf8_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	krc_char8_t  ch2;

	const krc_char_t* src;

	krc_size_t index;
	krc_size_t count;


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
KRC_API krc_size_t krc_utf8_to_cp949(krc_char_t* utf8_string, krc_size_t utf8_length, krc_char_t* cp949_string, krc_size_t cp949_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, cp949_string, cp949_length);


	krc_wchar32_t wcs32;
	krc_wchar_t wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	
	krc_char_t* utf8_pointer;
	krc_size_t utf8_size;
	krc_size_t utf8_read;


	krc_char_t* src;
	krc_size_t index;
	krc_size_t count;


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
		utf8_read = krc_utf8_to_unicode_char(utf8_pointer, utf8_size, &wcs32);
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
