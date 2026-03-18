/////////////////////////////////////////////////////////////////////////////
//
// File: krc_unicode_input.c
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
#include "krc_input_unicode.h"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static const int _krc_hangul_choseong_index_table[30] = {
	 0, // 0x3131 ㄱ
	 1, // 0x3132 ㄲ
	-1, // 0x3133 ㄳ x
	 2, // 0x3134 ㄴ
	-1, // 0x3135 ㄵ x
	-1, // 0x3136 ㄶ x
	 3, // 0x3137 ㄷ
	 4, // 0x3138 ㄸ
	 5, // 0x3139 ㄹ
	-1, // 0x313a ㄺ x
	-1, // 0x313b ㄻ x
	-1, // 0x313c ㄼ x
	-1, // 0x313d ㄽ x
	-1, // 0x313e ㄾ x
	-1, // 0x313f ㄿ x
	-1, // 0x3140 ㅀ x
	 6, // 0x3141 ㅁ
	 7, // 0x3142 ㅂ
	 8, // 0x3143 ㅃ
	-1, // 0x3144 ㅄ x
	 9, // 0x3145 ㅅ
	10, // 0x3146 ㅆ
	11, // 0x3147 ㅇ
	12, // 0x3148 ㅈ
	13, // 0x3149 ㅉ
	14, // 0x314a ㅊ
	15, // 0x314b ㅋ
	16, // 0x314c ㅌ
	17, // 0x314d ㅍ
	18  // 0x314e ㅎ
};

static const int _krc_hangul_jongseong_index_table[30] = {
	 1, // 0x3131 ㄱ
	 2, // 0x3132 ㄲ
	 3, // 0x3133 ㄳ
	 4, // 0x3134 ㄴ
	 5, // 0x3135 ㄵ
	 6, // 0x3136 ㄶ
	 7, // 0x3137 ㄷ
	-1, // 0x3138 ㄸ x
	 8, // 0x3139 ㄹ
	 9, // 0x313a ㄺ
	10, // 0x313b ㄻ
	11, // 0x313c ㄼ
	12, // 0x313d ㄽ
	13, // 0x313e ㄾ
	14, // 0x313f ㄿ
	15, // 0x3140 ㅀ
	16, // 0x3141 ㅁ
	17, // 0x3142 ㅂ
	-1, // 0x3143 ㅃ x
	18, // 0x3144 ㅄ
	19, // 0x3145 ㅅ
	20, // 0x3146 ㅆ
	21, // 0x3147 ㅇ
	22, // 0x3148 ㅈ
	-1, // 0x3149 ㅉ x
	23, // 0x314a ㅊ
	24, // 0x314b ㅋ
	25, // 0x314c ㅌ
	26, // 0x314d ㅍ
	27  // 0x314e ㅎ
};

static const krc_wchar_t _krc_hangul_jongseong_code_table[28] = {
	0x0000, // blank
	0x3131, // ㄱ
	0x3132, // ㄲ
	0x3133, // ㄳ
	0x3134, // ㄴ
	0x3135, // ㄵ
	0x3136, // ㄶ
	0x3137, // ㄷ
	0x3139, // ㄹ
	0x313a, // ㄺ
	0x313b, // ㄻ
	0x313c, // ㄼ
	0x313d, // ㄽ
	0x313e, // ㄾ
	0x313f, // ㄿ
	0x3140, // ㅀ
	0x3141, // ㅁ
	0x3142, // ㅂ
	0x3144, // ㅄ
	0x3145, // ㅅ
	0x3146, // ㅆ
	0x3147, // ㅇ
	0x3148, // ㅈ
	0x314a, // ㅊ
	0x314b, // ㅋ
	0x314c, // ㅌ
	0x314d, // ㅍ
	0x314e  // ㅎ
};

static const krc_wchar_t _krc_hangul_choseong_code_table[19] = {
	0x3131, // ㄱ
	0x3132, // ㄲ
	0x3134, // ㄴ
	0x3137, // ㄷ
	0x3138, // ㄸ
	0x3139, // ㄹ
	0x3141, // ㅁ
	0x3142, // ㅂ
	0x3143, // ㅃ
	0x3145, // ㅅ
	0x3146, // ㅆ
	0x3147, // ㅇ
	0x3148, // ㅈ
	0x3149, // ㅉ
	0x314a, // ㅊ
	0x314b, // ㅋ
	0x314c, // ㅌ
	0x314d, // ㅍ
	0x314e  // ㅎ
};

