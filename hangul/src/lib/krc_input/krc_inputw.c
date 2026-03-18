/////////////////////////////////////////////////////////////////////////////
//
// File: krc_inputw.c
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
#include "../krc/krc.h"

#include "krc_input_config.h"
#include "krc_input_type.h"
#include "krc_input_key.h"
#include "krc_input_keyboard_type.h"
#include "krc_inputw_key_char_table.h"
#include "krc_inputw_table.h"
#include "krc_inputw.h"






/////////////////////////////////////////////////////////////////////////////
// 
// text buffer 조작 helper
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 문자열 길이 계산 (NULL terminator 이전까지의 글자 수, 최대 max_length)
//---------------------------------------------------------------------------
static krc_size_t krc_text_length_w(const krc_wchar_t* text, krc_size_t max_length)
{
	krc_size_t i;

	for (i = 0u; i < max_length; i++)
	{
		if (text[i] == 0x0000)
		{
			return i;
		}
	}

	return max_length;
}

//---------------------------------------------------------------------------
// 문자열에서 index 위치의 글자 하나를 삭제
//---------------------------------------------------------------------------
static krc_bool_t krc_text_remove_char_w(krc_wchar_t* text, krc_size_t text_length, krc_size_t max_length, krc_size_t index)
{
	krc_size_t i;

	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_text_length_w(text, max_length);
	}

	if (text_length == 0u)
	{
		return KRC_FALSE;
	}

	if (index >= text_length)
	{
		return KRC_FALSE;
	}

	for (i = index; i <= text_length; i++) /* include NULL terminator */
	{
		text[i] = text[i + 1u];
	}

	return KRC_TRUE;
}

//---------------------------------------------------------------------------
// 문자열의 index 위치에 글자 하나를 삽입
//---------------------------------------------------------------------------
static krc_bool_t krc_text_insert_char_w(krc_wchar_t* text, krc_size_t text_length, krc_size_t max_length, krc_size_t index, krc_wchar_t ch)
{
	krc_size_t i;

	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_text_length_w(text, max_length);
	}

	if (text_length >= max_length)
	{
		return KRC_FALSE;
	}

	if (index > text_length)
	{
		return KRC_FALSE;
	}

	for (i = text_length + 1u; i > index; i--) /* include NULL terminator */
	{
		text[i] = text[i - 1u];
	}

	text[index] = ch;

	return KRC_TRUE;
}

//---------------------------------------------------------------------------
// insert_mode 에 따라 삽입 또는 덮어쓰기
//---------------------------------------------------------------------------
static krc_bool_t krc_text_put_char_w(krc_wchar_t* text, krc_size_t text_length, krc_size_t max_length, krc_size_t index, krc_wchar_t ch, krc_bool_t insert_mode)
{
	if (insert_mode == KRC_TRUE)
	{
		return krc_text_insert_char_w(text, text_length, max_length, index, ch);
	}

	/* 덮어쓰기 모드 */
	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_text_length_w(text, max_length);
	}

	if (index > text_length)
	{
		return KRC_FALSE;
	}

	if (index == text_length) /* 문자열 끝 — 확장 필요 */
	{
		if (text_length >= max_length)
		{
			return KRC_FALSE;
		}
		text[index]        = ch;
		text[index + 1u]   = 0x0000;
	}
	else
	{
		text[index] = ch; /* 기존 글자 덮어쓰기 */
	}

	return KRC_TRUE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// 한글 음절 index helper
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 음절 코드 → (초성, 중성, 종성) 인덱스 분해
//---------------------------------------------------------------------------
static void krc_hangul_syllable_to_indices(krc_wchar_t code, int* choseong, int* jungseong, int* jongseong)
{
	int offset = (int)(code - 0xac00);
	*jongseong = offset % 28;
	*jungseong = (offset / 28) % 21;
	*choseong  = offset / (28 * 21);
}

//---------------------------------------------------------------------------
// (초성, 중성, 종성) 인덱스 → 음절 코드
//---------------------------------------------------------------------------
static krc_wchar_t krc_hangul_indices_to_syllable(int choseong, int jungseong, int jongseong)
{
	return 0xac00 + (krc_wchar_t)(choseong * 21 * 28 + jungseong * 28 + jongseong);
}

