/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <assert.h>
#include <Windows.h>

#include "../lib/krc/krc.h"
#include "../lib/krc_input/krc_input.h"

#include <iostream>
#include <string>

#include "windows_charset.hpp"




/////////////////////////////////////////////////////////////////////////////
// 유틸리티
/////////////////////////////////////////////////////////////////////////////

static std::wstring buf_to_wstr(const krc_wchar_t* buf)
{
    std::wstring result;
    while (*buf)
        result += static_cast<wchar_t>(*buf++);
    return result;
}

static bool buf_equals(const krc_wchar_t* buf, const wchar_t* expected)
{
    return buf_to_wstr(buf) == std::wstring(expected);
}

static int s_pass = 0;
static int s_fail = 0;

static void check(const char* name, bool cond)
{
    if (cond)
    {
        std::cout << "  PASS  " << name << "\n";
        s_pass++;
    }
    else
    {
        std::cout << "  FAIL  " << name << "\n";
        s_fail++;
    }
}

static void print_buf(const char* label, const krc_inputw_t& ctx)
{
    std::string utf8 = wcs_to_utf8(buf_to_wstr(ctx.buffer_pointer));
    std::cout << "    \"" << utf8 << "\""
              << "  len=" << ctx.length
              << "  cur=" << ctx.cursor
              << "  composing=" << ctx.hangul_composing
              << "  [" << label << "]\n";
}

static void init_ctx(krc_inputw_t& ctx, krc_wchar_t* buf, krc_size_t size,
                     krc_bool_t multiline = KRC_FALSE)
{
    krc_inputw_init(&ctx, buf, size, multiline);
}

static void set_hangul(krc_inputw_t& ctx)
{
    if (ctx.keyboard_type != KRC_INPUT_KEYBOARD_TYPE_HANGUL)
        krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
}

static void shift_on(krc_inputw_t& ctx)
{
    if (ctx.shift_state == KRC_FALSE)
        krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT);
}