/* 영문 키 → 유니코드 문자 변환표
   인덱스 0 = KRC_INPUT_KEY_GRAVE(14),  인덱스 50 = KRC_INPUT_KEY_SPACE(64)
   0x0000 항목은 특수 키(BACKSPACE, ENTER) — switch 에서 별도 처리 */
static const krc_wchar_t _krc_key_char_table[51] = {
	/* 14 GRAVE      */ '`',
	/* 15 1          */ '1',
	/* 16 2          */ '2',
	/* 17 3          */ '3',
	/* 18 4          */ '4',
	/* 19 5          */ '5',
	/* 20 6          */ '6',
	/* 21 7          */ '7',
	/* 22 8          */ '8',
	/* 23 9          */ '9',
	/* 24 0          */ '0',
	/* 25 MINUS      */ '-',
	/* 26 PLUS(=)    */ '=',
	/* 27 BACKSPACE  */ 0x0000, /* special */
	/* 28 TAB        */ '\t',
	/* 29 A          */ 'a',
	/* 30 B          */ 'b',
	/* 31 C          */ 'c',
	/* 32 D          */ 'd',
	/* 33 E          */ 'e',
	/* 34 F          */ 'f',
	/* 35 G          */ 'g',
	/* 36 H          */ 'h',
	/* 37 I          */ 'i',
	/* 38 J          */ 'j',
	/* 39 K          */ 'k',
	/* 40 L          */ 'l',
	/* 41 M          */ 'm',
	/* 42 N          */ 'n',
	/* 43 O          */ 'o',
	/* 44 P          */ 'p',
	/* 45 Q          */ 'q',
	/* 46 R          */ 'r',
	/* 47 S          */ 's',
	/* 48 T          */ 't',
	/* 49 U          */ 'u',
	/* 50 V          */ 'v',
	/* 51 W          */ 'w',
	/* 52 X          */ 'x',
	/* 53 Y          */ 'y',
	/* 54 Z          */ 'z',
	/* 55 LBRACKET   */ '[',
	/* 56 RBRACKET   */ ']',
	/* 57 BACKSLASH  */ '\\',
	/* 58 SEMICOLON  */ ';',
	/* 59 APOSTROPHE */ '\'',
	/* 60 COMMA      */ ',',
	/* 61 DOT        */ '.',
	/* 62 SLASH      */ '/',
	/* 63 ENTER      */ 0x0000, /* special */
	/* 64 SPACE      */ ' ',
};

/* 한국어(두벌식) A-Z 키 → 한글 자모 변환표
   인덱스 0 = KRC_INPUT_KEY_A(29) */
static const krc_wchar_t _krc_key_korean_dubeolsik_table[26] = {
	/* A */ 0x3141, /* ㅁ */
	/* B */ 0x3160, /* ㅠ */
	/* C */ 0x314a, /* ㅊ */
	/* D */ 0x3147, /* ㅇ */
	/* E */ 0x3137, /* ㄷ */
	/* F */ 0x3139, /* ㄹ */
	/* G */ 0x314e, /* ㅎ */
	/* H */ 0x3157, /* ㅗ */
	/* I */ 0x3151, /* ㅑ */
	/* J */ 0x3153, /* ㅓ */
	/* K */ 0x314f, /* ㅏ */
	/* L */ 0x3163, /* ㅣ */
	/* M */ 0x3161, /* ㅡ */
	/* N */ 0x315c, /* ㅜ */
	/* O */ 0x3150, /* ㅐ */
	/* P */ 0x3154, /* ㅔ */
	/* Q */ 0x3142, /* ㅂ */
	/* R */ 0x3131, /* ㄱ */
	/* S */ 0x3134, /* ㄴ */
	/* T */ 0x3145, /* ㅅ */
	/* U */ 0x3155, /* ㅕ */
	/* V */ 0x314d, /* ㅍ */
	/* W */ 0x3148, /* ㅈ */
	/* X */ 0x314c, /* ㅌ */
	/* Y */ 0x315b, /* ㅛ */
	/* Z */ 0x314b, /* ㅋ */
};






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//-----------------------------------------------------------------------------
// krc_string_length_w()  :  strlen() 대체 — NULL 미포함 길이 반환
//-----------------------------------------------------------------------------
static krc_size_t krc_string_length_w(const krc_wchar_t* str, krc_size_t max_length)
{
	krc_size_t i;

	for (i = 0u; i < max_length; i++)
	{
		if (str[i] == 0x0000)
		{
			return i;
		}
	}

	return max_length;
}

