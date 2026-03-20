#ifndef __krc_input_key__h__
#define __krc_input_key__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_input_key.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_INPUT_KEY_MODE_LATIN   1
#define KRC_INPUT_KEY_MODE_HANGUL  2





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// 문자키
//===========================================================================
#define KRC_INPUT_KEY_1           1
#define KRC_INPUT_KEY_2           2
#define KRC_INPUT_KEY_3           3
#define KRC_INPUT_KEY_4           4
#define KRC_INPUT_KEY_5           5
#define KRC_INPUT_KEY_6           6
#define KRC_INPUT_KEY_7           7
#define KRC_INPUT_KEY_8           8
#define KRC_INPUT_KEY_9           9
#define KRC_INPUT_KEY_0          10

#define KRC_INPUT_KEY_A          11
#define KRC_INPUT_KEY_B          12
#define KRC_INPUT_KEY_C          13
#define KRC_INPUT_KEY_D          14
#define KRC_INPUT_KEY_E          15
#define KRC_INPUT_KEY_F          16
#define KRC_INPUT_KEY_G          17
#define KRC_INPUT_KEY_H          18
#define KRC_INPUT_KEY_I          19
#define KRC_INPUT_KEY_J          20
#define KRC_INPUT_KEY_K          21
#define KRC_INPUT_KEY_L          22
#define KRC_INPUT_KEY_M          23
#define KRC_INPUT_KEY_N          24
#define KRC_INPUT_KEY_O          25
#define KRC_INPUT_KEY_P          26
#define KRC_INPUT_KEY_Q          27
#define KRC_INPUT_KEY_R          28
#define KRC_INPUT_KEY_S          29
#define KRC_INPUT_KEY_T          30
#define KRC_INPUT_KEY_U          31
#define KRC_INPUT_KEY_V          32
#define KRC_INPUT_KEY_W          33
#define KRC_INPUT_KEY_X          34
#define KRC_INPUT_KEY_Y          35
#define KRC_INPUT_KEY_Z          36

#define KRC_INPUT_KEY_GRAVE      37
#define KRC_INPUT_KEY_MINUS      38
#define KRC_INPUT_KEY_EQUAL      39
#define KRC_INPUT_KEY_LBRACKET   40
#define KRC_INPUT_KEY_RBRACKET   41
#define KRC_INPUT_KEY_BACKSLASH  42
#define KRC_INPUT_KEY_SEMICOLON  43
#define KRC_INPUT_KEY_APOSTROPHE 44
#define KRC_INPUT_KEY_COMMA      45
#define KRC_INPUT_KEY_DOT        46
#define KRC_INPUT_KEY_SLASH      47

#define KRC_INPUT_KEY_SPACE      48

//===========================================================================
// 특수키
//===========================================================================
#define KRC_INPUT_KEY_ENTER      49
#define KRC_INPUT_KEY_BACKSPACE  50
#define KRC_INPUT_KEY_TAB        51

#define KRC_INPUT_KEY_ESC        52
#define KRC_INPUT_KEY_F1         53
#define KRC_INPUT_KEY_F2         54
#define KRC_INPUT_KEY_F3         55
#define KRC_INPUT_KEY_F4         56
#define KRC_INPUT_KEY_F5         57
#define KRC_INPUT_KEY_F6         58
#define KRC_INPUT_KEY_F7         59
#define KRC_INPUT_KEY_F8         60
#define KRC_INPUT_KEY_F9         61
#define KRC_INPUT_KEY_F10        62
#define KRC_INPUT_KEY_F11        63
#define KRC_INPUT_KEY_F12        64

#define KRC_INPUT_KEY_CAPSLOCK   65

#define KRC_INPUT_KEY_PRTSCRN    66
#define KRC_INPUT_KEY_SCROLLLOCK 67
#define KRC_INPUT_KEY_PAUSE      68

#define KRC_INPUT_KEY_INSERT     69
#define KRC_INPUT_KEY_HOME       70
#define KRC_INPUT_KEY_PAGEUP     71
#define KRC_INPUT_KEY_DELETE     72
#define KRC_INPUT_KEY_END        73
#define KRC_INPUT_KEY_PAGEDOWN   74

#define KRC_INPUT_KEY_LEFT       75
#define KRC_INPUT_KEY_UP         76
#define KRC_INPUT_KEY_RIGHT      77
#define KRC_INPUT_KEY_DOWN       78

//===========================================================================
// 수정자 키 (Modifier Keys)
//===========================================================================
#define KRC_INPUT_KEY_MODIFIER_VALUE(x) ((x)<<16u)

#define KRC_INPUT_KEY_MODIFIER_MASK 0xFFFF0000u
#define KRC_INPUT_KEY_MODIFIER(x) ((x) & KRC_INPUT_KEY_MODIFIER_MASK)

#define KRC_INPUT_KEY_CODE_MASK 0x0000FFFFu
#define KRC_INPUT_KEY_CODE(x) ((x) & (KRC_INPUT_KEY_CODE_MASK))

//===========================================================================
#define KRC_INPUT_KEY_LSHIFT (KRC_INPUT_KEY_MODIFIER_VALUE(0x8000u))
#define KRC_INPUT_KEY_LALT   (KRC_INPUT_KEY_MODIFIER_VALUE(0x4000u))
#define KRC_INPUT_KEY_LCTRL  (KRC_INPUT_KEY_MODIFIER_VALUE(0x2000u))
#define KRC_INPUT_KEY_RSHIFT (KRC_INPUT_KEY_MODIFIER_VALUE(0x1000u))
#define KRC_INPUT_KEY_RALT   (KRC_INPUT_KEY_MODIFIER_VALUE(0x0800u))
#define KRC_INPUT_KEY_RCTRL  (KRC_INPUT_KEY_MODIFIER_VALUE(0x0400u))

//===========================================================================
// 변환키
//===========================================================================
#define KRC_INPUT_KEY_HANGUL KRC_INPUT_KEY_RALT
#define KRC_INPUT_KEY_HANJA  KRC_INPUT_KEY_RCTRL





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_INPUT_KEY_CHAR_COUNT (KRC_INPUT_KEY_SPACE - KRC_INPUT_KEY_1 + 1u)





#endif




