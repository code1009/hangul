/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_input.c
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
#include "../krc/krc.h"

#include "krc_input_config.h"
#include "krc_input_type.h"

#include "krc_input_unicode.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_input_unicode_init(krc_input_unicode_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size)
{
	ctx->buffer_pointer = buffer;
	ctx->buffer_size = buffer_size;
	ctx->string_size = 0u;
	ctx->hangul_union = KRC_FALSE;
	ctx->keyboard_set = KRC_INPUT_KEYBOARD_SET_ENGLISH;
}

KRC_API void krc_input_unicode_put_char(krc_input_unicode_t* ctx, krc_wchar_t char_code)
{

}

KRC_API void krc_input_unicode_put_key(krc_input_unicode_t* ctx, krc_uint32_t key)
{

}

