#ifndef __bf_font_data__h__
#define __bf_font_data__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: bf_font_data.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//                                                        ( x/8 * y )* count
extern bf_uint8_t _bf_font_data_16x16_unknown           [ (16/8 * 16)*   1 ];

extern bf_uint8_t _bf_font_data_08x16_ascii_low         [ ( 8/8 * 16)* 128 ];

extern bf_uint8_t _bf_font_data_16x16_hangul_johab844   [ (16/8 * 16)*( (19*8)+(21*4)+(27*4) ) ];

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
extern bf_uint8_t _bf_font_data_16x16_cp949_special_1128[ (16/8 * 16)*1128 ];
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
extern bf_uint8_t _bf_font_data_16x16_cp949_hanja_4888  [ (16/8 * 16)*4888 ];
#endif





#endif




