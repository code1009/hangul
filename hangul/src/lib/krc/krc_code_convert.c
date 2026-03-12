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
 
#include "krc_code_table.h"

#include "krc_code_type.h"
#include "krc_code_convert.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_hangul_11172_index_cp949 (krc_char16_t ch)
{
	krc_uint32_t offset;

	krc_int32_t index;


	index = -1;


	//-----------------------------------------------------------------------
	// 완성형한글.한글2350자 
	//-----------------------------------------------------------------------

	// 0xB0A1u '가' - 0xC8FEu '힝'
	if (ch>=0xB0A1u && ch<=0xC8FEu)
	{
		offset = (ch-0xB0A0u)%0x0100u;
		if (offset>0u && offset<96u)
		{
			index = (ch-0xB0A0u)/0x0100u*94u + offset - 1u;
			return index;
		}
	}


	//-----------------------------------------------------------------------
	// 확장완성형한글.추가한글8822자
	//-----------------------------------------------------------------------

	// 5696 + 3108 + 18

	// 5696자
	// 0x8141u '갂' - 0xA0FEu '좤' 
	if (ch>=0x8141u && ch<=0xA0FEu)
	{
		offset = (ch-0x8140u)%0x0100u;
		if (offset>0u && offset<191u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040u) offset -= 12u;
			else if (offset>0x0020u) offset -= 6u;

			index  = (ch-0x8140u)/0x0100u*(192u-2u-6u-6u) + offset - 1u + 2350u;
			return index;
		}
	}

	// 3108자
	// 0xA141u '좥' - 0xC5A0u '힋' 
	if (ch>=0xA141u && ch<=0xC5A0u)
	{
		offset = (ch-0xA140u)%0x0100u;
		if (offset>0u && offset<97u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040) offset -= 12u;
			else if (offset>0x0020) offset -= 6u;

			index  = (ch-0xA140u)/0x0100u*(97u-6u-6u-1u) + offset - 1u + 5696u + 2350u;
			return index;
		}
	}

	// 18자
	// 0xC641u '힍' - 0xC652u '힣' 
	if (ch>=0xC641u && ch<=0xC652u)
	{
		index = ch - 0xC640u - 1u + 3108u + 5696u + 2350u;
		return index;
	}


	return index;
}

KRC_API krc_int32_t krc_special_1128_index_cp949 (krc_char16_t ch)
{
	krc_uint32_t offset;


	/*
	# 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	  12개영역 * 94문자 = 1128문자
	     0xA1A0u
	   + 0x0C00u(12*256)
	   = 0xFEA0u
	*/
	if (0xA1A1u<=ch && ch <= 0xACFEu)
	{
		offset = (ch-0xA1A0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xA1A0u)/0x0100u)*94u + offset -1u;
		}
	}

	return -1;
}

KRC_API krc_int32_t krc_hanja_4888_index_cp949 (krc_char16_t ch)
{
	krc_uint32_t offset;


	/*
	# 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	  52개영역 * 94문자 = 4888문자
	     0xCAA0u
	   + 0x3400u(52*256)
	   = 0xFEA0u
	*/
	if (0xCAA1u<=ch && ch <= 0xFDFEu)
	{
		offset = (ch-0xCAA0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xCAA0u)/0x0100u)*94u + offset - 1u;
		}
	}

	return -1;
}





