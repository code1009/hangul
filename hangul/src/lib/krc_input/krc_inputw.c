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
#include "krc_input_key_mode.h"
#include "krc_inputw_key_char_table.h"
#include "krc_inputw_table.h"
#include "krc_inputw.h"





/////////////////////////////////////////////////////////////////////////////
//
// 유니코드 상수
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_WCHAR_NULL         0x0000 // 와이드 문자 NULL terminator 
#define KRC_HANGULW_CHAR_BASE  0xAC00 // 한글 음절 시작: 가          
#define KRC_HANGULW_CHAR_END   0xD7A3 // 한글 음절 끝:   힣          
#define KRC_HANGULW_JAEUM_BASE 0x3131 // 자음/자모 시작: ㄱ          
#define KRC_HANGULW_JAEUM_END  0x314E // 자음 끝:        ㅎ          
#define KRC_HANGULW_MOEUM_BASE 0x314F // 모음 시작:      ㅏ          
#define KRC_HANGULW_MOEUM_END  0x3163 // 모음/자모 끝:   ㅣ          





/////////////////////////////////////////////////////////////////////////////
// 
// 한글 음절
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 음절 코드 → (초성, 중성, 종성) 인덱스 분해
//---------------------------------------------------------------------------
static void krc_hangulw_code_to_index(krc_wchar_t code, int* choseong, int* jungseong, int* jongseong)
{
	int offset = (int)(code - KRC_HANGULW_CHAR_BASE);
	*jongseong = offset % 28;
	*jungseong = (offset / 28) % 21;
	*choseong  = offset / (28 * 21);
}

//---------------------------------------------------------------------------
// (초성, 중성, 종성) 인덱스 → 음절 코드
//---------------------------------------------------------------------------
static krc_wchar_t krc_hangulw_index_to_code(int choseong, int jungseong, int jongseong)
{
	return KRC_HANGULW_CHAR_BASE + (krc_wchar_t)(choseong * 21 * 28 + jungseong * 28 + jongseong);
}

//===========================================================================
//---------------------------------------------------------------------------
// 겹받침 조합: jongseong_base + jongseong_additional → 겹받침 인덱스 (-1 = 조합 불가)
//---------------------------------------------------------------------------
static int krc_hangulw_compose_jongseong_index(int jongseong_base, int jongseong_additional)
{
	switch (jongseong_base)
	{
	case 1: /* ㄱ */
		if (jongseong_additional == 19) return  3; /* ㄱ+ㅅ=ㄳ */
		break;
	case 4: /* ㄴ */
		if (jongseong_additional == 22) return  5; /* ㄴ+ㅈ=ㄵ */
		if (jongseong_additional == 27) return  6; /* ㄴ+ㅎ=ㄶ */
		break;
	case 8: /* ㄹ */
		if (jongseong_additional ==  1) return  9; /* ㄹ+ㄱ=ㄺ */
		if (jongseong_additional == 16) return 10; /* ㄹ+ㅁ=ㄻ */
		if (jongseong_additional == 17) return 11; /* ㄹ+ㅂ=ㄼ */
		if (jongseong_additional == 19) return 12; /* ㄹ+ㅅ=ㄽ */
		if (jongseong_additional == 25) return 13; /* ㄹ+ㅌ=ㄾ */
		if (jongseong_additional == 26) return 14; /* ㄹ+ㅍ=ㄿ */
		if (jongseong_additional == 27) return 15; /* ㄹ+ㅎ=ㅀ */
		break;
	case 17: /* ㅂ */
		if (jongseong_additional == 19) return 18; /* ㅂ+ㅅ=ㅄ */
		break;
	default:
		break;
	}
	return -1;
}

//---------------------------------------------------------------------------
// 복합 중성 조합: jungseong_base + jungseong_additional → 복합 중성 인덱스 (-1 = 조합 불가)
//---------------------------------------------------------------------------
static int krc_hangulw_compose_jungseong_index(int jungseong_base, int jungseong_additional)
{
	switch (jungseong_base)
	{
	case 8: /* ㅗ */
		if (jungseong_additional ==  0) return  9; /* ㅗ+ㅏ=ㅘ */
		if (jungseong_additional ==  1) return 10; /* ㅗ+ㅐ=ㅙ */
		if (jungseong_additional == 20) return 11; /* ㅗ+ㅣ=ㅚ */
		break;
	case 13: /* ㅜ */
		if (jungseong_additional ==  4) return 14; /* ㅜ+ㅓ=ㅝ */
		if (jungseong_additional ==  5) return 15; /* ㅜ+ㅔ=ㅞ */
		if (jungseong_additional == 20) return 16; /* ㅜ+ㅣ=ㅟ */
		break;
	case 18: /* ㅡ */
		if (jungseong_additional == 20) return 19; /* ㅡ+ㅣ=ㅢ */
		break;
	default:
		break;
	}
	return -1;
}