//-----------------------------------------------------------------------------
// krc_string_remove_char_w()  :  문자열에서 nIndex 위치의 글자 하나를 삭제
//-----------------------------------------------------------------------------
static krc_bool_t krc_string_remove_char_w(krc_wchar_t* str, krc_size_t text_length, krc_size_t max_length, krc_size_t index)
{
	krc_size_t i;

	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_string_length_w(str, max_length);
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
		str[i] = str[i + 1u];
	}

	return KRC_TRUE;
}

//-----------------------------------------------------------------------------
// krc_string_insert_char_w()  :  문자열의 nIndex 위치에 글자 하나를 삽입
//-----------------------------------------------------------------------------
static krc_bool_t krc_string_insert_char_w(krc_wchar_t* str, krc_size_t text_length, krc_size_t max_length, krc_size_t index, krc_wchar_t ch)
{
	krc_size_t i;

	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_string_length_w(str, max_length);
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
		str[i] = str[i - 1u];
	}

	str[index] = ch;

	return KRC_TRUE;
}

//-----------------------------------------------------------------------------
// krc_string_put_char_w()  :  insert_mode 에 따라 삽입 또는 덮어쓰기
//-----------------------------------------------------------------------------
static krc_bool_t krc_string_put_char_w(krc_wchar_t* str, krc_size_t text_length, krc_size_t max_length, krc_size_t index, krc_wchar_t ch, krc_bool_t insert_mode)
{
	if (insert_mode == KRC_TRUE)
	{
		return krc_string_insert_char_w(str, text_length, max_length, index, ch);
	}

	/* 덮어쓰기 모드 */
	if (text_length == (krc_size_t)(-1))
	{
		text_length = krc_string_length_w(str, max_length);
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
		str[index]        = ch;
		str[index + 1u]   = 0x0000;
	}
	else
	{
		str[index] = ch; /* 기존 글자 덮어쓰기 */
	}

	return KRC_TRUE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//-----------------------------------------------------------------------------
// krc_hangul_get_union_code()
//
//   조합 중인 코드(union_code)에 낱글자(element_code)를 추가하여
//   조합된 코드(*result_code)를 만든다.
//
//   반환값: KRC_TRUE  — 조합 성공 (조합이 계속 진행중)
//           KRC_FALSE — 조합 불가 (새 글자 시작)
//-----------------------------------------------------------------------------
static krc_bool_t krc_hangul_get_union_code(krc_wchar_t union_code, krc_wchar_t element_code, krc_wchar_t* result_code)
{
	int cho_index;
	int jung_index;
	int jong_index;
	int additional_index;
	int union_element_index;


	if (0x3131 <= element_code && element_code <= 0x3163)
	{
		//=====================================================================
		// union_code 가 초성+중성 이상 조합된 상태
		//=====================================================================
		if (0xac00 <= union_code && union_code <= 0xd7a3)
		{
			cho_index  = (union_code - 0xac00) / (21 * 28);
			jung_index = ((union_code - 0xac00) % (21 * 28)) / 28;
			jong_index = (union_code - 0xac00) % 28;

			//-----------------------------------------------------------------
			// 자음 입력
			//-----------------------------------------------------------------
			if (element_code < 0x314f)
			{
				additional_index    = _krc_hangul_jongseong_index_table[element_code - 0x3131];
				union_element_index = -1;

				if (jong_index == 0)
				{
					/* 종성 없음: 가 + ㄱ = 각 */
					if (additional_index != -1)
					{
						*result_code = union_code + (krc_wchar_t)additional_index;
						return KRC_TRUE;
					}
				}
				else
				{
					/* 겹받침 시도 */
					switch (jong_index)
					{
					case 1:  /* ㄱ */
						if (additional_index == 19) union_element_index =  3; /* ㄱ+ㅅ=ㄳ */
						break;
					case 4:  /* ㄴ */
						if (additional_index == 22) union_element_index =  5; /* ㄴ+ㅈ=ㄵ */
						if (additional_index == 27) union_element_index =  6; /* ㄴ+ㅎ=ㄶ */
						break;
					case 8:  /* ㄹ */
						if (additional_index ==  1) union_element_index =  9; /* ㄹ+ㄱ=ㄺ */
						if (additional_index == 16) union_element_index = 10; /* ㄹ+ㅁ=ㄻ */
						if (additional_index == 17) union_element_index = 11; /* ㄹ+ㅂ=ㄼ */
						if (additional_index == 19) union_element_index = 12; /* ㄹ+ㅅ=ㄽ */
						if (additional_index == 25) union_element_index = 13; /* ㄹ+ㅌ=ㄾ */
						if (additional_index == 26) union_element_index = 14; /* ㄹ+ㅍ=ㄿ */
						if (additional_index == 27) union_element_index = 15; /* ㄹ+ㅎ=ㅀ */
						break;
					case 17: /* ㅂ */
						if (additional_index == 19) union_element_index = 18; /* ㅂ+ㅅ=ㅄ */
						break;
					}

					if (union_element_index != -1)
					{
						*result_code = 0xac00 + (krc_wchar_t)(cho_index * 28 * 21 + jung_index * 28 + union_element_index);
						return KRC_TRUE;
					}
				}
			}
			//-----------------------------------------------------------------
			// 모음 입력
			//-----------------------------------------------------------------
			else
			{
				if (jong_index == 0)
				{
					/* 복합 중성 시도: 고 + ㅏ = 과 */
					additional_index    = element_code - 0x314f;
					union_element_index = -1;

					switch (jung_index)
					{
					case  8: /* ㅗ */
						if (additional_index ==  0) union_element_index =  9; /* ㅗ+ㅏ=ㅘ */
						if (additional_index ==  1) union_element_index = 10; /* ㅗ+ㅐ=ㅙ */
						if (additional_index == 20) union_element_index = 11; /* ㅗ+ㅣ=ㅚ */
						break;
					case 13: /* ㅜ */
						if (additional_index ==  4) union_element_index = 14; /* ㅜ+ㅓ=ㅝ */
						if (additional_index ==  5) union_element_index = 15; /* ㅜ+ㅔ=ㅞ */
						if (additional_index == 20) union_element_index = 16; /* ㅜ+ㅣ=ㅟ */
						break;
					case 18: /* ㅡ */
						if (additional_index == 20) union_element_index = 19; /* ㅡ+ㅣ=ㅢ */
						break;
					}

					if (union_element_index != -1)
					{
						*result_code = 0xac00 + (krc_wchar_t)(cho_index * 28 * 21 + union_element_index * 28);
						return KRC_TRUE;
					}
				}
			}
		}

		//=====================================================================
		// union_code 가 자음 낱글자 — 모음과 결합하여 글자 완성
		//=====================================================================
		else if (0x3131 <= union_code && union_code <= 0x314e)
		{
			/* ㄱ + ㅏ = 가 */
			if (element_code > 0x314e)
			{
				cho_index = _krc_hangul_choseong_index_table[union_code - 0x3131];
				if (cho_index != -1)
				{
					jung_index   = element_code - 0x314f;
					*result_code = 0xac00 + (krc_wchar_t)(cho_index * 28 * 21 + jung_index * 28);
					return KRC_TRUE;
				}
			}
		}

		//=====================================================================
		// 조합 없음 — 첫 번째 낱글자
		//=====================================================================
		else
		{
			*result_code = element_code;
			return KRC_TRUE;
		}
	}

	*result_code = element_code;
	return KRC_FALSE;
}

//-----------------------------------------------------------------------------
// krc_hangul_pop_back_code()
//
//   조합된 한글 코드(union_code)에서 마지막 입력 요소를 분리한다.
//   union_code = *result_code + *pop_back_code
//
//   반환값: KRC_TRUE  — 분리 성공 (*result_code 에 조합 잔여분)
//           KRC_FALSE — 분리 불가 (*result_code = 0, *pop_back_code = union_code)
//-----------------------------------------------------------------------------
static krc_bool_t krc_hangul_pop_back_code(krc_wchar_t union_code, krc_wchar_t* result_code, krc_wchar_t* pop_back_code)
{
	int cho_index;
	int jung_index;
	int jong_index;
	int additional_index;
	int union_element_index;


	if (0xac00 <= union_code && union_code <= 0xd7a3)
	{
		cho_index  = (union_code - 0xac00) / (21 * 28);
		jung_index = ((union_code - 0xac00) % (21 * 28)) / 28;
		jong_index = (union_code - 0xac00) % 28;

		additional_index    = -1;
		union_element_index = -1;

		//=====================================================================
		// 종성 없음
		//=====================================================================
		if (jong_index == 0)
		{
			switch (jung_index)
			{
			case  9: union_element_index =  8; additional_index =  0; break; /* ㅘ → ㅗ+ㅏ */
			case 10: union_element_index =  8; additional_index =  1; break; /* ㅙ → ㅗ+ㅐ */
			case 11: union_element_index =  8; additional_index = 20; break; /* ㅚ → ㅗ+ㅣ */
			case 14: union_element_index = 13; additional_index =  4; break; /* ㅝ → ㅜ+ㅓ */
			case 15: union_element_index = 13; additional_index =  5; break; /* ㅞ → ㅜ+ㅔ */
			case 16: union_element_index = 13; additional_index = 20; break; /* ㅟ → ㅜ+ㅣ */
			case 19: union_element_index = 18; additional_index = 20; break; /* ㅢ → ㅡ+ㅣ */
			}

			if (additional_index != -1)
			{
				/* 복합 중성 분리: [초성+단순중성]  +  [단순모음] */
				*result_code  = 0xac00 + (krc_wchar_t)(cho_index * 21 * 28 + union_element_index * 28);
				*pop_back_code = 0x314f + (krc_wchar_t)additional_index;
			}
			else
			{
				/* 단순 중성 분리: [초성]  +  [중성모음] */
				*result_code  = _krc_hangul_choseong_code_table[cho_index];
				*pop_back_code = 0x314f + (krc_wchar_t)jung_index;
			}
		}

		//=====================================================================
		// 종성 있음
		//=====================================================================
		else
		{
			switch (jong_index)
			{
			case  3: union_element_index =  1; additional_index = 19; break; /* ㄳ → ㄱ+ㅅ */
			case  5: union_element_index =  4; additional_index = 22; break; /* ㄵ → ㄴ+ㅈ */
			case  6: union_element_index =  4; additional_index = 27; break; /* ㄶ → ㄴ+ㅎ */
			case  9: union_element_index =  8; additional_index =  1; break; /* ㄺ → ㄹ+ㄱ */
			case 10: union_element_index =  8; additional_index = 16; break; /* ㄻ → ㄹ+ㅁ */
			case 11: union_element_index =  8; additional_index = 17; break; /* ㄼ → ㄹ+ㅂ */
			case 12: union_element_index =  8; additional_index = 19; break; /* ㄽ → ㄹ+ㅅ */
			case 13: union_element_index =  8; additional_index = 25; break; /* ㄾ → ㄹ+ㅌ */
			case 14: union_element_index =  8; additional_index = 26; break; /* ㄿ → ㄹ+ㅍ */
			case 15: union_element_index =  8; additional_index = 27; break; /* ㅀ → ㄹ+ㅎ */
			case 18: union_element_index = 17; additional_index = 19; break; /* ㅄ → ㅂ+ㅅ */
			}

			if (additional_index != -1)
			{
				/* 겹받침 분리: [초성+중성+단자음종성]  +  [겹받침의 두번째 자음] */
				*result_code  = 0xac00 + (krc_wchar_t)(cho_index * 21 * 28 + jung_index * 28 + union_element_index);
				*pop_back_code = _krc_hangul_jongseong_code_table[additional_index];
			}
			else
			{
				/* 단받침 분리: [초성+중성]  +  [종성자음] */
				*result_code  = 0xac00 + (krc_wchar_t)(cho_index * 21 * 28 + jung_index * 28);
				*pop_back_code = _krc_hangul_jongseong_code_table[jong_index];
			}
		}

		return KRC_TRUE;
	}

	*result_code  = 0x0000;
	*pop_back_code = union_code;
	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API void krc_input_unicode_init(krc_input_unicode_t* ctx, krc_wchar_t* buffer, krc_size_t buffer_size, krc_bool_t multiline)
{
	ctx->buffer_pointer = buffer;
	ctx->buffer_size    = buffer_size;
	ctx->multiline      = multiline;
	ctx->length         = 0u;
	ctx->insert_mode    = KRC_TRUE;
	ctx->cursor         = 0u;
	ctx->cursor_line    = 0u;
	ctx->cursor_column  = 0u;
	ctx->keyboard_set   = KRC_INPUT_KEYBOARD_SET_ENGLISH;
	ctx->hangul_union   = KRC_FALSE;

	if (buffer != (krc_wchar_t*)0 && buffer_size > 0u)
	{
		buffer[0] = 0x0000;
	}
}

//-----------------------------------------------------------------------------
// krc_input_unicode_put_char()
//
//   유니코드 문자 하나를 현재 커서 위치에 삽입한다.
//   한글 낱글자(0x3131~0x3163)인 경우 한글 조합을 수행한다.
//-----------------------------------------------------------------------------
KRC_API void krc_input_unicode_put_char(krc_input_unicode_t* ctx, krc_wchar_t char_code)
{
	krc_wchar_t  union_code;
	krc_wchar_t  result_code;
	krc_wchar_t  pop_back_code;
	krc_wchar_t* text         = ctx->buffer_pointer;
	krc_size_t   max_length   = ctx->buffer_size - 1u; /* NULL 자리 확보 */
	krc_bool_t   hangul_union = ctx->hangul_union;
	krc_bool_t   insert_mode  = ctx->insert_mode;

	if (text == (krc_wchar_t*)0 || ctx->buffer_size == 0u)
	{
		return;
	}

	union_code = text[ctx->cursor];

	//=========================================================================
	// 한글 낱글자 (0x3131 ~ 0x3163)
	//=========================================================================
	if (0x3131 <= char_code && char_code <= 0x3163)
	{
		//-----------------------------------------------------------------
		// 한글 조합 중
		//-----------------------------------------------------------------
		if (hangul_union == KRC_TRUE)
		{
			hangul_union = krc_hangul_get_union_code(union_code, char_code, &result_code);

			if (hangul_union == KRC_FALSE)
			{
				/* 조합 실패 */
				if (0x314f <= char_code && char_code <= 0x3163) /* 모음 입력 */
				{
					krc_hangul_pop_back_code(union_code, &result_code, &pop_back_code);
					if (0x3131 <= pop_back_code && pop_back_code < 0x314f) /* 이전 종성이 자음 */
					{
						/* 갓 + ㅏ = 가 + 사 */
						text[ctx->cursor] = result_code;
						ctx->cursor++;
						krc_hangul_get_union_code(pop_back_code, char_code, &union_code);
						hangul_union = KRC_FALSE;
						if (krc_string_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, union_code, insert_mode))
						{
							hangul_union = KRC_TRUE;
						}
					}
					else
					{
						/* 가 + ㅏ = 가 + ㅏ  (조합 불가) */
						ctx->cursor++;
						hangul_union = KRC_FALSE;
						if (krc_string_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, result_code, insert_mode))
						{
							if (0x3131 <= result_code && result_code < 0x314f)
							{
								hangul_union = KRC_TRUE;
							}
						}
					}
				}
				else /* 자음 입력으로 조합 실패 */
				{
					/* 각 + ㄱ = 각 + ㄱ */
					text[ctx->cursor] = union_code;
					ctx->cursor++;
					hangul_union = KRC_FALSE;
					if (krc_string_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, result_code, insert_mode))
					{
						if (0x3131 <= result_code && result_code < 0x314f)
						{
							hangul_union = KRC_TRUE;
						}
					}
				}
			}
			else
			{
				/* 조합 성공: 가 + ㄱ = 각 */
				text[ctx->cursor] = result_code;
				hangul_union = KRC_TRUE;
			}
		}
		//-----------------------------------------------------------------
		// 한글 조합 시작
		//-----------------------------------------------------------------
		else
		{
			if (krc_string_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, char_code, insert_mode))
			{
				if (0x3131 <= char_code && char_code < 0x314f) /* 자음이면 조합 시작 */
				{
					hangul_union = KRC_TRUE;
				}
				else /* 모음은 단독 처리 */
				{
					hangul_union = KRC_FALSE;
					ctx->cursor++;
				}
			}
		}
	}
	//=========================================================================
	// 한글 낱글자가 아닌 경우 (영문, 숫자, 특수문자 등)
	//=========================================================================
	else
	{
		if (hangul_union == KRC_TRUE) /* 조합 중이었으면 커서 전진 후 종료 */
		{
			ctx->cursor++;
		}

		if (krc_string_put_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, char_code, insert_mode))
		{
			ctx->cursor++;
		}

		hangul_union = KRC_FALSE;
	}

	ctx->hangul_union = hangul_union;
	ctx->length       = krc_string_length_w(text, ctx->buffer_size);

	/* cursor_line / cursor_column 갱신 */
	{
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
}

