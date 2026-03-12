/////////////////////////////////////////////////////////////////////////////
//
// File: krc_cp949_index.c
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "krc_config.h"
#include "krc_type.h"

#include "krc_api.h"
#include "krc_cp949_index.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int32_t krc_cp949_index_hangul_11172 (krc_char16_t ch)
{
	krc_uint32_t offset;

	krc_int32_t index;


	index = -1;


	//-----------------------------------------------------------------------
	// 완성형한글.한글2350자 
	//-----------------------------------------------------------------------

	// 0xB0A1u '가' - 0xC8FEu '힝'
	if (ch>=0xB0A1u && ch<=0xC8FEu)
	{
		offset = (ch-0xB0A0u)%0x0100u;
		if (offset>0u && offset<96u)
		{
			index = (ch-0xB0A0u)/0x0100u*94u + offset - 1u;
			return index;
		}
	}


	//-----------------------------------------------------------------------
	// 확장완성형한글.추가한글8822자
	//-----------------------------------------------------------------------

	// 5696 + 3108 + 18

	// 5696자
	// 0x8141u '갂' - 0xA0FEu '좤' 
	if (ch>=0x8141u && ch<=0xA0FEu)
	{
		offset = (ch-0x8140u)%0x0100u;
		if (offset>0u && offset<191u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040u) offset -= 12u;
			else if (offset>0x0020u) offset -= 6u;

			index  = (ch-0x8140u)/0x0100u*(192u-2u-6u-6u) + offset - 1u + 2350u;
			return index;
		}
	}

	// 3108자
	// 0xA141u '좥' - 0xC5A0u '힋' 
	if (ch>=0xA141u && ch<=0xC5A0u)
	{
		offset = (ch-0xA140u)%0x0100u;
		if (offset>0u && offset<97u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040) offset -= 12u;
			else if (offset>0x0020) offset -= 6u;

			index  = (ch-0xA140u)/0x0100u*(97u-6u-6u-1u) + offset - 1u + 5696u + 2350u;
			return index;
		}
	}

	// 18자
	// 0xC641u '힍' - 0xC652u '힣' 
	if (ch>=0xC641u && ch<=0xC652u)
	{
		index = ch - 0xC640u - 1u + 3108u + 5696u + 2350u;
		return index;
	}


	return index;
}

KRC_API krc_int32_t krc_cp949_index_special_1128 (krc_char16_t ch)
{
	krc_uint32_t offset;


	/*
	# 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	  12개영역 * 94문자 = 1128문자
	     0xA1A0u
	   + 0x0C00u(12*256)
	   = 0xFEA0u
	*/
	if (0xA1A1u<=ch && ch <= 0xACFEu)
	{
		offset = (ch-0xA1A0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xA1A0u)/0x0100u)*94u + offset -1u;
		}
	}

	return -1;
}

KRC_API krc_int32_t krc_cp949_index_hanja_4888 (krc_char16_t ch)
{
	krc_uint32_t offset;


	/*
	# 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	  52개영역 * 94문자 = 4888문자
	     0xCAA0u
	   + 0x3400u(52*256)
	   = 0xFEA0u
	*/
	if (0xCAA1u<=ch && ch <= 0xFDFEu)
	{
		offset = (ch-0xCAA0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xCAA0u)/0x0100u)*94u + offset - 1u;
		}
	}

	return -1;
}
