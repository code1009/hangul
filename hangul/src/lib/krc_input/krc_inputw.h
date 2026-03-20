#ifndef __krc_inputw__h__
#define __krc_inputw__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_inputw.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _krc_inputw_t
{
	krc_wchar_t* buffer_pointer;
	krc_size_t   buffer_size;
	krc_bool_t   multiline;
	krc_size_t   length;
	krc_size_t   cursor_offset;
	krc_size_t   cursor_line_offset;
	krc_size_t   cursor_line;
	krc_size_t   cursor_column;
	krc_bool_t   shift_mode;       // 한글/영어 모두에서 SHIFT
	krc_bool_t   capslock_mode;    // 영문시 대문자
	krc_bool_t   insert_mode;      // 삽입/덮어쓰기
	krc_uint32_t key_mode;         // 한/영
	krc_bool_t   hangul_composing; // 한글 조합 중 여부 (초성/중성/종성 조합이 진행 중인지)
} krc_inputw_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_inputw_init(krc_inputw_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size, krc_bool_t multiline);

KRC_API void krc_inputw_put_char(krc_inputw_t* ctx, krc_wchar_t char_code);
KRC_API void krc_inputw_put_key(krc_inputw_t* ctx, krc_uint32_t key);

KRC_API void krc_inputw_put_new_line(krc_inputw_t* ctx);
KRC_API void krc_inputw_put_tab(krc_inputw_t* ctx);

KRC_API void krc_inputw_set_shift_mode(krc_inputw_t* ctx, krc_bool_t mode);
KRC_API void krc_inputw_set_capslock_mode(krc_inputw_t* ctx, krc_bool_t mode);
KRC_API void krc_inputw_set_insert_mode(krc_inputw_t* ctx, krc_bool_t mode);
KRC_API void krc_inputw_set_key_mode(krc_inputw_t* ctx, krc_uint32_t mode);





#endif




