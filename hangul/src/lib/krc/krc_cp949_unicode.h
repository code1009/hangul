#ifndef __krc_cp949_unicode__h__
#define __krc_cp949_unicode__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_unicode.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_size_t krc_cp949_to_unicode(const krc_char_t*  mbcs_string, const krc_size_t mbcs_length, krc_wchar_t* wcs_string , const krc_size_t wcs_length);
KRC_API krc_size_t krc_unicode_to_cp949(const krc_wchar_t* wcs_string , const krc_size_t wcs_length , krc_char_t*  mbcs_string, const krc_size_t mbcs_length);





#endif




