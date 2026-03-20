#ifndef __krc_type_h__
#define __krc_type_h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_type.h
//
// Created by MOON, Eui-kwon.
// Created on Mar-20th, 2026.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//typedef signed   char      krc_int8_t;
//typedef unsigned char      krc_uint8_t;
typedef signed   short int krc_int16_t;
typedef unsigned short int krc_uint16_t;
//typedef signed   int       krc_int32_t;
typedef unsigned int       krc_uint32_t; // krc_input에서 사용

typedef unsigned long long int krc_size_t;
//typedef signed   long long int krc_ssize_t;

//===========================================================================
typedef unsigned int krc_bool_t;

//===========================================================================
typedef char               krc_char_t;

typedef unsigned char      krc_char8_t;
typedef unsigned short int krc_char16_t;

typedef unsigned short int krc_wchar_t;

typedef unsigned char      krc_uchar8_t;
typedef unsigned short int krc_uchar16_t;
typedef unsigned long  int krc_uchar32_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_TRUE  1u
#define KRC_FALSE 0u





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif




