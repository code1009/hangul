/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_unicode.c
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

#include "krc_cp949_unicode.h"

#include "krc_code_conversion.h"
#include "krc_stream.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_cp949_to_unicode(
	const krc_char_t* cp949_string, const krc_size_t cp949_string_length,
	krc_wchar_t* unicode_string, const krc_size_t unicode_string_length
)
{
	krc_wcs_ostream_t o;
	krc_wcs_ostream_init(&o, unicode_string, unicode_string_length);


	krc_wchar_t  unicode;
	krc_char16_t cp949;
	krc_char8_t  ch1;
	krc_char8_t  ch2;


	const krc_char_t* src;
	krc_size_t index;
	krc_size_t count;


	src = cp949_string;
	count = cp949_string_length;
	for (index = 0u; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (ch1 == 0x00u)
		{
			krc_wcs_ostream_put_null_term(&o);
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
				cp949 = (ch1 << 8u) | ch2;


				if (krc_cp949_to_unicode_hangul_51_11172(cp949, &unicode) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, unicode);
					index++;
				}

				else if (krc_cp949_to_unicode_special_1128(cp949, &unicode) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, unicode);
					index++;
				}

				else if (krc_cp949_to_unicode_hanja_4888(cp949, &unicode) == KRC_TRUE)
				{
					krc_wcs_ostream_put_wchar(&o, unicode);
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

	krc_wcs_ostream_put_null_term(&o);
	return o.length;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_unicode_to_cp949(
	const krc_wchar_t* unicode_string, const krc_size_t unicode_string_length,
	krc_char_t* cp949_string, const krc_size_t cp949_string_length
)
{
	krc_mbcs_ostream_t o;
	krc_mbcs_ostream_init(&o, cp949_string, cp949_string_length);


	krc_wchar_t  unicode;
	krc_char16_t cp949;
	krc_char8_t  ch1;


	const krc_wchar_t* src;
	krc_size_t index;
	krc_size_t count;


	src = unicode_string;
	count = unicode_string_length;
	for (index = 0; index < count; index++)
	{
		unicode = *(src + index);


		if      (unicode == 0x0000u)
		{
			krc_mbcs_ostream_put_null_term(&o);
			return o.length;
		}
		else if (unicode < 0x0080u)
		{
			ch1 = (krc_char8_t)(unicode & 0x00FFu);
			krc_mbcs_ostream_put_char8(&o, ch1);
		}
		else if (unicode < 0x0100u)
		{
			 if ((0x00A1u <= unicode) && (unicode <= 0x00FEu))
			 {
				 if (krc_unicode_to_cp949_special_1128(unicode, &cp949) == KRC_TRUE)
				 {
					 krc_mbcs_ostream_put_char16(&o, cp949);
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

		else if (krc_unicode_to_cp949_hangul_51_11172(unicode, &cp949) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, cp949);
		}

		else if (krc_unicode_to_cp949_special_1128(unicode, &cp949) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, cp949);
		}

		else if (krc_unicode_to_cp949_hanja_4888(unicode, &cp949) == KRC_TRUE)
		{
			krc_mbcs_ostream_put_char16(&o, cp949);
		}

		else
		{
			krc_mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}

	krc_mbcs_ostream_put_null_term(&o);
	return o.length;
}
