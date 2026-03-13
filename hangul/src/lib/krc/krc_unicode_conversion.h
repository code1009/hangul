#ifndef __krc_unicode_conversion__h__
#define __krc_unicode_conversion__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_conversion.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_unicode_to_utf8_char(const krc_wchar32_t unicode, krc_char_t* utf8_pointer, const krc_size_t utf8_size);
KRC_API krc_size_t krc_utf8_to_unicode_char(const krc_char_t* utf8_pointer, const krc_size_t utf8_size, krc_wchar32_t* unicode);

//===========================================================================
KRC_API krc_size_t krc_utf8_char_size(const krc_char_t* utf8_pointer);





#endif




