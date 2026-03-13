/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_unicode.c
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
#include "krc_stream.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_cp949_to_unicode(const krc_char_t* mbcs_string, const krc_size_t mbcs_length, krc_wchar_t* wcs_string, const krc_size_t wcs_length)
{
	krc_wcs_ostream_t o;


	krc_wcs_ostream_init(&o, wcs_string, wcs_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;
	krc_char8_t  ch2;

	const krc_char_t* src;

	krc_size_t index;
	krc_size_t count;


	src = mbcs_string;
	count = mbcs_length;
	for (index = 0u; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (ch1 == 0x00u)
		{
			krc_wcs_ostream_term(&o);
			return o.length;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			krc_wcs_ostream_put_wchar(&o, ch1);
		}
		else
		{
			if ((index + 1u) < count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1u));
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

	krc_wcs_ostream_term(&o);
	return o.length;
}

//===========================================================================
KRC_API krc_size_t krc_unicode_to_cp949(const krc_wchar_t* wcs_string, krc_size_t wcs_length, krc_char_t* mbcs_string, krc_size_t mbcs_length)
{
	krc_mbcs_ostream_t o;


	krc_mbcs_ostream_init(&o, mbcs_string, mbcs_length);


	krc_wchar_t  wcs;
	krc_char16_t mbcs;
	krc_char8_t  ch1;


	const krc_wchar_t* src;

	krc_size_t index;
	krc_size_t count;

	src = wcs_string;
	count = wcs_length;
	for (index = 0; index < count; index++)
	{
		wcs = *(src + index);


		if (wcs == 0x00u)
		{
			krc_mbcs_ostream_term(&o);
			return o.length;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);
			krc_mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			 if ((0x00A1u <= wcs) && (wcs <= 0x00FEu))
			 {
				 if (krc_unicode_to_cp949_special_1128(wcs, &mbcs) == KRC_TRUE)
				 {
					 krc_mbcs_ostream_put_char16(&o, mbcs);
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

	krc_mbcs_ostream_term(&o);
	return o.length;
}
