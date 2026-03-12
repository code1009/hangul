#ifndef __krc_convert__h__
#define __krc_convert__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_convert.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_cp949_to_unicode(krc_char_t* mbcs_string, krc_int32_t mbcs_length, krc_wchar_t* wcs_string, krc_int32_t wcs_length);
KRC_API krc_int32_t krc_unicode_to_cp949(krc_wchar_t* wcs_string, krc_int32_t wcs_length, krc_char_t* mbcs_string, krc_int32_t mbcs_length);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_cp949_to_utf8(krc_char_t* cp949_string, krc_int32_t cp949_length, krc_char_t* utf8_string, krc_int32_t utf8_length);
KRC_API krc_int32_t krc_utf8_to_cp949(krc_char_t* utf8_string, krc_int32_t utf8_length, krc_char_t* cp949_string, krc_int32_t cp949_length);





#endif