/////////////////////////////////////////////////////////////////////////////
// 
// cp949_to_unicode 
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_hangul_51_11172_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int32_t index;

	
	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if (cp949>=0xA4A1u && cp949<=0xA4D3u)
	{
		*unicode = 0x3131u + cp949-0xA4A1u;
		return KRC_TRUE;
	}


	index = krc_hangul_11172_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_hangul_11172_cp949_to_unicode[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_special_1128_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int32_t index;


	index = krc_special_1128_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_special_1128_cp949_to_unicode[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_hanja_4888_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int32_t index;


	index = krc_hanja_4888_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_hanja_4888_cp949_to_unicode[index];

	return KRC_TRUE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// unicode_to_cp949
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_hangul_51_11172_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	krc_int32_t index;


	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if (unicode>=0x3131u && unicode<=0x3163u)
	{
		*cp949 = 0xA4A1u + unicode-0x3131u;
		return KRC_TRUE;
	}


	// 유니코드 한글 11172자: 0xAC00u '가' - 0xD7A3u '힣'
	if (unicode>=0xAC00u && unicode<=0xD7A3u)
	{
		index = unicode - 0xAC00u;
		*cp949 = _krc_code_table_hangul_11172_unicode_to_cp949[index];
		return KRC_TRUE;
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_special_1128_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	if (KRC_CODE_TYPE_SPECIAL_1128 != krc_code_type_unicode(unicode))
	{
		return KRC_FALSE;
	}


	krc_int32_t index;


	// 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	for (index=0; index<1128; index++)
	{
		if (unicode == _krc_code_table_special_1128_cp949_to_unicode[index])
		{
			*cp949 = 0xA1A1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_hanja_4888_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	if (KRC_CODE_TYPE_HANJA_4888 != krc_code_type_unicode(unicode))
	{
		return KRC_FALSE;
	}


	krc_int32_t index;


	// 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	for (index=0; index<4888; index++)
	{
		if (unicode == _krc_code_table_hanja_4888_cp949_to_unicode[index])
		{
			*cp949 = 0xCAA1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _mbcs_ostream
{
	krc_char_t* pointer;
	krc_int32_t max_length;
	krc_int32_t length;
	krc_int32_t offset;
}
mbcs_ostream;

static void mbcs_ostream_init(mbcs_ostream* ctx, krc_char_t* p, krc_int32_t l)
{
	ctx->pointer = p;
	ctx->max_length = l;
	ctx->length = 0;
	ctx->offset = 0;
}

static void mbcs_ostream_put_char(mbcs_ostream* ctx, krc_char_t c)
{
	if (ctx->offset < ctx->max_length)
	{
		if (ctx->pointer)
		{
			*(ctx->pointer + ctx->offset) = c;
			ctx->offset++;
		}
	}

	if (c)
	{
		ctx->length++;
	}
}

static void mbcs_ostream_put_char8(mbcs_ostream* ctx, krc_char8_t c)
{
	if (ctx->offset < ctx->max_length)
	{
		if (ctx->pointer)
		{
			*(ctx->pointer + ctx->offset) = (krc_char_t)c;
			ctx->offset++;
		}
	}

	if (c)
	{
		ctx->length++;
	}
}

static void mbcs_ostream_put_char16(mbcs_ostream* ctx, krc_char16_t c)
{
	if (ctx->offset+1 < ctx->max_length)
	{
		if (ctx->pointer)
		{
			*(ctx->pointer + ctx->offset) = (krc_char_t)((c & 0xFF00u) >> 8u);
			ctx->offset++;

			*(ctx->pointer + ctx->offset) = (krc_char_t)(c & 0x00FFu);
			ctx->offset++;
		}
	}

	if (c)
	{
		ctx->length+=2;
	}
}

//===========================================================================
typedef struct _wcs_ostream
{
	krc_wchar_t* pointer;
	krc_int32_t max_length;
	krc_int32_t length;
	krc_int32_t offset;
}
wcs_ostream;

static void wcs_ostream_init(wcs_ostream* ctx, krc_wchar_t* p, krc_int32_t l)
{
	ctx->pointer = p;
	ctx->max_length = l;
	ctx->length = 0;
	ctx->offset = 0;
}

static void wcs_ostream_put_wchar(wcs_ostream* ctx, krc_wchar_t c)
{
	if (ctx->offset < ctx->max_length)
	{
		if (ctx->pointer)
		{
			*(ctx->pointer + ctx->offset) = c;
			ctx->offset++;
		}
	}

	if (c)
	{
		ctx->length++;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_unicode_to_cp949(krc_wchar_t* wcs_string, krc_int32_t wcs_length, krc_char_t* mbcs_string, krc_int32_t mbcs_length)
{
	mbcs_ostream o;


	mbcs_ostream_init(&o, mbcs_string, mbcs_length);


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
			mbcs_ostream_put_char8(&o, 0u);
			return o.length;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}

		else if (krc_hangul_51_11172_unicode_to_cp949  (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_special_1128_unicode_to_cp949 (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_hanja_4888_unicode_to_cp949   (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else
		{
			mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}
	mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}

//===========================================================================
KRC_API krc_int32_t krc_cp949_to_unicode(krc_char_t* mbcs_string, krc_int32_t mbcs_length, krc_wchar_t* wcs_string, krc_int32_t wcs_length)
{
	wcs_ostream o;


	wcs_ostream_init(&o, wcs_string, wcs_length);


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
			wcs_ostream_put_wchar(&o, 0u);
			return o.length;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			wcs_ostream_put_wchar(&o, ch1);
		}
		else
		{
			if ((index + 1) <= count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1));
				mbcs = (ch1 << 8u) | ch2;


				if (krc_hangul_51_11172_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_special_1128_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_hanja_4888_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else
				{
					wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
				}
			}
			else
			{
				wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
			}
		}
	}
	wcs_ostream_put_wchar(&o, 0u);


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
static mbcs_ostream_put_utf8_from_unicode(mbcs_ostream* o, krc_wchar_t wcs)
{
	krc_char_t buffer[6];
	krc_int32_t wsize;


	wsize = unicode_to_utf8(wcs, buffer, 6);
	switch (wsize)
	{
	case 1:
		mbcs_ostream_put_char8(o, buffer[0]);
		break;

	case 2:
		mbcs_ostream_put_char8(o, buffer[0]);
		mbcs_ostream_put_char8(o, buffer[1]);
		break;

	case 3:
		mbcs_ostream_put_char8(o, buffer[0]);
		mbcs_ostream_put_char8(o, buffer[1]);
		mbcs_ostream_put_char8(o, buffer[2]);
		break;

	case 4:
		mbcs_ostream_put_char8(o, buffer[0]);
		mbcs_ostream_put_char8(o, buffer[1]);
		mbcs_ostream_put_char8(o, buffer[2]);
		mbcs_ostream_put_char8(o, buffer[3]);
		break;

	case 5:
		mbcs_ostream_put_char8(o, buffer[0]);
		mbcs_ostream_put_char8(o, buffer[1]);
		mbcs_ostream_put_char8(o, buffer[2]);
		mbcs_ostream_put_char8(o, buffer[3]);
		mbcs_ostream_put_char8(o, buffer[4]);
		break;

	case 6:
		mbcs_ostream_put_char8(o, buffer[0]);
		mbcs_ostream_put_char8(o, buffer[1]);
		mbcs_ostream_put_char8(o, buffer[2]);
		mbcs_ostream_put_char8(o, buffer[3]);
		mbcs_ostream_put_char8(o, buffer[4]);
		mbcs_ostream_put_char8(o, buffer[5]);
		break;

	default:
		mbcs_ostream_put_char8(o, 0x3Fu); // ?
		break;
	}
}

KRC_API krc_int32_t krc_cp949_to_utf8(krc_char_t* cp949_string, krc_int32_t cp949_length, krc_char_t* utf8_string, krc_int32_t utf8_length)
{
	mbcs_ostream o;


	mbcs_ostream_init(&o, utf8_string, utf8_length);


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
			mbcs_ostream_put_char8(&o, 0u);
			return o.length;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			mbcs_ostream_put_char8(&o, ch1);
		}
		else
		{
			if ((index + 1) <= count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1));
				mbcs = (ch1 << 8u) | ch2;


				if (krc_hangul_51_11172_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else if (krc_special_1128_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else if (krc_hanja_4888_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					mbcs_ostream_put_utf8_from_unicode(&o, wcs);
					index++;
				}

				else
				{
					mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
				}
			}
			else
			{
				mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
			}
		}
	}
	mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_utf8_to_cp949(krc_char_t* utf8_string, krc_int32_t utf8_length, krc_char_t* cp949_string, krc_int32_t cp949_length)
{
	mbcs_ostream o;


	mbcs_ostream_init(&o, cp949_string, cp949_length);


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
			mbcs_ostream_put_char8(&o, 0u);
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
			mbcs_ostream_put_char8(&o, 0u);
			return o.offset;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}

		else if (krc_hangul_51_11172_unicode_to_cp949(wcs, &mbcs) == KRC_TRUE)
		{
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_special_1128_unicode_to_cp949(wcs, &mbcs) == KRC_TRUE)
		{
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_hanja_4888_unicode_to_cp949(wcs, &mbcs) == KRC_TRUE)
		{
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else
		{
			mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}
	mbcs_ostream_put_char8(&o, 0u);


	return o.length;
}
