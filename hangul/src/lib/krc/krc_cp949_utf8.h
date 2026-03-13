#ifndef __krc_cp949_utf8__h__
#define __krc_cp949_utf8__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_utf8.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_cp949_to_utf8(const krc_char_t* cp949_string, const krc_size_t cp949_length, krc_char_t* utf8_string, const krc_size_t utf8_length);
KRC_API krc_size_t krc_utf8_to_cp949(const krc_char_t* utf8_string, const krc_size_t utf8_length, krc_char_t* cp949_string, const krc_size_t cp949_length);





#endif




