/////////////////////////////////////////////////////////////////////////////
//
// File: bf_context.c
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "bf_config.h"
#include "bf_type.h"
#include "bf_api.h"

#include "bf_font.h"

#include "bf_context.h"

#include "bf_font_data.h"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static bf_font_t _bf_default_font_unknown = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static bf_font_t _bf_default_font_ascii_low = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static bf_font_hangul_johab844_t _bf_default_font_hangul_johab844 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0 }, { 0 }, { 0 } };

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
static bf_font_t _bf_font_cp949_special_1128 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
static bf_font_t _bf_font_cp949_hanja_4888 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#endif

//===========================================================================
static bf_context_t _context;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void bf_font_default_font_unknown(void)
{
	bf_font_setup_16x16(
		&_bf_default_font_unknown, 
		_bf_font_data_16x16_unknown, sizeof(_bf_font_data_16x16_unknown), 
		BF_FONT_TYPE_UNKNOWN, 1u
	);
}

static void bf_font_default_font_ascii_low(void)
{
	bf_font_setup_08x16(
		&_bf_default_font_ascii_low,
		_bf_font_data_08x16_ascii_low, sizeof(_bf_font_data_08x16_ascii_low),
		BF_FONT_TYPE_ASCII_LOW, 128u
	);
}

static void bf_font_default_font_hangul(void)
{
	bf_font_setup_hangul_johab844_16x16(
		&_bf_default_font_hangul_johab844,
		_bf_font_data_16x16_hangul_johab844, sizeof(_bf_font_data_16x16_hangul_johab844)
	);
}

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
static void bf_font_default_font_special(void)
{
	bf_font_setup_16x16(
		&_bf_font_cp949_special_1128,
		_bf_font_data_16x16_cp949_special_1128, sizeof(_bf_font_data_16x16_cp949_special_1128),
		BF_FONT_TYPE_CP949_SPECIAL_1128, 1128u
	);
}
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
static void bf_font_default_font_hanja(void)
{
	bf_font_setup_16x16(
		&_bf_font_cp949_hanja_4888,
		_bf_font_data_16x16_cp949_hanja_4888, sizeof(_bf_font_data_16x16_cp949_hanja_4888),
		BF_FONT_TYPE_CP949_HANJA_4888, 4888u
	);
}
#endif

static void bf_font_default_font_setup(void)
{
	bf_font_default_font_unknown();

	bf_font_default_font_ascii_low();

	bf_font_default_font_hangul();

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
	bf_font_default_font_special();
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
	bf_font_default_font_hanja();
#endif
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API void bf_context_default_setup(void)
{
	bf_font_default_font_setup();

	_context.font_unknown = &_bf_default_font_unknown;

	_context.font_ascii_low = &_bf_default_font_ascii_low;

	_context.font_hangul_johab844 = &_bf_default_font_hangul_johab844;

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
	_context.font_cp949_special_1128 = &_bf_font_cp949_special_1128;
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
	_context.font_cp949_hanja_4888 = &_bf_font_cp949_hanja_4888;
#endif
}

BF_API bf_context_t* bf_context_default_get(void)
{
	return &_context;
}

