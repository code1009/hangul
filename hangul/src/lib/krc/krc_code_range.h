#ifndef __krc_code_range__h__
#define __krc_code_range__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_range.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "krc_type.h"
#include "krc_config.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_CODE_RANGE_UNKNOWN                 0
#define KRC_CODE_RANGE_ASCII_LOW               1
#define KRC_CODE_RANGE_ASCII_HIGH              2
#define KRC_CODE_RANGE_HANGUL_11172            3
#define KRC_CODE_RANGE_HANGUL_JAMO_CHOSEONG    4
#define KRC_CODE_RANGE_HANGUL_JAMO_JUNGSEONG   5
#define KRC_CODE_RANGE_HANGUL_JAMO_JONGSEONG   6
#define KRC_CODE_RANGE_HANGUL_JAMO_JAEUM       7
#define KRC_CODE_RANGE_HANGUL_JAMO_MOEUM       8
#define KRC_CODE_RANGE_SPECIAL_1128            9
#define KRC_CODE_RANGE_HANJA_4888             10




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint16_t krc_code_range_cp949(const krc_char16_t ch);
KRC_API krc_uint16_t krc_code_range_unicode(const krc_wchar_t ch);





#endif




