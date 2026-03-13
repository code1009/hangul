#ifndef __krc_code_conversion__h__
#define __krc_code_conversion__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_conversion.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_cp949_to_unicode_hangul_51_11172(const krc_char16_t cp949, krc_wchar_t* unicode);
KRC_API krc_bool_t krc_cp949_to_unicode_special_1128(const krc_char16_t cp949, krc_wchar_t* unicode);
KRC_API krc_bool_t krc_cp949_to_unicode_hanja_4888(const krc_char16_t cp949, krc_wchar_t* unicode);

KRC_API krc_bool_t krc_unicode_to_cp949_hangul_51_11172(const krc_wchar_t unicode, krc_char16_t* cp949);
KRC_API krc_bool_t krc_unicode_to_cp949_special_1128(const krc_wchar_t unicode, krc_char16_t* cp949);
KRC_API krc_bool_t krc_unicode_to_cp949_hanja_4888(const krc_wchar_t unicode, krc_char16_t* cp949);



#endif




