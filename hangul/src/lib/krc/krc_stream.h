#ifndef __krc_stream__h__
#define __krc_stream__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_stream.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _krc_mbcs_ostream_t
{
	krc_char_t* pointer;
	krc_size_t max_length;
	krc_size_t length;
	krc_size_t offset;
}
krc_mbcs_ostream_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_mbcs_ostream_init(krc_mbcs_ostream_t* ctx, krc_char_t* p, const krc_size_t l);
KRC_API void krc_mbcs_ostream_put_null_term(krc_mbcs_ostream_t* ctx);
KRC_API void krc_mbcs_ostream_put_char(krc_mbcs_ostream_t* ctx, const krc_char_t c);
KRC_API void krc_mbcs_ostream_put_char8(krc_mbcs_ostream_t* ctx, const krc_char8_t c);
KRC_API void krc_mbcs_ostream_put_char16(krc_mbcs_ostream_t* ctx, const krc_char16_t c);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _krc_wcs_ostream_t
{
	krc_wchar_t* pointer;
	krc_size_t max_length;
	krc_size_t length;
	krc_size_t offset;
}
krc_wcs_ostream_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_wcs_ostream_init(krc_wcs_ostream_t* ctx, krc_wchar_t* p, const krc_size_t l);
KRC_API void krc_wcs_ostream_put_null_term(krc_wcs_ostream_t* ctx);
KRC_API void krc_wcs_ostream_put_wchar(krc_wcs_ostream_t* ctx, const krc_wchar_t c);





#endif