//---------------------------------------------------------------------------
// 겹받침 분리: jongseong_index → (*jongseong_base, *jongseong_add)  반환: TRUE = 겹받침
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_decompose_jongseong_index(int jongseong_index, int* jongseong_base, int* jongseong_additional)
{
	switch (jongseong_index)
	{
	case  3: *jongseong_base =  1; *jongseong_additional = 19; return KRC_TRUE; /* ㄳ → ㄱ+ㅅ */
	case  5: *jongseong_base =  4; *jongseong_additional = 22; return KRC_TRUE; /* ㄵ → ㄴ+ㅈ */
	case  6: *jongseong_base =  4; *jongseong_additional = 27; return KRC_TRUE; /* ㄶ → ㄴ+ㅎ */
	case  9: *jongseong_base =  8; *jongseong_additional =  1; return KRC_TRUE; /* ㄺ → ㄹ+ㄱ */
	case 10: *jongseong_base =  8; *jongseong_additional = 16; return KRC_TRUE; /* ㄻ → ㄹ+ㅁ */
	case 11: *jongseong_base =  8; *jongseong_additional = 17; return KRC_TRUE; /* ㄼ → ㄹ+ㅂ */
	case 12: *jongseong_base =  8; *jongseong_additional = 19; return KRC_TRUE; /* ㄽ → ㄹ+ㅅ */
	case 13: *jongseong_base =  8; *jongseong_additional = 25; return KRC_TRUE; /* ㄾ → ㄹ+ㅌ */
	case 14: *jongseong_base =  8; *jongseong_additional = 26; return KRC_TRUE; /* ㄿ → ㄹ+ㅍ */
	case 15: *jongseong_base =  8; *jongseong_additional = 27; return KRC_TRUE; /* ㅀ → ㄹ+ㅎ */
	case 18: *jongseong_base = 17; *jongseong_additional = 19; return KRC_TRUE; /* ㅄ → ㅂ+ㅅ */
	default:
		return KRC_FALSE;
	}
}

//---------------------------------------------------------------------------
// 복합 중성 분리: jungseong_index → (*jungseong_base, *jungseong_add)  반환: TRUE = 복합 중성
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_decompose_jungseong_index(int jungseong_index, int* jungseong_base, int* jungseong_additional)
{
	switch (jungseong_index)
	{
	case  9: *jungseong_base =  8; *jungseong_additional =  0; return KRC_TRUE; /* ㅘ → ㅗ+ㅏ */
	case 10: *jungseong_base =  8; *jungseong_additional =  1; return KRC_TRUE; /* ㅙ → ㅗ+ㅐ */
	case 11: *jungseong_base =  8; *jungseong_additional = 20; return KRC_TRUE; /* ㅚ → ㅗ+ㅣ */
	case 14: *jungseong_base = 13; *jungseong_additional =  4; return KRC_TRUE; /* ㅝ → ㅜ+ㅓ */
	case 15: *jungseong_base = 13; *jungseong_additional =  5; return KRC_TRUE; /* ㅞ → ㅜ+ㅔ */
	case 16: *jungseong_base = 13; *jungseong_additional = 20; return KRC_TRUE; /* ㅟ → ㅜ+ㅣ */
	case 19: *jungseong_base = 18; *jungseong_additional = 20; return KRC_TRUE; /* ㅢ → ㅡ+ㅣ */
	default: return KRC_FALSE;
	}
}

