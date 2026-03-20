#ifndef __krc_cp949_unicode__h__
#define __krc_cp949_unicode__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_unicode.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_cp949_to_unicode(
	const krc_char_t*  cp949_string, const krc_size_t cp949_string_length, 
	krc_wchar_t* unicode_string, const krc_size_t unicode_string_length
);

KRC_API krc_size_t krc_unicode_to_cp949(
	const krc_wchar_t* unicode_string , const krc_size_t unicode_string_length, 
	krc_char_t* cp949_string, const krc_size_t cp949_string_length
);





#endif