//-----------------------------------------------------------------------------
// krc_input_unicode_put_key()
//
//   KRC_INPUT_KEY_* 키 입력을 처리한다.
//   문자 키(GRAVE~SPACE 범위)는 put_char()에 위임하고,
//   특수 키(방향키, BACKSPACE, ENTER 등)는 내부에서 처리한다.
//-----------------------------------------------------------------------------
KRC_API void krc_input_unicode_put_key(krc_input_unicode_t* ctx, krc_uint32_t key)
{
	krc_wchar_t* text       = ctx->buffer_pointer;
	krc_size_t   max_length = ctx->buffer_size - 1u;
	krc_wchar_t  union_code;
	krc_wchar_t  result_code;
	krc_wchar_t  pop_back_code;
	krc_size_t   text_len;
	krc_wchar_t  char_code;

	if (text == (krc_wchar_t*)0 || ctx->buffer_size == 0u)
	{
		return;
	}

	//=========================================================================
	// 문자 키 처리 — put_char() 에 위임
	// KRC_INPUT_KEY_GRAVE(14) ~ KRC_INPUT_KEY_SPACE(64) 범위
	// BACKSPACE(27), ENTER(63) 는 표에서 0x0000 이므로 아래 switch 로 낙하
	//=========================================================================
	if (key >= KRC_INPUT_KEY_GRAVE && key <= KRC_INPUT_KEY_SPACE)
	{
		char_code = _krc_key_char_table[key - KRC_INPUT_KEY_GRAVE];

		/* 한국어 모드: A-Z 키는 두벌식 자모로 변환 */
		if (ctx->keyboard_set == KRC_INPUT_KEYBOARD_SET_KOREAN
			&& key >= KRC_INPUT_KEY_A && key <= KRC_INPUT_KEY_Z)
		{
			char_code = _krc_key_korean_dubeolsik_table[key - KRC_INPUT_KEY_A];
		}

		if (char_code != 0x0000)
		{
			krc_input_unicode_put_char(ctx, char_code);
			return;
		}
	}

	//=========================================================================
	// 특수 키 처리
	//=========================================================================
	switch (key)
	{
	//=========================================================================
	// Backspace
	//=========================================================================
	case KRC_INPUT_KEY_BACKSPACE:
		union_code = text[ctx->cursor];

		if (ctx->cursor == 0u && ctx->hangul_union == KRC_FALSE)
		{
			break;
		}

		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = krc_hangul_pop_back_code(union_code, &result_code, &pop_back_code);
			if (result_code != 0x0000)
			{
				text[ctx->cursor] = result_code;
				ctx->hangul_union = KRC_TRUE;
			}
			else
			{
				krc_string_remove_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor);
				ctx->hangul_union = KRC_FALSE;
			}
		}
		else
		{
			if (ctx->cursor > 0u)
			{
				ctx->cursor--;
				krc_string_remove_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor);
			}
			ctx->hangul_union = KRC_FALSE;
		}
		ctx->length = krc_string_length_w(text, ctx->buffer_size);
		break;

	//=========================================================================
	// Enter
	//=========================================================================
	case KRC_INPUT_KEY_ENTER:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
			ctx->cursor++;
		}
		if (ctx->multiline == KRC_TRUE)
		{
			if (krc_string_insert_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, '\r'))
			{
				ctx->cursor++;
			}
			if (krc_string_insert_char_w(text, (krc_size_t)(-1), max_length, ctx->cursor, '\n'))
			{
				ctx->cursor++;
			}
			ctx->length = krc_string_length_w(text, ctx->buffer_size);
		}
		break;

	//=========================================================================
	// Delete
	//=========================================================================
	case KRC_INPUT_KEY_DELETE:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
			ctx->cursor++;
		}
		text_len = krc_string_length_w(text, ctx->buffer_size);
		if (ctx->cursor < text_len)
		{
			krc_string_remove_char_w(text, text_len, max_length, ctx->cursor);
		}
		ctx->length = krc_string_length_w(text, ctx->buffer_size);
		break;

	//=========================================================================
	// Cursor left
	//=========================================================================
	case KRC_INPUT_KEY_LEFT:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
		}
		else if (ctx->cursor > 0u)
		{
			ctx->cursor--;
		}
		break;

	//=========================================================================
	// Cursor right
	//=========================================================================
	case KRC_INPUT_KEY_RIGHT:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
			ctx->cursor++;
		}
		else
		{
			text_len = krc_string_length_w(text, ctx->buffer_size);
			if (ctx->cursor < text_len)
			{
				ctx->cursor++;
			}
		}
		break;

	//=========================================================================
	// Cursor up
	//=========================================================================
	case KRC_INPUT_KEY_UP:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
		}
		if (ctx->multiline == KRC_TRUE && ctx->cursor_line > 0u)
		{
			/* 윗 줄의 같은 컬럼 위치로 이동 */
			krc_size_t target_line   = ctx->cursor_line - 1u;
			krc_size_t target_column = ctx->cursor_column;
			krc_size_t pos    = 0u;
			krc_size_t line   = 0u;
			krc_size_t column = 0u;
			ctx->cursor = 0u;
			while (text[pos] != 0x0000)
			{
				if (line == target_line && column == target_column)
				{
					ctx->cursor = pos;
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
		break;

	//=========================================================================
	// Cursor down
	//=========================================================================
	case KRC_INPUT_KEY_DOWN:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
		}
		if (ctx->multiline == KRC_TRUE)
		{
			/* 아랫 줄의 같은 컬럼 위치로 이동 */
			krc_size_t target_line   = ctx->cursor_line + 1u;
			krc_size_t target_column = ctx->cursor_column;
			krc_size_t pos    = 0u;
			krc_size_t line   = 0u;
			krc_size_t column = 0u;
			krc_size_t found  = 0u;
			while (text[pos] != 0x0000)
			{
				if (line == target_line && column == target_column)
				{
					ctx->cursor = pos;
					found = 1u;
					break;
				}
				if (text[pos] == '\n')
				{
					if (line == target_line) /* 줄 끝 도달 */
					{
						ctx->cursor = pos;
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
				ctx->cursor = pos; /* 마지막 줄 끝 */
			}
		}
		break;

	//=========================================================================
	// Home
	//=========================================================================
	case KRC_INPUT_KEY_HOME:
		if (ctx->hangul_union == KRC_TRUE)
			ctx->hangul_union = KRC_FALSE;
		if (ctx->multiline == KRC_TRUE)
		{
			/* 현재 줄의 맨 앞으로 이동 */
			while (ctx->cursor > 0u
				&& text[ctx->cursor - 1u] != '\n')
				ctx->cursor--;
		}
		else
		{
			ctx->cursor = 0u;
		}
		break;

	//=========================================================================
	// End
	//=========================================================================
	case KRC_INPUT_KEY_END:
		if (ctx->hangul_union == KRC_TRUE)
			ctx->hangul_union = KRC_FALSE;
		if (ctx->multiline == KRC_TRUE)
		{
			/* 현재 줄의 맨 끝으로 이동 */
			text_len = krc_string_length_w(text, ctx->buffer_size);
			while (ctx->cursor < text_len
				&& text[ctx->cursor] != '\r'
				&& text[ctx->cursor] != '\n')
				ctx->cursor++;
		}
		else
		{
			ctx->cursor = krc_string_length_w(text, ctx->buffer_size);
		}
		break;

	//=========================================================================
	// Insert
	//=========================================================================
	case KRC_INPUT_KEY_INSERT:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
		}
		ctx->insert_mode = (ctx->insert_mode == KRC_TRUE) ? KRC_FALSE : KRC_TRUE;
		break;

	//=========================================================================
	// 한/영 전환 (HANGUL = RALT)
	//=========================================================================
	case KRC_INPUT_KEY_HANGUL:
		if (ctx->hangul_union == KRC_TRUE)
		{
			ctx->hangul_union = KRC_FALSE;
		}
		if (ctx->keyboard_set == KRC_INPUT_KEYBOARD_SET_ENGLISH)
		{
			ctx->keyboard_set = KRC_INPUT_KEYBOARD_SET_KOREAN;
		}
		else
		{
			ctx->keyboard_set = KRC_INPUT_KEYBOARD_SET_ENGLISH;
		}
		break;

	default:
		break;
	}

	/* cursor_line / cursor_column 갱신 */
	{
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
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================

