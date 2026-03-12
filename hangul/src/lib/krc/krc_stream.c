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
#include "krc_stream.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_mbcs_ostream_init(krc_mbcs_ostream_t* ctx, krc_char_t* p, krc_int32_t l)
{
	ctx->pointer = p;
	ctx->max_length = l;
	ctx->length = 0;
	ctx->offset = 0;
}

KRC_API void krc_mbcs_ostream_put_char(krc_mbcs_ostream_t* ctx, krc_char_t c)
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

KRC_API void krc_mbcs_ostream_put_char8(krc_mbcs_ostream_t* ctx, krc_char8_t c)
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

KRC_API void krc_mbcs_ostream_put_char16(krc_mbcs_ostream_t* ctx, krc_char16_t c)
{
	if (ctx->offset + 1 < ctx->max_length)
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
		ctx->length += 2;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_wcs_ostream_init(krc_wcs_ostream_t* ctx, krc_wchar_t* p, krc_int32_t l)
{
	ctx->pointer = p;
	ctx->max_length = l;
	ctx->length = 0;
	ctx->offset = 0;
}

KRC_API krc_wcs_ostream_put_wchar(krc_wcs_ostream_t* ctx, krc_wchar_t c)
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
