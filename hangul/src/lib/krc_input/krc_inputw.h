#ifndef __krc_inputw__h__
#define __krc_inputw__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_input_unicode.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_INPUT_KEYBOARD_SET_LATIN 1
#define KRC_INPUT_KEYBOARD_SET_HANGUL  2





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _krc_inputw_t
{
	krc_wchar_t* buffer_pointer;
	krc_size_t buffer_size;
	krc_bool_t multiline;
	krc_size_t length;
	krc_size_t cursor;
	krc_size_t cursor_line;
	krc_size_t cursor_column;
	krc_bool_t shift_state;
	krc_bool_t insert_mode;
	krc_uint32_t keyboard_set;
	krc_bool_t hangul_composing;
} krc_inputw_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_inputw_init(krc_inputw_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size, krc_bool_t multiline);
KRC_API void krc_inputw_put_char(krc_inputw_t* ctx, krc_wchar_t char_code);
KRC_API void krc_inputw_put_key(krc_inputw_t* ctx, krc_uint32_t key);





#endif




