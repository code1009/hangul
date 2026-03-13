#ifndef __bf_font__h__
#define __bf_font__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_FONT_TYPE_UNKNOWN             0
#define BF_FONT_TYPE_ASCII_LOW           1
#define BF_FONT_TYPE_HANGUL_JOHAB844     2
#define BF_FONT_TYPE_CP949_SPECIAL_1128  3
#define BF_FONT_TYPE_CP949_HANJA_4888    4




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _bf_font_t
{
	bf_uint8_t* font_data_pointer;
	bf_uint32_t font_data_size;
	bf_uint32_t font_type;
	bf_uint32_t font_count;
	bf_uint32_t font_size;
	bf_uint32_t font_cx;
	bf_uint32_t font_cy;
	bf_uint32_t font_cx_size;
	bf_uint32_t font_cy_size;
}
bf_font_t;

typedef struct _bf_font_hangul_johab844_t
{
	bf_uint8_t* font_data_pointer;
	bf_uint32_t font_data_size;
	bf_uint32_t font_type;
	bf_uint32_t font_count;
	bf_uint32_t font_size;
	bf_uint32_t font_cx;
	bf_uint32_t font_cy;
	bf_uint32_t font_cx_size;
	bf_uint32_t font_cy_size;

	bf_uint8_t* choseong [8];
	bf_uint8_t* jungseong[4];
	bf_uint8_t* jongseong[4];
}
bf_font_hangul_johab844_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BF_API bf_bool_t bf_font_08x16_init(bf_font_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size, bf_uint32_t font_type, bf_uint32_t font_count);
BF_API bf_bool_t bf_font_16x16_init(bf_font_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size, bf_uint32_t font_type, bf_uint32_t font_count);
BF_API bf_bool_t bf_font_hangul_johab844_16x16_init(bf_font_hangul_johab844_t* ctx, bf_uint8_t* font_data_pointer, bf_uint32_t font_data_size);





#endif