//===========================================================================
//---------------------------------------------------------------------------
// 한글11172자 음절 여부
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_is_char(krc_wchar_t code)
{
	return (KRC_HANGULW_CHAR_BASE <= code && code <= KRC_HANGULW_CHAR_END) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 한글 자모 여부
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_is_jamo(krc_wchar_t code)
{
	return (KRC_HANGULW_JAEUM_BASE <= code && code <= KRC_HANGULW_MOEUM_END) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 자음 자모 여부
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_is_jaeum(krc_wchar_t code)
{
	return (KRC_HANGULW_JAEUM_BASE <= code && code <= KRC_HANGULW_JAEUM_END) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 모음 자모 여부
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_is_moeum(krc_wchar_t code)
{
	return (KRC_HANGULW_MOEUM_BASE <= code && code <= KRC_HANGULW_MOEUM_END) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 음절 코드가 종성 포함 여부
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_has_jongseong(krc_wchar_t base_code)
{
	int choseong, jungseong, jongseong;
	krc_hangulw_code_to_index(base_code, &choseong, &jungseong, &jongseong);
	return (jongseong != 0) ? KRC_TRUE : KRC_FALSE;
}

//---------------------------------------------------------------------------
// 음절 + 자음 → 종성 추가 또는 겹받침  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_add_jongseong(krc_wchar_t base_code, krc_wchar_t additional_code, krc_wchar_t* compound_code)
{
	int choseong, jungseong, jongseong;
	krc_hangulw_code_to_index(base_code, &choseong, &jungseong, &jongseong);

	int jongseong_additional = _krc_inputw_hangul_jongseong_index_table[additional_code - KRC_HANGULW_JAEUM_BASE];
	int jongseong_compound;

	if (jongseong == 0)
	{
		/* 종성 없음: 가 + ㄱ = 각 */
		if (jongseong_additional != -1)
		{
			*compound_code = krc_hangulw_index_to_code(choseong, jungseong, jongseong_additional);
			return KRC_TRUE;
		}
	}
	else
	{
		/* 겹받침 시도 */
		jongseong_compound = krc_hangulw_compose_jongseong_index(jongseong, jongseong_additional);
		if (jongseong_compound != -1)
		{
			*compound_code = krc_hangulw_index_to_code(choseong, jungseong, jongseong_compound);
			return KRC_TRUE;
		}
	}
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// 음절(종성 없음) + 모음 → 복합 중성  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_add_jungseong(krc_wchar_t base_code, krc_wchar_t additional_code, krc_wchar_t* compound_code)
{
	int choseong, jungseong, jongseong;
	krc_hangulw_code_to_index(base_code, &choseong, &jungseong, &jongseong);

	int jungseong_additional = (int)(additional_code - KRC_HANGULW_MOEUM_BASE);
	int jungseong_compound = krc_hangulw_compose_jungseong_index(jungseong, jungseong_additional);

	if (jungseong_compound != -1)
	{
		*compound_code = krc_hangulw_index_to_code(choseong, jungseong_compound, 0);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// 단독 자음(초성) + 모음 → 새 음절  (반환: KRC_TRUE = 성공)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_compose_jamo(krc_wchar_t jaeum_code, krc_wchar_t moeum_code, krc_wchar_t* compound_code)
{
	int choseong = _krc_inputw_hangul_choseong_index_table[jaeum_code - KRC_HANGULW_JAEUM_BASE];
	int jungseong = (int)(moeum_code - KRC_HANGULW_MOEUM_BASE);

	if (choseong != -1)
	{
		*compound_code = krc_hangulw_index_to_code(choseong, jungseong, 0);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// krc_hangulw_compose()
//
//   조합 중인 코드(base_code)에 낱글자(additional_code)를 추가하여
//   조합된 코드(*compound_code)를 만든다.
//
//   반환값: KRC_TRUE  — 조합 성공 (조합이 계속 진행중)
//           KRC_FALSE — 조합 불가 (새 글자 시작)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_compose(krc_wchar_t base_code, krc_wchar_t additional_code, krc_wchar_t* compound_code)
{
	if (krc_hangulw_is_jamo(additional_code) == KRC_TRUE)
	{
		//===================================================================
		// base_code 가 초성+중성 이상 조합된 상태
		//===================================================================
		if (krc_hangulw_is_char(base_code) == KRC_TRUE)
		{
			if (krc_hangulw_is_jaeum(additional_code))
			{
				if (krc_hangulw_add_jongseong(base_code, additional_code, compound_code))
				{
					return KRC_TRUE;
				}
			}
			else
			{
				if (krc_hangulw_has_jongseong(base_code) == KRC_FALSE)
				{
					if (krc_hangulw_add_jungseong(base_code, additional_code, compound_code))
					{
						return KRC_TRUE;
					}
				}
			}
		}

		//===================================================================
		// base_code 가 자음 낱글자 — 모음과 결합하여 글자 완성
		//===================================================================
		else if (krc_hangulw_is_jaeum(base_code))
		{
			if (krc_hangulw_is_moeum(additional_code))
			{
				if (krc_hangulw_compose_jamo(base_code, additional_code, compound_code))
					return KRC_TRUE;
			}
		}

		//===================================================================
		// 조합 없음 — 첫 번째 낱글자
		//===================================================================
		else
		{
			*compound_code = additional_code;
			return KRC_TRUE;
		}
	}

	*compound_code = additional_code;
	return KRC_FALSE;
}

//---------------------------------------------------------------------------
// krc_hangulw_decompose_last()
//
//   조합된 한글 코드(base_code)에서 마지막 입력 요소를 분리한다.
//   base_code = *remaining_code + *last_code
//
//   반환값: KRC_TRUE  — 분리 성공 (*remaining_code 에 조합 잔여분)
//           KRC_FALSE — 분리 불가 (*remaining_code = 0, *last_code = base_code)
//---------------------------------------------------------------------------
static krc_bool_t krc_hangulw_decompose_last(krc_wchar_t base_code, krc_wchar_t* remaining_code, krc_wchar_t* last_code)
{
	int choseong, jungseong, jongseong;
	int jungseong_base, jungseong_additional;
	int jongseong_base, jongseong_additional;

	if (krc_hangulw_is_char(base_code) == KRC_TRUE)
	{
		krc_hangulw_code_to_index(base_code, &choseong, &jungseong, &jongseong);

		//===================================================================
		// 종성 없음
		//===================================================================
		if (jongseong == 0)
		{
			if (krc_hangulw_decompose_jungseong_index(jungseong, &jungseong_base, &jungseong_additional))
			{
				/* 복합 중성 분리: [초성+단순중성] + [단순모음] */
				*remaining_code = krc_hangulw_index_to_code(choseong, jungseong_base, 0);
				*last_code = KRC_HANGULW_MOEUM_BASE + (krc_wchar_t)jungseong_additional;
			}
			else
			{
				/* 단순 중성 분리: [초성] + [중성모음] */
				*remaining_code = _krc_inputw_hangul_choseong_code_table[choseong];
				*last_code = KRC_HANGULW_MOEUM_BASE + (krc_wchar_t)jungseong;
			}
		}

		//===================================================================
		// 종성 있음
		//===================================================================
		else
		{
			if (krc_hangulw_decompose_jongseong_index(jongseong, &jongseong_base, &jongseong_additional))
			{
				/* 겹받침 분리: [초성+중성+단자음종성] + [겹받침의 두번째 자음] */
				*remaining_code = krc_hangulw_index_to_code(choseong, jungseong, jongseong_base);
				*last_code = _krc_inputw_hangul_jongseong_code_table[jongseong_additional];
			}
			else
			{
				/* 단받침 분리: [초성+중성] + [종성자음] */
				*remaining_code = krc_hangulw_index_to_code(choseong, jungseong, 0);
				*last_code = _krc_inputw_hangul_jongseong_code_table[jongseong];
			}
		}

		return KRC_TRUE;
	}

	*remaining_code = KRC_WCHAR_NULL;
	*last_code = base_code;
	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// 커서
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 커서 위치(cursor_line, cursor_column, current_line_offset) 갱신
//
//   current_line_offset 캐시를 활용하여 스캔 범위를 최소화한다.
//   - cursor_offset >= current_line_offset : 현재 줄 시작부터 순방향 스캔
//   - cursor_offset <  current_line_offset : 처음부터 전체 스캔 (역방향 이동)
//---------------------------------------------------------------------------
static void krc_inputw_cursor_update(krc_inputw_t* ctx)
{
	const krc_wchar_t* text = ctx->buffer_pointer;
	krc_size_t offset;
	krc_size_t line;
	krc_size_t line_start;
	krc_size_t column = 0u;

	if (ctx->current_line_offset <= ctx->cursor_offset && (ctx->current_line_offset == 0u || text[ctx->current_line_offset - 1u] == '\n'))
	{
		/* 순방향: 현재 줄 시작부터 스캔 */
		offset = ctx->current_line_offset;
		line = ctx->cursor_line;
		line_start = ctx->current_line_offset;
	}
	else
	{
		/* 역방향: 버퍼 처음부터 전체 스캔 (역방향 커서 이동 또는 개행 삭제 후 캐시 무효) */
		offset = 0u;
		line = 0u;
		line_start = 0u;
	}

	while (offset < ctx->cursor_offset && text[offset] != KRC_WCHAR_NULL)
	{
		if (text[offset] == '\n')
		{
			line++;
			column = 0u;
			line_start = offset + 1u;
		}
		else if (text[offset] != '\r')
		{
			column++;
		}
		offset++;
	}

	ctx->cursor_line = line;
	ctx->cursor_column = column;
	ctx->current_line_offset = line_start;
}

static krc_bool_t krc_inputw_cursor_advance(krc_inputw_t* ctx)
{
	if (ctx->cursor_offset < ctx->length)
	{
		ctx->cursor_offset++;

		krc_inputw_cursor_update(ctx);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

static krc_bool_t krc_inputw_cursor_back(krc_inputw_t* ctx)
{
	if (ctx->cursor_offset > 0u)
	{
		ctx->cursor_offset--;

		krc_inputw_cursor_update(ctx);
		return KRC_TRUE;
	}
	return KRC_FALSE;
}

static void krc_inputw_cursor_set(krc_inputw_t* ctx, krc_size_t offset)
{
	if (offset <= ctx->length)
	{
		ctx->cursor_offset = offset;

		krc_inputw_cursor_update(ctx);
	}
}

static krc_size_t krc_inputw_cursor_get(krc_inputw_t* ctx)
{
	return ctx->cursor_offset;
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
static krc_bool_t krc_inputw_is_composing(const krc_inputw_t* ctx)
{
	return ctx->hangul_composing;
}

//---------------------------------------------------------------------------
// 조합 시작
//---------------------------------------------------------------------------
static void krc_inputw_start_composing(krc_inputw_t* ctx)
{
	ctx->hangul_composing = KRC_TRUE;
}

//---------------------------------------------------------------------------
// 조합 중단
//---------------------------------------------------------------------------
static void krc_inputw_stop_composing(krc_inputw_t* ctx)
{
	ctx->hangul_composing = KRC_FALSE;
}

//---------------------------------------------------------------------------
// 한글 조합 종료 — 커서 전진 후 composing 해제
//---------------------------------------------------------------------------
static void krc_inputw_commit_composing(krc_inputw_t* ctx)
{
	if (krc_inputw_is_composing(ctx) == KRC_TRUE)
	{
		krc_inputw_stop_composing(ctx);
		krc_inputw_cursor_advance(ctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
// 
// 텍스트
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
static krc_bool_t krc_inputw_text_is_null(krc_inputw_t* ctx)
{
	return ctx->buffer_pointer == (krc_wchar_t*)0 || ctx->buffer_size == 0u;
}

static krc_bool_t krc_inputw_text_empty(krc_inputw_t* ctx)
{
	return ctx->length == 0u;
}

static krc_size_t krc_inputw_text_get_length(krc_inputw_t* ctx)
{
	return ctx->length;
}

static krc_wchar_t krc_inputw_text_peek_char(krc_inputw_t* ctx, krc_size_t offset)
{
	if (offset < ctx->length)
	{
		return ctx->buffer_pointer[offset];
	}
	return KRC_WCHAR_NULL;
}

static krc_wchar_t krc_inputw_text_get_char(krc_inputw_t* ctx)
{
	if (ctx->cursor_offset < ctx->length)
	{
		return ctx->buffer_pointer[ctx->cursor_offset];
	}
	return KRC_WCHAR_NULL;
}

static void krc_inputw_text_set_char(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	if (ctx->cursor_offset < ctx->buffer_size - 1u)
	{
		ctx->buffer_pointer[ctx->cursor_offset] = char_code;
	}
}

static krc_bool_t krc_inputw_text_put_char(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	krc_size_t i;

	if (ctx->insert_mode == KRC_TRUE)
	{
		/* 삽입 모드 */
		if (ctx->length == max_length)
		{
			krc_inputw_stop_composing(ctx);
			return KRC_FALSE;
		}

		for (i = ctx->length + 1u; i > ctx->cursor_offset; i--) /* NULL terminator 포함 */
		{
			text[i] = text[i - 1u];
		}
		text[ctx->cursor_offset] = char_code;

		ctx->length++;

		if (KRC_FALSE==krc_inputw_is_composing(ctx))
		{
			krc_inputw_cursor_advance(ctx);
		}
		/*
		else
		{
			// 조합 중인 경우 커서 위치는 고정
		}
		*/
	}
	else
	{
		/* 덮어쓰기 모드 */
		if (ctx->length == ctx->cursor_offset)
		{
			krc_inputw_stop_composing(ctx);
			return KRC_FALSE;
		}

		text[ctx->cursor_offset] = char_code; /* 기존 글자 덮어쓰기 */

		if (KRC_FALSE == krc_inputw_is_composing(ctx))
		{
			krc_inputw_cursor_advance(ctx);
		}
		/*
		else
		{
			// 조합 중인 경우 커서 위치는 고정
		}
		*/
	}
	return KRC_TRUE;
}

static void krc_inputw_text_remove_char(krc_inputw_t* ctx)
{
	krc_size_t i;

	if (ctx->length == 0u || ctx->cursor_offset == ctx->length)
	{
		return;
	}

	for (i = ctx->cursor_offset; i <= ctx->length; i++) /* NULL terminator 포함 */
	{
		ctx->buffer_pointer[i] = ctx->buffer_pointer[i + 1u];
	}

	ctx->length--;
}

static void krc_inputw_text_backspace_char(krc_inputw_t* ctx)
{
	krc_size_t i;

	if (ctx->length == 0u || ctx->cursor_offset == 0u)
	{
		return;
	}

	for (i = ctx->cursor_offset - 1u; i <= ctx->length; i++) /* NULL terminator 포함 */
	{
		ctx->buffer_pointer[i] = ctx->buffer_pointer[i + 1u];
	}

	krc_inputw_cursor_back(ctx);

	ctx->length--;
}

static void krc_inputw_text_clear(krc_inputw_t* ctx)
{
	ctx->length              = 0u;
	ctx->current_line_offset = 0u;
	ctx->cursor_offset       = 0u;
	ctx->cursor_line         = 0u;
	ctx->cursor_column       = 0u;
	ctx->hangul_composing    = KRC_FALSE;
	if (ctx->buffer_pointer != (krc_wchar_t*)0 && ctx->buffer_size > 0u)
	{
		ctx->buffer_pointer[0] = KRC_WCHAR_NULL;
	}
}

/* 현재 cursor 위치에 char_code 를 항상 삽입한다 (insert_mode 무관)
   → krc_inputw_key_enter 처럼 \r 삽입 직후 length 미갱신 상태에서 \n 삽입 시에도 안전하다. */
static krc_bool_t krc_inputw_text_insert_char(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;

	krc_size_t i;

	if (ctx->length == max_length)
	{
		return KRC_FALSE;
	}

	if (ctx->length == ctx->cursor_offset)
	{
		return KRC_FALSE;
	}

	for (i = ctx->length + 1u; i > ctx->cursor_offset; i--) /* NULL terminator 포함 */
	{
		text[i] = text[i - 1u];
	}

	text[ctx->cursor_offset] = char_code;

	ctx->length++;

	krc_inputw_cursor_advance(ctx);

	return KRC_TRUE;
}

static void krc_inputw_text_new_line(krc_inputw_t* ctx)
{
	krc_inputw_text_insert_char(ctx, '\r');
	krc_inputw_text_insert_char(ctx, '\n');
}

static void krc_inputw_text_tab(krc_inputw_t* ctx)
{
	krc_inputw_text_insert_char(ctx, '\t');
}





/////////////////////////////////////////////////////////////////////////////
// 
// 커서
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 커서 위치를 처음으로 이동
//---------------------------------------------------------------------------
static void krc_inputw_cursor_begin(krc_inputw_t* ctx)
{
	krc_inputw_cursor_set(ctx, 0u);
}

//---------------------------------------------------------------------------
// 커서 위치를 끝으로 이동
//---------------------------------------------------------------------------
static void krc_inputw_cursor_end(krc_inputw_t* ctx)
{
	krc_inputw_cursor_set(ctx, krc_inputw_text_get_length(ctx));
}

static void krc_inputw_cursor_line_begin(krc_inputw_t* ctx)
{
	krc_size_t offset = ctx->cursor_offset;
	krc_wchar_t ch;
	while (offset > 0u)
	{
		ch = krc_inputw_text_peek_char(ctx, offset - 1u);
		if (ch == '\r' || ch == '\n')
		{
			break;
		}
		offset--;
	}
	krc_inputw_cursor_set(ctx, offset);
}

static void krc_inputw_cursor_line_end(krc_inputw_t* ctx)
{
	krc_size_t offset = ctx->cursor_offset;
	krc_wchar_t ch;
	while (offset < ctx->length)
	{
		ch = krc_inputw_text_peek_char(ctx, offset);
		if (ch == '\r' || ch == '\n')
		{
			break;
		}
		offset++;
	}
	krc_inputw_cursor_set(ctx, offset);
}

static void krc_inputw_cursor_left(krc_inputw_t* ctx)
{
	krc_inputw_cursor_back(ctx);
}

static void krc_inputw_cursor_right(krc_inputw_t* ctx)
{
	krc_inputw_cursor_advance(ctx);
}

//---------------------------------------------------------------------------
// 다중라인 커서 이동 — 윗 줄 목표 행/열 위치로 이동
//---------------------------------------------------------------------------
static void krc_inputw_cursor_up(krc_inputw_t* ctx)
{
	krc_size_t  target_line = ctx->cursor_line - 1u;
	krc_size_t  target_column = ctx->cursor_column;
	krc_size_t  line = 0u;
	krc_size_t  column = 0u;
	krc_size_t  offset = 0u;
	krc_wchar_t ch;


	while ((ch = krc_inputw_text_peek_char(ctx, offset)) != KRC_WCHAR_NULL)
	{
		if (line == target_line && column == target_column)
		{
			krc_inputw_cursor_set(ctx, offset);
			break;
		}
		if (ch == '\n')
		{
			line++;
			column = 0u;
		}
		else if (ch != '\r')
		{
			if (line == target_line)
			{
				column++;
			}
		}
		offset++;
	}
}

//---------------------------------------------------------------------------
// 다중라인 커서 이동 — 아랫 줄 목표 행/열 위치로 이동
//---------------------------------------------------------------------------
static void krc_inputw_cursor_down(krc_inputw_t* ctx)
{
	krc_size_t  target_line = ctx->cursor_line + 1u;
	krc_size_t  target_column = ctx->cursor_column;
	krc_size_t  line = 0u;
	krc_size_t  column = 0u;
	krc_size_t  offset = 0u;
	krc_wchar_t ch;
	krc_bool_t  found = KRC_FALSE;

	while ((ch = krc_inputw_text_peek_char(ctx, offset)) != KRC_WCHAR_NULL)
	{
		if (line == target_line && column == target_column)
		{
			krc_inputw_cursor_set(ctx, offset);
			found = KRC_TRUE;
			break;
		}
		if (ch == '\n')
		{
			if (line == target_line) /* 줄 끝 도달 */
			{
				krc_inputw_cursor_set(ctx, offset);
				found = KRC_TRUE;
				break;
			}
			line++;
			column = 0u;
		}
		else if (ch != '\r')
		{
			if (line == target_line)
			{
				column++;
			}
		}
		offset++;
	}
	if (found == KRC_FALSE && line == target_line)
	{
		krc_inputw_cursor_set(ctx, offset); /* 마지막 줄 끝 */
	}
}





/////////////////////////////////////////////////////////////////////////////
// 
// put_char 서브 핸들러
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 조합 중 + 모음 입력 실패
//---------------------------------------------------------------------------
static void krc_inputw_put_char_hangul_fail_moeum(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t  base_code = krc_inputw_text_get_char(ctx);
	krc_wchar_t  remaining_code;
	krc_wchar_t  last_code;
	krc_wchar_t  compound_code;

	krc_hangulw_decompose_last(base_code, &remaining_code, &last_code);

	if (krc_hangulw_is_jaeum(last_code))
	{
		/* 갓 + ㅏ = 가 + 사 */
		krc_inputw_text_set_char(ctx, remaining_code);
		krc_inputw_commit_composing(ctx);

		if (krc_hangulw_compose(last_code, char_code, &compound_code) == KRC_TRUE)
		{
			krc_inputw_start_composing(ctx);
			krc_inputw_text_put_char(ctx, compound_code);
		}
	}
	else
	{
		/* 가 + ㅏ = 가 + ㅏ  (조합 불가) */
		krc_inputw_commit_composing(ctx);

		krc_inputw_text_put_char(ctx, char_code);
	}
}

//---------------------------------------------------------------------------
// 조합 중 + 자음 입력 실패
//---------------------------------------------------------------------------
static void krc_inputw_put_char_hangul_fail_jaeum(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	/* 각 + ㄱ = 각 + ㄱ */
	krc_inputw_commit_composing(ctx);

	krc_inputw_start_composing(ctx);
	krc_inputw_text_put_char(ctx, char_code);
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 한글 조합 진행 중
//---------------------------------------------------------------------------
static void krc_inputw_put_char_hangul_composing(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t  base_code = krc_inputw_text_get_char(ctx);
	krc_wchar_t  compound_code;

	if (krc_hangulw_compose(base_code, char_code, &compound_code) == KRC_TRUE)
	{
		/* 조합 성공: 가 + ㄱ = 각 */
		krc_inputw_text_set_char(ctx, compound_code);
	}
	else
	{
		/* 조합 실패 */
		if (krc_hangulw_is_moeum(char_code))
		{
			krc_inputw_put_char_hangul_fail_moeum(ctx, char_code);
		}
		else
		{
			krc_inputw_put_char_hangul_fail_jaeum(ctx, char_code);
		}
	}
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 한글 조합 시작
//---------------------------------------------------------------------------
static void krc_inputw_put_char_hangul_new(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	if (krc_hangulw_is_jaeum(char_code))
	{
		krc_inputw_start_composing(ctx);
	}

	krc_inputw_text_put_char(ctx, char_code);
}

//---------------------------------------------------------------------------
// put_char 서브 핸들러 — 비한글 문자(영문/숫자/특수문자 등)
//---------------------------------------------------------------------------
static void krc_inputw_put_char_new(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	krc_inputw_commit_composing(ctx);

	krc_inputw_text_put_char(ctx, char_code);
}





/////////////////////////////////////////////////////////////////////////////
// 
// 키 핸들러
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// 키 핸들러 — Enter
//---------------------------------------------------------------------------
static void krc_inputw_key_enter(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		krc_inputw_text_new_line(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Tab
//---------------------------------------------------------------------------
static void krc_inputw_key_tab(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	krc_inputw_text_tab(ctx);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Backspace
//---------------------------------------------------------------------------
static void krc_inputw_key_backspace(krc_inputw_t* ctx)
{
	krc_wchar_t base_code;
	krc_wchar_t remaining_code;
	krc_wchar_t last_code;

	if (krc_inputw_cursor_get(ctx) == 0u && krc_inputw_is_composing(ctx) == KRC_FALSE)
	{
		return;
	}

	if (krc_inputw_is_composing(ctx) == KRC_TRUE)
	{
		base_code = krc_inputw_text_get_char(ctx);
		krc_hangulw_decompose_last(base_code, &remaining_code, &last_code);
		if (remaining_code != KRC_WCHAR_NULL)
		{
			krc_inputw_text_set_char(ctx, remaining_code);
		}
		else
		{
			krc_inputw_text_remove_char(ctx);
			krc_inputw_stop_composing(ctx);
		}
	}
	else
	{
		krc_inputw_text_backspace_char(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Delete
//---------------------------------------------------------------------------
static void krc_inputw_key_delete(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	krc_inputw_text_remove_char(ctx);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Left
//---------------------------------------------------------------------------
static void krc_inputw_key_left(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	krc_inputw_cursor_left(ctx);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Right
//---------------------------------------------------------------------------
static void krc_inputw_key_right(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	krc_inputw_cursor_right(ctx);
}

//---------------------------------------------------------------------------
// 키 핸들러 — Up
//---------------------------------------------------------------------------
static void krc_inputw_key_up(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		krc_inputw_cursor_up(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Down
//---------------------------------------------------------------------------
static void krc_inputw_key_down(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		krc_inputw_cursor_down(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Home
//---------------------------------------------------------------------------
static void krc_inputw_key_home(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		krc_inputw_cursor_line_begin(ctx);
	}
	else
	{
		krc_inputw_cursor_begin(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — End
//---------------------------------------------------------------------------
static void krc_inputw_key_end(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->multiline == KRC_TRUE)
	{
		krc_inputw_cursor_line_end(ctx);
	}
	else
	{
		krc_inputw_cursor_end(ctx);
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — Insert (삽입/덮어쓰기 모드 전환)
//---------------------------------------------------------------------------
static void krc_inputw_key_insert(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	ctx->insert_mode = (ctx->insert_mode == KRC_TRUE) ? KRC_FALSE : KRC_TRUE;
}

//---------------------------------------------------------------------------
// 키 핸들러 — Caps Lock (토글)
//---------------------------------------------------------------------------
static void krc_inputw_key_capslock(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	ctx->capslock_mode = (ctx->capslock_mode == KRC_TRUE) ? KRC_FALSE : KRC_TRUE;
}

//---------------------------------------------------------------------------
// 키 핸들러 — 한/영 전환 (HANGUL = RALT)
//---------------------------------------------------------------------------
static void krc_inputw_key_hangul(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);

	if (ctx->key_mode == KRC_INPUT_KEY_MODE_LATIN)
	{
		ctx->key_mode = KRC_INPUT_KEY_MODE_HANGUL;
	}
	else
	{
		ctx->key_mode = KRC_INPUT_KEY_MODE_LATIN;
	}
}

//---------------------------------------------------------------------------
// 키 핸들러 — ESC (조합 취소 — 확정하지 않고 중단)
//---------------------------------------------------------------------------
static void krc_inputw_key_esc(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);
}

//---------------------------------------------------------------------------
// 키 핸들러 — 기타 특수키 (PAGEUP, PAGEDOWN, F1~F12 등)
//---------------------------------------------------------------------------
static void krc_inputw_key_default(krc_inputw_t* ctx)
{
	krc_inputw_commit_composing(ctx);
}





/////////////////////////////////////////////////////////////////////////////
// 
// API
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_inputw_init(krc_inputw_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size, krc_bool_t multiline)
{
	ctx->buffer_pointer      = buffer;
	ctx->buffer_size         = buffer_size;
	ctx->multiline           = multiline;
	ctx->length              = 0u;
	ctx->current_line_offset = 0u;
	ctx->cursor_offset       = 0u;
	ctx->cursor_line         = 0u;
	ctx->cursor_column       = 0u;
	ctx->shift_mode          = KRC_FALSE;
	ctx->capslock_mode       = KRC_FALSE;
	ctx->insert_mode         = KRC_TRUE;
	ctx->key_mode            = KRC_INPUT_KEY_MODE_LATIN;
	ctx->hangul_composing    = KRC_FALSE;

	if (buffer != (krc_wchar_t*)0 && buffer_size > 0u)
	{
		buffer[0] = KRC_WCHAR_NULL;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//---------------------------------------------------------------------------
// krc_inputw_put_char()
//
//   유니코드 문자 하나를 현재 커서 위치에 삽입한다.
//   한글 낱글자인 경우 한글 조합을 수행한다.
//---------------------------------------------------------------------------
KRC_API void krc_inputw_put_char(krc_inputw_t* ctx, krc_wchar_t char_code)
{
	if (krc_inputw_text_is_null(ctx))
	{
		return;
	}

	//=======================================================================
	// 한글 낱글자
	//=======================================================================
	if (krc_hangulw_is_jamo(char_code) == KRC_TRUE)
	{
		if (krc_inputw_is_composing(ctx) == KRC_TRUE)
		{
			krc_inputw_put_char_hangul_composing(ctx, char_code);
		}
		else
		{
			krc_inputw_put_char_hangul_new(ctx, char_code);
		}
	}
	//=======================================================================
	// 한글 낱글자가 아닌 경우 (영문, 숫자, 특수문자 등)
	//=======================================================================
	else
	{
		krc_inputw_put_char_new(ctx, char_code);
	}
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
	krc_wchar_t char_code;

	if (krc_inputw_text_is_null(ctx))
	{
		return;
	}


	//=======================================================================
	// 문자 키 처리
	//=======================================================================
	krc_uint32_t key_code = KRC_INPUT_KEY_CODE(key);
	krc_uint32_t key_modifier = KRC_INPUT_KEY_MODIFIER(key);
	krc_bool_t shift = KRC_FALSE;

	if (key_modifier == KRC_INPUT_KEY_MODIFIER(KRC_INPUT_KEY_LSHIFT) || key_modifier == KRC_INPUT_KEY_MODIFIER(KRC_INPUT_KEY_RSHIFT))
	{
		shift = KRC_TRUE;
	}
	if (ctx->shift_mode == KRC_TRUE)
	{
		shift = KRC_TRUE;
	}
	if ((ctx->capslock_mode == KRC_TRUE) && (ctx->key_mode == KRC_INPUT_KEY_MODE_LATIN))
	{
		shift = KRC_TRUE;
	}

	if (key_code >= KRC_INPUT_KEY_1 && key_code <= KRC_INPUT_KEY_SPACE)
	{
		char_code = krc_inputw_key_char(key_code - KRC_INPUT_KEY_1, ctx->key_mode, shift);
		if (char_code != KRC_WCHAR_NULL)
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
	case KRC_INPUT_KEY_BACKSPACE: krc_inputw_key_backspace(ctx); break;
	case KRC_INPUT_KEY_ENTER:     krc_inputw_key_enter(ctx);     break;
	case KRC_INPUT_KEY_TAB:       krc_inputw_key_tab(ctx);       break;

	case KRC_INPUT_KEY_HANGUL:    krc_inputw_key_hangul(ctx);    break;
	case KRC_INPUT_KEY_CAPSLOCK:  krc_inputw_key_capslock(ctx);  break;

	case KRC_INPUT_KEY_INSERT:    krc_inputw_key_insert(ctx);    break;
	case KRC_INPUT_KEY_HOME:      krc_inputw_key_home(ctx);      break;
	case KRC_INPUT_KEY_DELETE:    krc_inputw_key_delete(ctx);    break;
	case KRC_INPUT_KEY_END:       krc_inputw_key_end(ctx);       break;

	case KRC_INPUT_KEY_LEFT:      krc_inputw_key_left(ctx);      break;
	case KRC_INPUT_KEY_RIGHT:     krc_inputw_key_right(ctx);     break;
	case KRC_INPUT_KEY_UP:        krc_inputw_key_up(ctx);        break;
	case KRC_INPUT_KEY_DOWN:      krc_inputw_key_down(ctx);      break;

	case KRC_INPUT_KEY_ESC:       krc_inputw_key_esc(ctx);       break;

	default:                      krc_inputw_key_default(ctx);   break;
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_inputw_set_shift_mode(krc_inputw_t* ctx, krc_bool_t mode)
{
	ctx->shift_mode = mode;
}

KRC_API void krc_inputw_set_capslock_mode(krc_inputw_t* ctx, krc_bool_t mode)
{
	krc_inputw_commit_composing(ctx);

	ctx->capslock_mode = mode;
}

KRC_API void krc_inputw_set_insert_mode(krc_inputw_t* ctx, krc_bool_t mode)
{
	krc_inputw_commit_composing(ctx);

	ctx->insert_mode = mode;
}

KRC_API void krc_inputw_set_key_mode(krc_inputw_t* ctx, krc_uint32_t mode)
{
	krc_inputw_commit_composing(ctx);

	if (mode == KRC_INPUT_KEY_MODE_LATIN || mode == KRC_INPUT_KEY_MODE_HANGUL)
	{
		ctx->key_mode = mode;
	}
}