//---------------------------------------------------------------------------
// 자음 자모 여부: 0x3131 ~ 0x314e
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_is_consonant_jamo(krc_wchar_t code)
{
	return (0x3131 <= code && code <= 0x314e) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 모음 자모 여부: 0x314f ~ 0x3163
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_is_vowel_jamo(krc_wchar_t code)
{
	return (0x314f <= code && code <= 0x3163) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 겹받침 조합: jongseong_base + jongseong_add → 겹받침 인덱스 (-1 = 조합 불가)
//---------------------------------------------------------------------------
static int krc_hangul_try_jongseong_compound(int jongseong_base, int jongseong_add)
{
	switch (jongseong_base)
	{
	case 1: /* ㄱ */
		if (jongseong_add == 19) return  3; /* ㄱ+ㅅ=ㄳ */
		break;
	case 4: /* ㄴ */
		if (jongseong_add == 22) return  5; /* ㄴ+ㅈ=ㄵ */
		if (jongseong_add == 27) return  6; /* ㄴ+ㅎ=ㄶ */
		break;
	case 8: /* ㄹ */
		if (jongseong_add ==  1) return  9; /* ㄹ+ㄱ=ㄺ */
		if (jongseong_add == 16) return 10; /* ㄹ+ㅁ=ㄻ */
		if (jongseong_add == 17) return 11; /* ㄹ+ㅂ=ㄼ */
		if (jongseong_add == 19) return 12; /* ㄹ+ㅅ=ㄽ */
		if (jongseong_add == 25) return 13; /* ㄹ+ㅌ=ㄾ */
		if (jongseong_add == 26) return 14; /* ㄹ+ㅍ=ㄿ */
		if (jongseong_add == 27) return 15; /* ㄹ+ㅎ=ㅀ */
		break;
	case 17: /* ㅂ */
		if (jongseong_add == 19) return 18; /* ㅂ+ㅅ=ㅄ */
		break;
	default:
		break;
	}
	return -1;
}

//---------------------------------------------------------------------------
// 복합 중성 조합: jungseong_base + jungseong_add → 복합 중성 인덱스 (-1 = 조합 불가)
//---------------------------------------------------------------------------
static int krc_hangul_try_jungseong_compound(int jungseong_base, int jungseong_add)
{
	switch (jungseong_base)
	{
	case 8: /* ㅗ */
		if (jungseong_add ==  0) return  9; /* ㅗ+ㅏ=ㅘ */
		if (jungseong_add ==  1) return 10; /* ㅗ+ㅐ=ㅙ */
		if (jungseong_add == 20) return 11; /* ㅗ+ㅣ=ㅚ */
		break;
	case 13: /* ㅜ */
		if (jungseong_add ==  4) return 14; /* ㅜ+ㅓ=ㅝ */
		if (jungseong_add ==  5) return 15; /* ㅜ+ㅔ=ㅞ */
		if (jungseong_add == 20) return 16; /* ㅜ+ㅣ=ㅟ */
		break;
	case 18: /* ㅡ */
		if (jungseong_add == 20) return 19; /* ㅡ+ㅣ=ㅢ */
		break;
	default:
		break;
	}
	return -1;
}

//---------------------------------------------------------------------------
// 겹받침 분리: jongseong_index → (*jongseong_base, *jongseong_add)  반환: TRUE = 겹받침
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_try_jongseong_decompose(int jongseong_index, int* jongseong_base, int* jongseong_add)
{
	switch (jongseong_index)
	{
	case  3: *jongseong_base =  1; *jongseong_add = 19; return KRC_TRUE; /* ㄳ → ㄱ+ㅅ */
	case  5: *jongseong_base =  4; *jongseong_add = 22; return KRC_TRUE; /* ㄵ → ㄴ+ㅈ */
	case  6: *jongseong_base =  4; *jongseong_add = 27; return KRC_TRUE; /* ㄶ → ㄴ+ㅎ */
	case  9: *jongseong_base =  8; *jongseong_add =  1; return KRC_TRUE; /* ㄺ → ㄹ+ㄱ */
	case 10: *jongseong_base =  8; *jongseong_add = 16; return KRC_TRUE; /* ㄻ → ㄹ+ㅁ */
	case 11: *jongseong_base =  8; *jongseong_add = 17; return KRC_TRUE; /* ㄼ → ㄹ+ㅂ */
	case 12: *jongseong_base =  8; *jongseong_add = 19; return KRC_TRUE; /* ㄽ → ㄹ+ㅅ */
	case 13: *jongseong_base =  8; *jongseong_add = 25; return KRC_TRUE; /* ㄾ → ㄹ+ㅌ */
	case 14: *jongseong_base =  8; *jongseong_add = 26; return KRC_TRUE; /* ㄿ → ㄹ+ㅍ */
	case 15: *jongseong_base =  8; *jongseong_add = 27; return KRC_TRUE; /* ㅀ → ㄹ+ㅎ */
	case 18: *jongseong_base = 17; *jongseong_add = 19; return KRC_TRUE; /* ㅄ → ㅂ+ㅅ */
	default:
		return KRC_FALSE;
	}
}

//---------------------------------------------------------------------------
// 복합 중성 분리: jungseong_index → (*jungseong_base, *jungseong_add)  반환: TRUE = 복합 중성
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_try_jungseong_decompose(int jungseong_index, int* jungseong_base, int* jungseong_add)
{
	switch (jungseong_index)
	{
	case  9: *jungseong_base =  8; *jungseong_add =  0; return KRC_TRUE; /* ㅘ → ㅗ+ㅏ */
	case 10: *jungseong_base =  8; *jungseong_add =  1; return KRC_TRUE; /* ㅙ → ㅗ+ㅐ */
	case 11: *jungseong_base =  8; *jungseong_add = 20; return KRC_TRUE; /* ㅚ → ㅗ+ㅣ */
	case 14: *jungseong_base = 13; *jungseong_add =  4; return KRC_TRUE; /* ㅝ → ㅜ+ㅓ */
	case 15: *jungseong_base = 13; *jungseong_add =  5; return KRC_TRUE; /* ㅞ → ㅜ+ㅔ */
	case 16: *jungseong_base = 13; *jungseong_add = 20; return KRC_TRUE; /* ㅟ → ㅜ+ㅣ */
	case 19: *jungseong_base = 18; *jungseong_add = 20; return KRC_TRUE; /* ㅢ → ㅡ+ㅣ */
	default: return KRC_FALSE;
	}
}

//---------------------------------------------------------------------------
// 음절 + 자음 → 종성 추가 또는 겹받침  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_add_consonant_to_syllable(int choseong, int jungseong, int jongseong,	krc_wchar_t element_code, krc_wchar_t* result_code)
{
	int jongseong_add = _krc_inputw_hangul_jongseong_index_table[element_code - 0x3131];
	int compound;

	if (jongseong == 0)
	{
		/* 종성 없음: 가 + ㄱ = 각 */
		if (jongseong_add != -1)
		{
			*result_code = krc_hangul_indices_to_syllable(choseong, jungseong, jongseong_add);
			return KRC_TRUE;
		}
	}
	else
	{
		/* 겹받침 시도 */
		compound = krc_hangul_try_jongseong_compound(jongseong, jongseong_add);
		if (compound != -1)
		{
			*result_code = krc_hangul_indices_to_syllable(choseong, jungseong, compound);
			return KRC_TRUE;
		}
	}
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// 음절(종성 없음) + 모음 → 복합 중성  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_add_vowel_to_syllable(int choseong, int jungseong,	krc_wchar_t element_code, krc_wchar_t* result_code)
{
	int jungseong_add = (int)(element_code - 0x314f);
	int compound = krc_hangul_try_jungseong_compound(jungseong, jungseong_add);

	if (compound != -1)
	{
		*result_code = krc_hangul_indices_to_syllable(choseong, compound, 0);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// 단독 자음(초성) + 모음 → 새 음절  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_compose_jamo(krc_wchar_t consonant, krc_wchar_t vowel,	krc_wchar_t* result_code)
{
	int choseong_index  = _krc_inputw_hangul_choseong_index_table[consonant - 0x3131];

	if (choseong_index != -1)
	{
		int jungseong_index = (int)(vowel - 0x314f);
		*result_code   = krc_hangul_indices_to_syllable(choseong_index, jungseong_index, 0);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// krc_hangul_get_composing_code()
//
//   조합 중인 코드(composing_code)에 낱글자(element_code)를 추가하여
//   조합된 코드(*result_code)를 만든다.
//
//   반환값: KRC_TRUE  — 조합 성공 (조합이 계속 진행중)
//           KRC_FALSE — 조합 불가 (새 글자 시작)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_get_composing_code(krc_wchar_t composing_code, krc_wchar_t element_code, krc_wchar_t* result_code)
{
	int choseong, jungseong, jongseong;

	if (0x3131 <= element_code && element_code <= 0x3163)
	{
		//===================================================================
		// composing_code 가 초성+중성 이상 조합된 상태
		//===================================================================
		if (0xac00 <= composing_code && composing_code <= 0xd7a3)
		{
			krc_hangul_syllable_to_indices(composing_code, &choseong, &jungseong, &jongseong);

			if (krc_hangul_is_consonant_jamo(element_code))
			{
				if (krc_hangul_add_consonant_to_syllable(choseong, jungseong, jongseong, element_code, result_code))
					return KRC_TRUE;
			}
			else if (jongseong == 0)
			{
				if (krc_hangul_add_vowel_to_syllable(choseong, jungseong, element_code, result_code))
					return KRC_TRUE;
			}
		}

		//===================================================================
		// composing_code 가 자음 낱글자 — 모음과 결합하여 글자 완성
		//===================================================================
		else if (krc_hangul_is_consonant_jamo(composing_code))
		{
			if (krc_hangul_is_vowel_jamo(element_code))
			{
				if (krc_hangul_compose_jamo(composing_code, element_code, result_code))
					return KRC_TRUE;
			}
		}

		//===================================================================
		// 조합 없음 — 첫 번째 낱글자
		//===================================================================
		else
		{
			*result_code = element_code;
			return KRC_TRUE;
		}
	}

	*result_code = element_code;
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// krc_hangul_pop_back_code()
//
//   조합된 한글 코드(composing_code)에서 마지막 입력 요소를 분리한다.
//   composing_code = *result_code + *pop_back_code
//
//   반환값: KRC_TRUE  — 분리 성공 (*result_code 에 조합 잔여분)
//           KRC_FALSE — 분리 불가 (*result_code = 0, *pop_back_code = composing_code)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangul_pop_back_code(krc_wchar_t composing_code, krc_wchar_t* result_code, krc_wchar_t* pop_back_code)
{
	int choseong, jungseong, jongseong;
	int base, add_idx;

	if (0xac00 <= composing_code && composing_code <= 0xd7a3)
	{
		krc_hangul_syllable_to_indices(composing_code, &choseong, &jungseong, &jongseong);

		//===================================================================
		// 종성 없음
		//===================================================================
		if (jongseong == 0)
		{
			if (krc_hangul_try_jungseong_decompose(jungseong, &base, &add_idx))
			{
				/* 복합 중성 분리: [초성+단순중성] + [단순모음] */
				*result_code   = krc_hangul_indices_to_syllable(choseong, base, 0);
				*pop_back_code = 0x314f + (krc_wchar_t)add_idx;
			}
			else
			{
				/* 단순 중성 분리: [초성] + [중성모음] */
				*result_code   = _krc_inputw_hangul_choseong_code_table[choseong];
				*pop_back_code = 0x314f + (krc_wchar_t)jungseong;
			}
		}

		//===================================================================
		// 종성 있음
		//===================================================================
		else
		{
			if (krc_hangul_try_jongseong_decompose(jongseong, &base, &add_idx))
			{
				/* 겹받침 분리: [초성+중성+단자음종성] + [겹받침의 두번째 자음] */
				*result_code   = krc_hangul_indices_to_syllable(choseong, jungseong, base);
				*pop_back_code = _krc_inputw_hangul_jongseong_code_table[add_idx];
			}
			else
			{
				/* 단받침 분리: [초성+중성] + [종성자음] */
				*result_code   = krc_hangul_indices_to_syllable(choseong, jungseong, 0);
				*pop_back_code = _krc_inputw_hangul_jongseong_code_table[jongseong];
			}
		}

		return KRC_TRUE;
	}

	*result_code   = 0x0000;
	*pop_back_code = composing_code;
	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// 한글 조합 중
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 조합 중 여부 확인
//---------------------------------------------------------------------------
static krc_bool_t krc_input_is_composing(const krc_inputw_t* ctx)
{
	return ctx->hangul_composing;
}

//---------------------------------------------------------------------------
// 조합 시작
//---------------------------------------------------------------------------
static void krc_input_composing_start(krc_inputw_t* ctx)
{
	ctx->hangul_composing = KRC_TRUE;
}

//---------------------------------------------------------------------------
// 조합 중단
//---------------------------------------------------------------------------
static void krc_input_composing_stop(krc_inputw_t* ctx)
{
	ctx->hangul_composing = KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// 커서
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 커서 위치(cursor_line, cursor_column) 갱신
//---------------------------------------------------------------------------
static void krc_input_cursor_update_pos(krc_inputw_t* ctx)
{
	const krc_wchar_t* text = ctx->buffer_pointer;
	krc_size_t pos    = 0u;
	krc_size_t line   = 0u;
	krc_size_t column = 0u;

	while (pos < ctx->cursor && text[pos] != 0x0000)
	{
		if (text[pos] == '\n')
		{
			line++;
			column = 0u;
		}
		else if (text[pos] != '\r')
		{
			column++;
		}
		pos++;
	}
	ctx->cursor_line   = line;
	ctx->cursor_column = column;
}

//---------------------------------------------------------------------------
// 커서 함수
//---------------------------------------------------------------------------
static void krc_input_cursor_advance(krc_inputw_t* ctx)
{
	if (ctx->cursor < ctx->buffer_size - 1u)
	{
		ctx->cursor++;
	}
}

static void krc_input_cursor_back(krc_inputw_t* ctx)
{
	if (ctx->cursor > 0u)
	{
		ctx->cursor--;
	}
}

static void krc_input_cursor_set(krc_inputw_t* ctx, krc_size_t pos)
{
	if (pos <= ctx->buffer_size - 1u)
	{
		ctx->cursor = pos;
	}
}

static void krc_input_cursor_reset(krc_inputw_t* ctx)
{
	ctx->cursor = 0u;
}

static void krc_input_cursor_set_end(krc_inputw_t* ctx)
{
	ctx->cursor = krc_text_length_w(ctx->buffer_pointer, ctx->buffer_size);
}





/////////////////////////////////////////////////////////////////////////////
// 
// 입력
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 한글 조합 종료 — 커서 전진 후 composing 해제
//---------------------------------------------------------------------------
static void krc_input_commit_composing(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
		krc_input_cursor_advance(ctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
// 
// put_char 서브 핸들러
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 조합 중 + 모음 입력 실패
//---------------------------------------------------------------------------
static void krc_input_put_char_fail_vowel(krc_inputw_t* ctx, krc_wchar_t char_code, krc_wchar_t composing_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;
	krc_wchar_t  result_code;
	krc_wchar_t  pop_back_code;
	krc_wchar_t  new_composing;

	krc_hangul_pop_back_code(composing_code, &result_code, &pop_back_code);

	if (krc_hangul_is_consonant_jamo(pop_back_code))
	{
		/* 갓 + ㅏ = 가 + 사 */
		text[ctx->cursor] = result_code;
		krc_input_cursor_advance(ctx);
		krc_hangul_get_composing_code(pop_back_code, char_code, &new_composing);
		krc_input_composing_stop(ctx);
		if (krc_text_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, new_composing, ctx->insert_mode))
		{
			krc_input_composing_start(ctx);
		}
	}
	else
	{
		/* 가 + ㅏ = 가 + ㅏ  (조합 불가) */
		krc_input_cursor_advance(ctx);
		krc_input_composing_stop(ctx);
		if (krc_text_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, result_code, ctx->insert_mode))
		{
			if (krc_hangul_is_consonant_jamo(result_code))
			{
				krc_input_composing_start(ctx);
			}
		}
	}
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 조합 중 + 자음 입력 실패
//---------------------------------------------------------------------------
static void krc_input_put_char_fail_consonant(krc_inputw_t* ctx,
	krc_wchar_t composing_code, krc_wchar_t result_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	/* 각 + ㄱ = 각 + ㄱ */
	text[ctx->cursor] = composing_code;
	krc_input_cursor_advance(ctx);
	krc_input_composing_stop(ctx);
	if (krc_text_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, result_code, ctx->insert_mode))
	{
		if (krc_hangul_is_consonant_jamo(result_code))
		{
			krc_input_composing_start(ctx);
		}
	}
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 한글 조합 진행 중
//---------------------------------------------------------------------------
static void krc_input_put_char_hangul_composing(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_wchar_t  composing_code = text[ctx->cursor];
	krc_wchar_t  result_code;

	if (krc_hangul_get_composing_code(composing_code, char_code, &result_code) == KRC_TRUE)
	{
		krc_input_composing_start(ctx);
		/* 조합 성공: 가 + ㄱ = 각 */
		text[ctx->cursor] = result_code;
	}
	else
	{
		krc_input_composing_stop(ctx);
		/* 조합 실패 */
		if (krc_hangul_is_vowel_jamo(char_code))
		{
			krc_input_put_char_fail_vowel(ctx, char_code, composing_code);
		}
		else
		{
			krc_input_put_char_fail_consonant(ctx, composing_code, result_code);
		}
	}
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 한글 조합 시작
//---------------------------------------------------------------------------
static void krc_input_put_char_hangul_new(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	if (krc_text_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, char_code, ctx->insert_mode))
	{
		if (krc_hangul_is_consonant_jamo(char_code))
		{
			krc_input_composing_start(ctx);
		}
		else
		{
			krc_input_composing_stop(ctx);
			krc_input_cursor_advance(ctx);
		}
	}
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 비한글 문자(영문/숫자/특수문자 등)
//---------------------------------------------------------------------------
static void krc_input_put_char_non_hangul(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	krc_input_commit_composing(ctx);

	if (krc_text_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, char_code, ctx->insert_mode))
	{
		krc_input_cursor_advance(ctx);
	}
	krc_input_composing_stop(ctx);
}





/////////////////////////////////////////////////////////////////////////////
// 
// 키 핸들러
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 키 핸들러 — Backspace
//---------------------------------------------------------------------------
static void krc_input_key_backspace(krc_inputw_t* ctx)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;
	krc_wchar_t  composing_code = text[ctx->cursor];
	krc_wchar_t  result_code;
	krc_wchar_t  pop_back_code;

	if (ctx->cursor == 0u && krc_input_is_composing(ctx) == KRC_FALSE)
	{
		return;
	}

	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_hangul_pop_back_code(composing_code, &result_code, &pop_back_code);
		if (result_code != 0x0000)
		{
			text[ctx->cursor] = result_code;
			krc_input_composing_start(ctx);
		}
		else
		{
			krc_text_remove_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor);
			krc_input_composing_stop(ctx);
		}
	}
	else
	{
		if (ctx->cursor > 0u)
		{
			krc_input_cursor_back(ctx);
			krc_text_remove_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor);
		}
		krc_input_composing_stop(ctx);
	}
	ctx->length = krc_text_length_w(text, ctx->buffer_size);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Enter
//---------------------------------------------------------------------------
static void krc_input_key_enter(krc_inputw_t* ctx)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	krc_input_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		if (krc_text_insert_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, '\r'))
		{
			krc_input_cursor_advance(ctx);
		}
		if (krc_text_insert_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, '\n'))
		{
			krc_input_cursor_advance(ctx);
		}
		ctx->length = krc_text_length_w(text, ctx->buffer_size);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Delete
//---------------------------------------------------------------------------
static void krc_input_key_delete(krc_inputw_t* ctx)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;
	krc_size_t   text_len;

	krc_input_commit_composing(ctx);

	text_len = krc_text_length_w(text, ctx->buffer_size);
	if (ctx->cursor < text_len)
	{
		krc_text_remove_char_w(text, text_len, max_length, ctx->cursor);
	}
	ctx->length = krc_text_length_w(text, ctx->buffer_size);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Left
//---------------------------------------------------------------------------
static void krc_input_key_left(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	else if (ctx->cursor > 0u)
	{
		krc_input_cursor_back(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Right
//---------------------------------------------------------------------------
static void krc_input_key_right(krc_inputw_t* ctx)
{
	krc_wchar_t* text = ctx->buffer_pointer;
	krc_size_t   text_len;

	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
		krc_input_cursor_advance(ctx);
	}
	else
	{
		text_len = krc_text_length_w(text, ctx->buffer_size);
		if (ctx->cursor < text_len)
		{
			krc_input_cursor_advance(ctx);
		}
	}
}

//---------------------------------------------------------------------------
// 다중라인 커서 이동 — 윗 줄 목표 행/열 위치로 이동
//---------------------------------------------------------------------------
static void krc_input_move_cursor_up(krc_inputw_t* ctx)
{
	krc_wchar_t* text        = ctx->buffer_pointer;
	krc_size_t   target_line = ctx->cursor_line - 1u;
	krc_size_t   target_col  = ctx->cursor_column;
	krc_size_t   pos    = 0u;
	krc_size_t   line   = 0u;
	krc_size_t   column = 0u;

	krc_input_cursor_reset(ctx);
	while (text[pos] != 0x0000)
	{
		if (line == target_line && column == target_col)
		{
			krc_input_cursor_set(ctx, pos);
			break;
		}
		if (text[pos] == '\n')
		{
			line++;
			column = 0u;
		}
		else if (text[pos] != '\r')
		{
			if (line == target_line)
			{
				column++;
			}
		}
		pos++;
	}
}

//---------------------------------------------------------------------------
// 다중라인 커서 이동 — 아랫 줄 목표 행/열 위치로 이동
//---------------------------------------------------------------------------
static void krc_input_move_cursor_down(krc_inputw_t* ctx)
{
	krc_wchar_t* text        = ctx->buffer_pointer;
	krc_size_t   target_line = ctx->cursor_line + 1u;
	krc_size_t   target_col  = ctx->cursor_column;
	krc_size_t   pos    = 0u;
	krc_size_t   line   = 0u;
	krc_size_t   column = 0u;
	krc_size_t   found  = 0u;

	while (text[pos] != 0x0000)
	{
		if (line == target_line && column == target_col)
		{
			krc_input_cursor_set(ctx, pos);
			found = 1u;
			break;
		}
		if (text[pos] == '\n')
		{
			if (line == target_line) /* 줄 끝 도달 */
			{
				krc_input_cursor_set(ctx, pos);
				found = 1u;
				break;
			}
			line++;
			column = 0u;
		}
		else if (text[pos] != '\r')
		{
			if (line == target_line)
			{
				column++;
			}
		}
		pos++;
	}
	if (found == 0u && line == target_line)
	{
		krc_input_cursor_set(ctx, pos); /* 마지막 줄 끝 */
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Up
//---------------------------------------------------------------------------
static void krc_input_key_up(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	if (ctx->multiline == KRC_TRUE && ctx->cursor_line > 0u)
	{
		krc_input_move_cursor_up(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Down
//---------------------------------------------------------------------------
static void krc_input_key_down(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	if (ctx->multiline == KRC_TRUE)
	{
		krc_input_move_cursor_down(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Home
//---------------------------------------------------------------------------
static void krc_input_key_home(krc_inputw_t* ctx)
{
	krc_wchar_t* text = ctx->buffer_pointer;

	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	if (ctx->multiline == KRC_TRUE)
	{
		/* 현재 줄의 맨 앞으로 이동 */
		while (ctx->cursor > 0u && text[ctx->cursor - 1u] != '\n')
		{
			krc_input_cursor_back(ctx);
		}
	}
	else
	{
		krc_input_cursor_reset(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — End
//---------------------------------------------------------------------------
static void krc_input_key_end(krc_inputw_t* ctx)
{
	krc_wchar_t* text = ctx->buffer_pointer;
	krc_size_t   text_len;

	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	if (ctx->multiline == KRC_TRUE)
	{
		/* 현재 줄의 맨 끝으로 이동 */
		text_len = krc_text_length_w(text, ctx->buffer_size);
		while (ctx->cursor < text_len
			&& text[ctx->cursor] != '\r'
			&& text[ctx->cursor] != '\n')
		{
			krc_input_cursor_advance(ctx);
		}
	}
	else
	{
		krc_input_cursor_set_end(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Insert (삽입/덮어쓰기 모드 전환)
//---------------------------------------------------------------------------
static void krc_input_key_insert(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	ctx->insert_mode = (ctx->insert_mode == KRC_TRUE) ? KRC_FALSE : KRC_TRUE;
}

//---------------------------------------------------------------------------
// 키 핸들러 — 한/영 전환 (HANGUL = RALT)
//---------------------------------------------------------------------------
static void krc_input_key_hangul(krc_inputw_t* ctx)
{
	if (krc_input_is_composing(ctx) == KRC_TRUE)
	{
		krc_input_composing_stop(ctx);
	}
	if (ctx->keyboard_type == KRC_INPUT_KEYBOARD_TYPE_LATIN)
	{
		ctx->keyboard_type = KRC_INPUT_KEYBOARD_TYPE_HANGUL;
	}
	else
	{
		ctx->keyboard_type = KRC_INPUT_KEYBOARD_TYPE_LATIN;
	}
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_inputw_init(krc_inputw_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size, krc_bool_t multiline)
{
	ctx->buffer_pointer = buffer;
	ctx->buffer_size    = buffer_size;
	ctx->multiline      = multiline;
	ctx->length         = 0u;
	ctx->cursor         = 0u;
	ctx->cursor_line    = 0u;
	ctx->cursor_column  = 0u;
	ctx->shift_state    = KRC_FALSE;
	ctx->insert_mode    = KRC_TRUE;
	ctx->keyboard_type   = KRC_INPUT_KEYBOARD_TYPE_LATIN;
	krc_input_composing_stop(ctx);

	if (buffer != (krc_wchar_t*)0 && buffer_size > 0u)
	{
		buffer[0] = 0x0000;
	}
}

//---------------------------------------------------------------------------
// krc_inputw_put_char()
//
//   유니코드 문자 하나를 현재 커서 위치에 삽입한다.
//   한글 낱글자(0x3131~0x3163)인 경우 한글 조합을 수행한다.
//---------------------------------------------------------------------------
KRC_API void krc_inputw_put_char(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text = ctx->buffer_pointer;

	if (text == (krc_wchar_t*)0 || ctx->buffer_size == 0u)
	{
		return;
	}

	//=======================================================================
	// 한글 낱글자 (0x3131 ~ 0x3163)
	//=======================================================================
	if (0x3131 <= char_code && char_code <= 0x3163)
	{
		if (krc_input_is_composing(ctx) == KRC_TRUE)
		{
			krc_input_put_char_hangul_composing(ctx, char_code);
		}
		else
		{
			krc_input_put_char_hangul_new(ctx, char_code);
		}
	}
	//=======================================================================
	// 한글 낱글자가 아닌 경우 (영문, 숫자, 특수문자 등)
	//=======================================================================
	else
	{
		krc_input_put_char_non_hangul(ctx, char_code);
	}

	ctx->length = krc_text_length_w(text, ctx->buffer_size);
	krc_input_cursor_update_pos(ctx);
}

//---------------------------------------------------------------------------
// krc_inputw_put_key()
//
//   KRC_INPUT_KEY_* 키 입력을 처리한다.
//   문자 키(GRAVE~SPACE 범위)는 put_char()에 위임하고,
//   특수 키(방향키, BACKSPACE, ENTER 등)는 내부에서 처리한다.
//---------------------------------------------------------------------------
KRC_API void krc_inputw_put_key(krc_inputw_t* ctx, krc_uint32_t key)
{
	krc_wchar_t* text     = ctx->buffer_pointer;
	krc_wchar_t  char_code;

	if (text == (krc_wchar_t*)0 || ctx->buffer_size == 0u)
	{
		return;
	}


	//=======================================================================
	// 문자 키 처리 — put_char() 에 위임
	// keyboard_type × shift 조합으로 4개 테이블 중 하나를 선택
	// KRC_INPUT_KEY_1(1) ~ KRC_INPUT_KEY_SPACE(48) 범위
	//=======================================================================
	krc_uint32_t key_code = KRC_INPUT_KEY_CODE(key);
	krc_uint32_t key_modifier = KRC_INPUT_KEY_MODIFIER(key);
	krc_bool_t shift = KRC_FALSE;
	
	if ( (key_modifier == KRC_INPUT_KEY_LSHIFT) || (key_modifier == KRC_INPUT_KEY_RSHIFT) )
	{
		shift = KRC_TRUE;
	}
	if (ctx->shift_state == KRC_TRUE)
	{
		shift = KRC_TRUE;
	}

	if (key_code >= KRC_INPUT_KEY_1 && key_code <= KRC_INPUT_KEY_SPACE)
	{
		char_code = krc_inputw_key_char(key_code - KRC_INPUT_KEY_1, ctx->keyboard_type, shift);
		if (char_code != 0x0000u)
		{
			krc_inputw_put_char(ctx, char_code);
			return;
		}
	}

	//=======================================================================
	// 특수 키 처리
	//=======================================================================
	switch (key)
	{
	case KRC_INPUT_KEY_BACKSPACE: krc_input_key_backspace(ctx); break;
	case KRC_INPUT_KEY_ENTER:     krc_input_key_enter(ctx);     break;
	case KRC_INPUT_KEY_TAB:       krc_inputw_put_char(ctx, '\t'); return;

	case KRC_INPUT_KEY_HANGUL:    krc_input_key_hangul(ctx);    break;

	case KRC_INPUT_KEY_INSERT:    krc_input_key_insert(ctx);    break;
	case KRC_INPUT_KEY_HOME:      krc_input_key_home(ctx);      break;
	case KRC_INPUT_KEY_DELETE:    krc_input_key_delete(ctx);    break;
	case KRC_INPUT_KEY_END:       krc_input_key_end(ctx);       break;

	case KRC_INPUT_KEY_LEFT:      krc_input_key_left(ctx);      break;
	case KRC_INPUT_KEY_RIGHT:     krc_input_key_right(ctx);     break;
	case KRC_INPUT_KEY_UP:        krc_input_key_up(ctx);        break;
	case KRC_INPUT_KEY_DOWN:      krc_input_key_down(ctx);      break;
	default: break;
	}

	krc_input_cursor_update_pos(ctx);
}

KRC_API void krc_inputw_shift(krc_inputw_t* ctx, krc_bool_t shift)
{
	ctx->shift_state = shift;
}
