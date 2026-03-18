#ifndef __krc_unicode_input__h__
#define __krc_unicode_input__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_input.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_INPUT_KEYBOARD_SET_ENGLISH 1
#define KRC_INPUT_KEYBOARD_SET_KOREAN  2

//===========================================================================
#define KRC_INPUT_KEY_ESC 1

#define KRC_INPUT_KEY_F1 2
#define KRC_INPUT_KEY_F2 3
#define KRC_INPUT_KEY_F3 4
#define KRC_INPUT_KEY_F4 5
#define KRC_INPUT_KEY_F5 6
#define KRC_INPUT_KEY_F6 7
#define KRC_INPUT_KEY_F7 8
#define KRC_INPUT_KEY_F8 9
#define KRC_INPUT_KEY_F9 10
#define KRC_INPUT_KEY_F10 11
#define KRC_INPUT_KEY_F11 12
#define KRC_INPUT_KEY_F12 13

#define KRC_INPUT_KEY_GRAVE 14
#define KRC_INPUT_KEY_1 15
#define KRC_INPUT_KEY_2 16
#define KRC_INPUT_KEY_3 17
#define KRC_INPUT_KEY_4 18
#define KRC_INPUT_KEY_5 19
#define KRC_INPUT_KEY_6 20
#define KRC_INPUT_KEY_7 21
#define KRC_INPUT_KEY_8 22
#define KRC_INPUT_KEY_9 23
#define KRC_INPUT_KEY_0 24
#define KRC_INPUT_KEY_MINUS 25
#define KRC_INPUT_KEY_PLUS 26
#define KRC_INPUT_KEY_BACKSPACE 27

#define KRC_INPUT_KEY_TAB 28

#define KRC_INPUT_KEY_A 29
#define KRC_INPUT_KEY_B 30
#define KRC_INPUT_KEY_C 31
#define KRC_INPUT_KEY_D 32
#define KRC_INPUT_KEY_E 33
#define KRC_INPUT_KEY_F 34
#define KRC_INPUT_KEY_G 35
#define KRC_INPUT_KEY_H 36
#define KRC_INPUT_KEY_I 37
#define KRC_INPUT_KEY_J 38
#define KRC_INPUT_KEY_K 39
#define KRC_INPUT_KEY_L 40
#define KRC_INPUT_KEY_M 41
#define KRC_INPUT_KEY_N 42
#define KRC_INPUT_KEY_O 43
#define KRC_INPUT_KEY_P 44
#define KRC_INPUT_KEY_Q 45
#define KRC_INPUT_KEY_R 46
#define KRC_INPUT_KEY_S 47
#define KRC_INPUT_KEY_T 48
#define KRC_INPUT_KEY_U 49
#define KRC_INPUT_KEY_V 50
#define KRC_INPUT_KEY_W 51
#define KRC_INPUT_KEY_X 52
#define KRC_INPUT_KEY_Y 53
#define KRC_INPUT_KEY_Z 54

#define KRC_INPUT_KEY_LBRACKET 55
#define KRC_INPUT_KEY_RBRACKET 56
#define KRC_INPUT_KEY_BACKSLASH 57
#define KRC_INPUT_KEY_SEMICOLON 58
#define KRC_INPUT_KEY_APOSTROPHE 59
#define KRC_INPUT_KEY_COMMA 60
#define KRC_INPUT_KEY_DOT 61
#define KRC_INPUT_KEY_SLASH 62

#define KRC_INPUT_KEY_ENTER 63
#define KRC_INPUT_KEY_SPACE 64

#define KRC_INPUT_KEY_CAPSLOCK 0x8065

#define KRC_INPUT_KEY_LSHIFT 0x8066
#define KRC_INPUT_KEY_LALT 0x8067
#define KRC_INPUT_KEY_LCTRL 0x8068

#define KRC_INPUT_KEY_RSHIFT 0x8069
#define KRC_INPUT_KEY_RALT 0x8070
#define KRC_INPUT_KEY_RCTRL 0x8071

#define KRC_INPUT_KEY_HANGUL KRC_INPUT_KEY_RALT
#define KRC_INPUT_KEY_HANJA KRC_INPUT_KEY_RCTRL

#define KRC_INPUT_KEY_PRTSCRN 0x8072
#define KRC_INPUT_KEY_SCROLLLOCK 0x8073
#define KRC_INPUT_KEY_PAUSE 0x8074

#define KRC_INPUT_KEY_INSERT 0x8075
#define KRC_INPUT_KEY_HOME 0x8076
#define KRC_INPUT_KEY_PAGEUP 0x8077
#define KRC_INPUT_KEY_DELETE 0x8078
#define KRC_INPUT_KEY_END 0x8079
#define KRC_INPUT_KEY_PAGEDOWN 0x807A

#define KRC_INPUT_KEY_LEFT 0x807B
#define KRC_INPUT_KEY_UP 0x807C
#define KRC_INPUT_KEY_RIGHT 0x807D
#define KRC_INPUT_KEY_DOWN 0x807E

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _krc_input_unicode_t
{
	krc_wchar_t* buffer_pointer;
	krc_size_t buffer_size;
	krc_size_t string_size;
	krc_size_t cursor;
	krc_size_t cursor_line;
	krc_size_t cursor_column;
	krc_bool_t hangul_union;
	krc_uint32_t keyboard_set;
} krc_input_unicode_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_input_unicode_init(krc_input_unicode_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size);
KRC_API void krc_input_unicode_put_char(krc_input_unicode_t* ctx, krc_wchar_t char_code);
KRC_API void krc_input_unicode_put_key(krc_input_unicode_t* ctx, krc_uint32_t key);





#endif