static void shift_off(krc_inputw_t& ctx)
{
    if (ctx.shift_state == KRC_TRUE)
        krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 1: 영문 기본 입력
/////////////////////////////////////////////////////////////////////////////
static void test_latin_basic()
{
    std::cout << "[test_latin_basic]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_L);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_L);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_O);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_SPACE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_1);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_3);
    print_buf("hello 123", ctx);
    check("buffer == \"hello 123\"", buf_equals(buf, L"hello 123"));
    check("length == 9", ctx.length == 9);
    check("cursor == 9", ctx.cursor == 9);

    // 특수문자
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_GRAVE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_MINUS);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_EQUAL);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_SEMICOLON);
    print_buf("`-=;", ctx);
    check("special chars: buffer == \"`-=;\"", buf_equals(buf, L"`-=;"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 2: 영문 SHIFT 입력
/////////////////////////////////////////////////////////////////////////////
static void test_latin_shift()
{
    std::cout << "[test_latin_shift]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_I);
    shift_off(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_SPACE);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_1);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_3);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_GRAVE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_MINUS);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_EQUAL);
    shift_off(ctx);
    print_buf("HI !@#~_+", ctx);
    check("buffer == \"HI !@#~_+\"", buf_equals(buf, L"HI !@#~_+"));
    check("length == 9", ctx.length == 9);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 3: 한글 기본 조합 (초성 + 중성 -> 음절)
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_syllable()
{
    std::cout << "[test_hangul_syllable]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // R(ㄱ) -> 자음 단독, 조합 시작
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    check("R(ㄱ): composing=true", ctx.hangul_composing == KRC_TRUE);
    check("R(ㄱ): cursor==0", ctx.cursor == 0);

    // K(ㅏ) -> 가 조합
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("가", ctx);
    check("RK -> 가", buf_equals(buf, L"\xAC00"));
    check("RK: cursor==0 (조합중)", ctx.cursor == 0);
    check("RK: composing=true", ctx.hangul_composing == KRC_TRUE);

    // D(ㅇ) + J(ㅓ) -> 어
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_J);
    print_buf("어", ctx);
    check("DJ -> 어", buf_equals(buf, L"\xC5B4"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 4: 종성 추가
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_jongseong()
{
    std::cout << "[test_hangul_jongseong]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // R(ㄱ) K(ㅏ) S(ㄴ) -> 간
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    print_buf("간", ctx);
    check("RKS -> 간", buf_equals(buf, L"\xAC04"));
    check("RKS: length==1", ctx.length == 1);

    // R(ㄱ) K(ㅏ) R(ㄱ) -> 각
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    print_buf("각", ctx);
    check("RKR -> 각", buf_equals(buf, L"\xAC01"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 5: 종성이 다음 음절 초성으로 분리
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_split_jongseong()
{
    std::cout << "[test_hangul_split_jongseong]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // RKSK -> 가나 (ㄴ이 다음 초성으로 분리)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("가나", ctx);
    check("RKSK -> 가나", buf_equals(buf, L"\xAC00\xB098"));
    check("RKSK: length==2", ctx.length == 2);
    check("RKSK: cursor==1 (조합중)", ctx.cursor == 1);

    // GKSRMF -> 한글
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_G);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_M);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_F);
    print_buf("한글", ctx);
    check("GKSRMF -> 한글", buf_equals(buf, L"\xD55C\xAE00"));
    check("GKSRMF: length==2", ctx.length == 2);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 6: 겹받침 조합 및 분리
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_compound_jongseong()
{
    std::cout << "[test_hangul_compound_jongseong]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // R K(가) R(각=ㄱ종성) T(ㅅ추가) -> 갃(ㄳ=ㄱ+ㅅ 겹받침)  U+AC03
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_T);
    print_buf("갃(ㄳ겹받침)", ctx);
    check("RKRT -> 갃(ㄳ)", buf_equals(buf, L"\xAC03"));

    // 겹받침+모음 -> 분리: 갃+ㅏ -> 각+사
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("각사", ctx);
    check("RKRTK -> 각사", buf_equals(buf, L"\xAC01\xC0AC"));
    check("RKRTK: length==2", ctx.length == 2);

    // ㄴ+ㅎ=ㄶ 겹받침  U+AC06
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_G);
    print_buf("갆(ㄶ겹받침)", ctx);
    check("RKSG -> 갆(ㄶ)", buf_equals(buf, L"\xAC06"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 7: 복합 중성 조합
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_compound_jungseong()
{
    std::cout << "[test_hangul_compound_jungseong]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // D(ㅇ) H(ㅗ) K(ㅏ) -> ㅗ+ㅏ=ㅘ -> 와  U+C640
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("와", ctx);
    check("DHK -> 와(ㅘ)", buf_equals(buf, L"\xC640"));

    // 와+ㄴ 종성 -> 완  U+C644
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    print_buf("완", ctx);
    check("DHKS -> 완", buf_equals(buf, L"\xC644"));

    // ㄴ(S) ㅜ(N) ㅔ(P) -> ㅜ+ㅔ=ㅞ -> 눼  U+B240
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_N);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_P);
    print_buf("눼(ㅞ)", ctx);
    check("SNP -> 눼(ㅞ)", buf_equals(buf, L"\xB23C"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 8: 한글 SHIFT (쌍자음, 겹모음)
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_shift()
{
    std::cout << "[test_hangul_shift]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // shift+R(ㄲ) + K(ㅏ) -> 까  U+AE4C
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    shift_off(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("까", ctx);
    check("shift+R, K -> 까", buf_equals(buf, L"\xAE4C"));

    // shift+E(ㄸ) + J(ㅓ) -> 떠  U+B5A0
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E);
    shift_off(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_J);
    print_buf("떠", ctx);
    check("shift+E, J -> 떠", buf_equals(buf, L"\xB5A0"));

    // shift+Q(ㅃ) 단독 입력
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_Q);
    shift_off(ctx);
    check("shift+Q -> ㅃ composing", ctx.hangul_composing == KRC_TRUE);
    check("shift+Q: buf[0]==ㅃ(0x3143)", buf[0] == 0x3143);

    // shift+O(ㅒ): 모음이므로 커서 전진
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_O);
    shift_off(ctx);
    print_buf("ㅒ", ctx);
    check("shift+O -> ㅒ(0x3152)", buf[0] == 0x3152);
    check("shift+O: cursor==1", ctx.cursor == 1);

    // shift+P(ㅖ) 단독
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    shift_on(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_P);
    shift_off(ctx);
    check("shift+P -> ㅖ(0x3156)", buf[0] == 0x3156);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 9: Backspace
/////////////////////////////////////////////////////////////////////////////
static void test_backspace()
{
    std::cout << "[test_backspace]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // 간 -> BS -> 가
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("간->가", ctx);
    check("BS: 간->가", buf_equals(buf, L"\xAC00"));
    check("BS: composing=true", ctx.hangul_composing == KRC_TRUE);

    // 가 -> BS -> ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("가->ㄱ", ctx);
    check("BS: 가->ㄱ", buf_equals(buf, L"\x3131"));

    // ㄱ -> BS -> empty
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("ㄱ->empty", ctx);
    check("BS: ㄱ->empty", buf_equals(buf, L""));
    check("BS: composing=false", ctx.hangul_composing == KRC_FALSE);
    check("BS: cursor==0", ctx.cursor == 0);

    // 빈 상태에서 BS -> no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    check("BS on empty: cursor==0", ctx.cursor == 0);

    // 겹받침 BS 분리: 갃(ㄳ) -> BS -> 각
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_T);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("갃->각", ctx);
    check("BS: 갃(ㄳ)->각", buf_equals(buf, L"\xAC01"));

    // 영문 BS
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_I);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("hi->h", ctx);
    check("latin BS: hi->h", buf_equals(buf, L"h"));
    check("latin BS: cursor==1", ctx.cursor == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 10: 커서 이동 (Left/Right/Home/End)
/////////////////////////////////////////////////////////////////////////////
static void test_cursor_movement()
{
    std::cout << "[test_cursor_movement]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    check("abc: cursor==3", ctx.cursor == 3);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: cursor==2", ctx.cursor == 2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: cursor==1", ctx.cursor == 1);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right: cursor==2", ctx.cursor == 2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home: cursor==0", ctx.cursor == 0);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end: cursor==3", ctx.cursor == 3);

    // 0에서 left -> no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left at 0: cursor==0", ctx.cursor == 0);

    // 끝에서 right -> no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right at end: cursor==3", ctx.cursor == 3);

    // 한글 조합 중 left -> composing 해제, 커서 전진 없음
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가(composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left during composing: composing=false", ctx.hangul_composing == KRC_FALSE);
    check("left during composing: cursor==0", ctx.cursor == 0);

    // 한글 조합 중 right -> composing 해제 + cursor 전진
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가(composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right during composing: composing=false", ctx.hangul_composing == KRC_FALSE);
    check("right during composing: cursor==1", ctx.cursor == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 11: 한/영 전환
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_latin_toggle()
{
    std::cout << "[test_hangul_latin_toggle]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    check("init: LATIN", ctx.keyboard_type == KRC_INPUT_KEYBOARD_TYPE_LATIN);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
    check("toggle->HANGUL", ctx.keyboard_type == KRC_INPUT_KEYBOARD_TYPE_HANGUL);

    // 가 입력 (composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    check("hangul: buf[0]==가(0xAC00)", buf[0] == 0xAC00);

    // 영문 전환 -> 조합 해제, cursor 유지
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
    check("toggle->LATIN", ctx.keyboard_type == KRC_INPUT_KEYBOARD_TYPE_LATIN);
    check("composing stopped on toggle", ctx.hangul_composing == KRC_FALSE);

    // cursor=0에서 영문 'r' -> 삽입 모드이므로 가 앞에 삽입
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    print_buf("r가", ctx);
    check("latin after toggle: buf[0]=='r'", buf[0] == 'r');
    check("latin after toggle: buf[1]==가(0xAC00)", buf[1] == 0xAC00);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 12: 삽입/덮어쓰기 모드
/////////////////////////////////////////////////////////////////////////////
static void test_insert_overwrite()
{
    std::cout << "[test_insert_overwrite]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    check("init: insert_mode=true", ctx.insert_mode == KRC_TRUE);

    // 덮어쓰기 + HOME -> 'x' -> "xbc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_INSERT);
    check("INSERT: insert_mode=false", ctx.insert_mode == KRC_FALSE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_X);
    print_buf("overwrite xbc", ctx);
    check("overwrite: buffer==\"xbc\"", buf_equals(buf, L"xbc"));
    check("overwrite: length==3", ctx.length == 3);

    // 삽입 모드 복귀 -> cursor=1 -> 'y' -> "xybc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_INSERT);
    check("INSERT again: insert_mode=true", ctx.insert_mode == KRC_TRUE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_Y);
    print_buf("insert xybc", ctx);
    check("insert: buffer==\"xybc\"", buf_equals(buf, L"xybc"));
    check("insert: length==4", ctx.length == 4);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 13: 버퍼 크기 경계
/////////////////////////////////////////////////////////////////////////////
static void test_buffer_limit()
{
    std::cout << "[test_buffer_limit]\n";
    krc_wchar_t buf[5] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 5);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E); // 5번째 -> 거부
    print_buf("abcd (e 거부)", ctx);
    check("buffer limit: length==4", ctx.length == 4);
    check("buffer limit: cursor==4", ctx.cursor == 4);
    check("buffer limit: buf[4]==0", buf[4] == 0x0000);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 14: cursor_line / cursor_column (단일 라인)
/////////////////////////////////////////////////////////////////////////////
static void test_cursor_pos_singleline()
{
    std::cout << "[test_cursor_pos_singleline]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    // 초기 상태
    check("init: line==0", ctx.cursor_line == 0);
    check("init: col==0",  ctx.cursor_column == 0);

    // a b c 입력 → cursor=3, col=3
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    check("abc: line==0",  ctx.cursor_line == 0);
    check("abc: col==3",   ctx.cursor_column == 3);

    // Left → col=2
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: line==0", ctx.cursor_line == 0);
    check("left: col==2",  ctx.cursor_column == 2);

    // Home → col=0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home: line==0", ctx.cursor_line == 0);
    check("home: col==0",  ctx.cursor_column == 0);

    // End → col=3
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end: line==0",  ctx.cursor_line == 0);
    check("end: col==3",   ctx.cursor_column == 3);

    // 한글 입력: 가 (조합 중, cursor=3, col=3 유지)
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가
    check("hangul composing: line==0", ctx.cursor_line == 0);
    check("hangul composing: col==3",  ctx.cursor_column == 3);

    // Right → 조합 확정하고 cursor=4, col=4
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("after composing right: line==0", ctx.cursor_line == 0);
    check("after composing right: col==4",  ctx.cursor_column == 4);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 15: cursor_line / cursor_column (멀티 라인)
/////////////////////////////////////////////////////////////////////////////
static void test_cursor_pos_multiline()
{
    std::cout << "[test_cursor_pos_multiline]\n";
    krc_wchar_t buf[128] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 128, KRC_TRUE); // multiline=true

    // "ab\r\ncd" 입력
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    check("ab: line==0, col==2", ctx.cursor_line == 0 && ctx.cursor_column == 2);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER); // \r\n 삽입, cursor 전진 2
    check("after enter: line==1", ctx.cursor_line == 1);
    check("after enter: col==0",  ctx.cursor_column == 0);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    check("cd: line==1, col==2",  ctx.cursor_line == 1);
    check("cd: col==2",           ctx.cursor_column == 2);

    // 두 번째 줄에서 Enter → 세 번째 줄
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E);
    check("3rd line: line==2",    ctx.cursor_line == 2);
    check("3rd line: col==1",     ctx.cursor_column == 1);

    // Up: line 2 → line 1, col=min(1,2)=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    check("up: line==1",          ctx.cursor_line == 1);
    check("up: col==1",           ctx.cursor_column == 1);

    // Up: line 1 → line 0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    check("up: line==0",          ctx.cursor_line == 0);
    check("up: col==1",           ctx.cursor_column == 1);

    // Down: line 0 → line 1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DOWN);
    check("down: line==1",        ctx.cursor_line == 1);
    check("down: col==1",         ctx.cursor_column == 1);

    // Home: 현재 줄 맨 앞 → col=0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home multiline: line==1", ctx.cursor_line == 1);
    check("home multiline: col==0",  ctx.cursor_column == 0);

    // End: 현재 줄 맨 끝 → col=2 (cd)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end multiline: line==1",  ctx.cursor_line == 1);
    check("end multiline: col==2",   ctx.cursor_column == 2);

    // Backspace: 'd' 삭제 → col=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    check("bs multiline: line==1",   ctx.cursor_line == 1);
    check("bs multiline: col==1",    ctx.cursor_column == 1);
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void krc_input_main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "===== krc_input_unicode 테스트 =====\n\n";

    test_latin_basic();               std::cout << "\n";
    test_latin_shift();               std::cout << "\n";
    test_hangul_syllable();           std::cout << "\n";
    test_hangul_jongseong();          std::cout << "\n";
    test_hangul_split_jongseong();    std::cout << "\n";
    test_hangul_compound_jongseong(); std::cout << "\n";
    test_hangul_compound_jungseong(); std::cout << "\n";
    test_hangul_shift();              std::cout << "\n";
    test_backspace();                 std::cout << "\n";
    test_cursor_movement();           std::cout << "\n";
    test_hangul_latin_toggle();       std::cout << "\n";
    test_insert_overwrite();          std::cout << "\n";
    test_buffer_limit();              std::cout << "\n";
    test_cursor_pos_singleline();     std::cout << "\n";
    test_cursor_pos_multiline();      std::cout << "\n";

    std::cout << "===== 결과: PASS=" << s_pass << "  FAIL=" << s_fail << " =====\n";
}