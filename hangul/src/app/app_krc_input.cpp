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
              << "  cur=" << ctx.cursor_offset
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
    if (ctx.key_mode != KRC_INPUT_KEY_MODE_HANGUL)
        krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
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
    check("cursor == 9", ctx.cursor_offset == 9);

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
// KRC_INPUT_KEY_LSHIFT 수식자를 OR해서 shift 입력 처리
/////////////////////////////////////////////////////////////////////////////
static void test_latin_shift()
{
    std::cout << "[test_latin_shift]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_H);   // H
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_I);   // I
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_SPACE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_1);       // !
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_2);       // @
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_3);       // #
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_GRAVE);   // ~
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_MINUS);   // _
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_EQUAL);   // +
    print_buf("HI !@#~_+", ctx);
    check("buffer == \"HI !@#~_+\"", buf_equals(buf, L"HI !@#~_+"));
    check("length == 9", ctx.length == 9);
    check("cursor == 9", ctx.cursor_offset == 9);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 3: 한글 기본 조합 (초성 + 중성 → 음절)
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_syllable()
{
    std::cout << "[test_hangul_syllable]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);

    // R(ㄱ) → 자음 단독, 조합 시작
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    check("R(ㄱ): composing=true",    ctx.hangul_composing == KRC_TRUE);
    check("R(ㄱ): cursor==0",         ctx.cursor_offset == 0);
    check("R(ㄱ): buf[0]==0x3131",    buf[0] == 0x3131);

    // K(ㅏ) → ㄱ+ㅏ = 가
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("가", ctx);
    check("RK -> 가(0xAC00)",          buf_equals(buf, L"\xAC00"));
    check("RK: cursor==0 (조합중)",    ctx.cursor_offset == 0);
    check("RK: composing=true",        ctx.hangul_composing == KRC_TRUE);

    // D(ㅇ) + J(ㅓ) → 어
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_J);
    print_buf("어", ctx);
    check("DJ -> 어(0xC5B4)",          buf_equals(buf, L"\xC5B4"));
    check("DJ: length==1",             ctx.length == 1);
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

    // R(ㄱ) K(ㅏ) S(ㄴ) → 간
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    print_buf("간", ctx);
    check("RKS -> 간(0xAC04)",  buf_equals(buf, L"\xAC04"));
    check("RKS: length==1",     ctx.length == 1);
    check("RKS: cursor==0",     ctx.cursor_offset == 0);

    // R(ㄱ) K(ㅏ) R(ㄱ) → 각
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    print_buf("각", ctx);
    check("RKR -> 각(0xAC01)",  buf_equals(buf, L"\xAC01"));
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

    // R K S K → 가나 (ㄴ이 다음 초성으로 분리)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("가나", ctx);
    check("RKSK -> 가나",       buf_equals(buf, L"\xAC00\xB098"));
    check("RKSK: length==2",    ctx.length == 2);
    check("RKSK: cursor==1",    ctx.cursor_offset == 1);

    // G K S R M F → 한글
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_G);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_M);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_F);
    print_buf("한글", ctx);
    check("GKSRMF -> 한글",     buf_equals(buf, L"\xD55C\xAE00"));
    check("GKSRMF: length==2",  ctx.length == 2);
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

    // R K R T → 갃 (ㄳ=ㄱ+ㅅ 겹받침) U+AC03
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_T);
    print_buf("갃(ㄳ겹받침)", ctx);
    check("RKRT -> 갃(0xAC03)",      buf_equals(buf, L"\xAC03"));

    // 겹받침 + 모음 → 분리: 갃+ㅏ → 각+사
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("각사", ctx);
    check("RKRTK -> 각사",           buf_equals(buf, L"\xAC01\xC0AC"));
    check("RKRTK: length==2",        ctx.length == 2);

    // R K S G → 갆 (ㄶ=ㄴ+ㅎ 겹받침) U+AC06
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_G);
    print_buf("갆(ㄶ겹받침)", ctx);
    check("RKSG -> 갆(0xAC06)",      buf_equals(buf, L"\xAC06"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 7: 복합 중성 조합
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_compound_jungseong()
{
    std::cout << "[test_hangul_compound_jungseong]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // D(ㅇ) H(ㅗ) K(ㅏ) → ㅗ+ㅏ=ㅘ → 와 U+C640
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("와", ctx);
    check("DHK -> 와(0xC640)",    buf_equals(buf, L"\xC640"));

    // 와 + S(ㄴ) → 완 U+C644
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    print_buf("완", ctx);
    check("DHKS -> 완(0xC644)",   buf_equals(buf, L"\xC644"));

    // S(ㄴ) N(ㅜ) P(ㅔ) → ㅜ+ㅔ=ㅞ → 눼 U+B23C
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_N);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_P);
    print_buf("눼(ㅞ)", ctx);
    check("SNP -> 눼(0xB23C)",    buf_equals(buf, L"\xB23C"));
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 8: 한글 SHIFT (쌍자음, 겹모음)
// KRC_INPUT_KEY_LSHIFT 수식자를 OR해서 shift 입력 처리
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_shift()
{
    std::cout << "[test_hangul_shift]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // shift+R(ㄲ) + K(ㅏ) → 까 U+AE4C
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_R);  // ㄲ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);  // ㅏ
    print_buf("까", ctx);
    check("shift+R, K -> 까(0xAE4C)",   buf_equals(buf, L"\xAE4C"));

    // shift+E(ㄸ) + J(ㅓ) → 떠 U+B5A0
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_E);  // ㄸ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_J);  // ㅓ
    print_buf("떠", ctx);
    check("shift+E, J -> 떠(0xB5A0)",   buf_equals(buf, L"\xB5A0"));

    // shift+T(ㅆ) + L(ㅣ) → 씨 U+C528
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_T);  // ㅆ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_L);  // ㅣ
    print_buf("씨", ctx);
    check("shift+T, L -> 씨(0xC528)",   buf_equals(buf, L"\xC528"));

    // shift+Q(ㅃ) 단독 → 자음이므로 composing
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_Q);  // ㅃ
    check("shift+Q -> ㅃ composing",    ctx.hangul_composing == KRC_TRUE);
    check("shift+Q: buf[0]==0x3143",     buf[0] == 0x3143);

    // shift+O(ㅒ) → 단독 모음이므로 cursor 전진
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_O);  // ㅒ
    print_buf("ㅒ", ctx);
    check("shift+O -> ㅒ(0x3152)",      buf[0] == 0x3152);
    check("shift+O: cursor==1",         ctx.cursor_offset == 1);
    check("shift+O: composing=false",   ctx.hangul_composing == KRC_FALSE);

    // shift+P(ㅖ) → 단독 모음
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LSHIFT | KRC_INPUT_KEY_P);  // ㅖ
    check("shift+P -> ㅖ(0x3156)",      buf[0] == 0x3156);
    check("shift+P: cursor==1",         ctx.cursor_offset == 1);
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

    // 간 → BS → 가 (종성 삭제)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("간->가", ctx);
    check("BS: 간->가(0xAC00)",     buf_equals(buf, L"\xAC00"));
    check("BS: composing=true",     ctx.hangul_composing == KRC_TRUE);
    check("BS: cursor==0",          ctx.cursor_offset == 0);

    // 가 → BS → ㄱ (중성 삭제)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("가->ㄱ", ctx);
    check("BS: 가->ㄱ(0x3131)",     buf_equals(buf, L"\x3131"));
    check("BS: composing=true",     ctx.hangul_composing == KRC_TRUE);

    // ㄱ → BS → empty (초성 삭제)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("ㄱ->empty", ctx);
    check("BS: ㄱ->empty",          buf_equals(buf, L""));
    check("BS: composing=false",    ctx.hangul_composing == KRC_FALSE);
    check("BS: cursor==0",          ctx.cursor_offset == 0);
    check("BS: length==0",          ctx.length == 0);

    // 빈 상태에서 BS → no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    check("BS on empty: cursor==0", ctx.cursor_offset == 0);
    check("BS on empty: length==0", ctx.length == 0);

    // 겹받침 BS 분리: 갃(ㄳ) → BS → 각
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_T);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("갃->각", ctx);
    check("BS: 갃(ㄳ)->각(0xAC01)", buf_equals(buf, L"\xAC01"));

    // 영문 BS
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_I);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("hi->h", ctx);
    check("latin BS: hi->h",        buf_equals(buf, L"h"));
    check("latin BS: cursor==1",    ctx.cursor_offset == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 10: Delete 키
/////////////////////////////////////////////////////////////////////////////
static void test_delete_key()
{
    std::cout << "[test_delete_key]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);

    // cursor=끝(3) → DELETE → no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    check("DEL at end: length==3",      ctx.length == 3);
    check("DEL at end: cursor==3",      ctx.cursor_offset == 3);

    // HOME → DELETE → "bc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    print_buf("bc", ctx);
    check("DEL: abc->bc",               buf_equals(buf, L"bc"));
    check("DEL: length==2",             ctx.length == 2);
    check("DEL: cursor==0",             ctx.cursor_offset == 0);

    // DELETE 반복 → "" 
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    check("DEL: bc->c",                 buf_equals(buf, L"c"));
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    check("DEL: c->empty",              buf_equals(buf, L""));
    check("DEL: empty length==0",       ctx.length == 0);

    // 한글 조합 중 DELETE → commit(cursor 전진) 후 다음 글자 삭제
    // "간"(composing, cursor=0) → HANGUL 전환(commit: cursor=1) → HOME(cursor=0) 후
    // 영문 'b' 삽입 → "b간" → HOME(cursor=0) → DELETE → 'b' 삭제 → "간"
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S); // 간(composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL); // 영문 전환, commit: cursor=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);   // cursor=0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B); // 'b' at cursor=0 → "b간"(cursor=1)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME); // cursor=0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE); // 'b' 삭제 → "간"
    print_buf("간", ctx);
    check("DEL: b간->간(0xAC04)",        buf_equals(buf, L"\xAC04"));
    check("DEL: cursor==0 after del",   ctx.cursor_offset == 0);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 11: 커서 이동 (Left/Right/Home/End)
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
    check("abc: cursor==3",         ctx.cursor_offset == 3);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: cursor==2",        ctx.cursor_offset == 2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: cursor==1",        ctx.cursor_offset == 1);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right: cursor==2",       ctx.cursor_offset == 2);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home: cursor==0",        ctx.cursor_offset == 0);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end: cursor==3",         ctx.cursor_offset == 3);

    // cursor=0에서 LEFT → no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left at 0: cursor==0",   ctx.cursor_offset == 0);

    // cursor=끝에서 RIGHT → no-op
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right at end: cursor==3", ctx.cursor_offset == 3);

    // 한글 조합 중 LEFT → composing 해제, cursor 그대로(전진 없음)
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가(composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left during composing: composing=false", ctx.hangul_composing == KRC_FALSE);
    check("left during composing: cursor==0",       ctx.cursor_offset == 0);

    // 한글 조합 중 RIGHT → composing 해제 + cursor 전진
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가(composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("right during composing: composing=false", ctx.hangul_composing == KRC_FALSE);
    check("right during composing: cursor==1",       ctx.cursor_offset == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 12: 한/영 전환
/////////////////////////////////////////////////////////////////////////////
static void test_hangul_latin_toggle()
{
    std::cout << "[test_hangul_latin_toggle]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    check("init: LATIN",    ctx.key_mode == KRC_INPUT_KEY_MODE_LATIN);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
    check("toggle->HANGUL", ctx.key_mode == KRC_INPUT_KEY_MODE_HANGUL);

    // 가 입력 (composing=true, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    check("hangul: buf[0]==가(0xAC00)",         buf[0] == 0xAC00);
    check("hangul: cursor==0",                  ctx.cursor_offset == 0);

    // 영문 전환 → composing 확정(커서 전진), cursor=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL);
    check("toggle->LATIN",                      ctx.key_mode == KRC_INPUT_KEY_MODE_LATIN);
    check("composing stopped",                  ctx.hangul_composing == KRC_FALSE);
    check("cursor advanced after toggle==1",    ctx.cursor_offset == 1);

    // cursor=1에서 영문 'r' → 삽입모드이므로 가 뒤에 삽입 → "가r"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);
    print_buf("가r", ctx);
    check("latin: buf[0]==가(0xAC00)",   buf[0] == 0xAC00);
    check("latin: buf[1]=='r'",          buf[1] == 'r');
    check("latin: length==2",            ctx.length == 2);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 13: 삽입/덮어쓰기 모드
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

    // 덮어쓰기 모드 + HOME → 'x' → "xbc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_INSERT);
    check("INSERT: insert_mode=false",  ctx.insert_mode == KRC_FALSE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_X);
    print_buf("overwrite xbc", ctx);
    check("overwrite: buffer==\"xbc\"", buf_equals(buf, L"xbc"));
    check("overwrite: length==3",       ctx.length == 3);
    check("overwrite: cursor==1",       ctx.cursor_offset == 1);

    // 삽입 모드 복귀 → cursor=1에서 'y' → "xybc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_INSERT);
    check("INSERT again: insert_mode=true", ctx.insert_mode == KRC_TRUE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_Y);
    print_buf("insert xybc", ctx);
    check("insert: buffer==\"xybc\"",   buf_equals(buf, L"xybc"));
    check("insert: length==4",          ctx.length == 4);
    check("insert: cursor==2",          ctx.cursor_offset == 2);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 14: 버퍼 크기 경계
/////////////////////////////////////////////////////////////////////////////
static void test_buffer_limit()
{
    std::cout << "[test_buffer_limit]\n";
    krc_wchar_t buf[5] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 5); // max_length = 4 (buffer_size-1)

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E); // 5번째 → 거부
    print_buf("abcd (e 거부)", ctx);
    check("buffer limit: length==4",    ctx.length == 4);
    check("buffer limit: cursor==4",    ctx.cursor_offset == 4);
    check("buffer limit: buf[4]==0",    buf[4] == 0x0000);

    // HOME → 'z' 삽입 시도 → 거부 (버퍼 꽉 참)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_Z);
    check("insert at limit: length==4", ctx.length == 4);
    check("insert at limit: buf[0]=='a'", buf[0] == 'a');
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 15: krc_inputw_put_char() 직접 호출
/////////////////////////////////////////////////////////////////////////////
static void test_put_char_direct()
{
    std::cout << "[test_put_char_direct]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    // 영문자 직접 입력
    krc_inputw_put_char(&ctx, L'A');
    krc_inputw_put_char(&ctx, L'B');
    krc_inputw_put_char(&ctx, L'C');
    check("put_char ABC: buf",          buf_equals(buf, L"ABC"));
    check("put_char ABC: length==3",    ctx.length == 3);
    check("put_char ABC: cursor==3",    ctx.cursor_offset == 3);

    // 한글 자모 직접 입력
    init_ctx(ctx, buf, 64);
    krc_inputw_put_char(&ctx, 0x3131);  // ㄱ
    check("put_char ㄱ: composing=true",    ctx.hangul_composing == KRC_TRUE);
    check("put_char ㄱ: cursor==0",         ctx.cursor_offset == 0);

    krc_inputw_put_char(&ctx, 0x314F);  // ㅏ → ㄱ+ㅏ = 가
    print_buf("가", ctx);
    check("put_char ㄱ+ㅏ -> 가(0xAC00)",  buf_equals(buf, L"\xAC00"));
    check("put_char 가: composing=true",    ctx.hangul_composing == KRC_TRUE);

    krc_inputw_put_char(&ctx, 0x3134);  // ㄴ → 가+ㄴ = 간
    check("put_char 가+ㄴ -> 간(0xAC04)", buf_equals(buf, L"\xAC04"));

    krc_inputw_put_char(&ctx, 0x314F);  // ㅏ → 간+ㅏ → 가+나
    print_buf("가나", ctx);
    check("put_char 간+ㅏ -> 가나",        buf_equals(buf, L"\xAC00\xB098"));
    check("put_char 가나: length==2",       ctx.length == 2);
    check("put_char 가나: cursor==1",       ctx.cursor_offset == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 16: cursor_line / cursor_column (단일 라인)
/////////////////////////////////////////////////////////////////////////////
static void test_cursor_pos_singleline()
{
    std::cout << "[test_cursor_pos_singleline]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    check("init: line==0",  ctx.cursor_line == 0);
    check("init: col==0",   ctx.cursor_column == 0);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    check("abc: line==0",   ctx.cursor_line == 0);
    check("abc: col==3",    ctx.cursor_column == 3);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_LEFT);
    check("left: col==2",   ctx.cursor_column == 2);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home: line==0",  ctx.cursor_line == 0);
    check("home: col==0",   ctx.cursor_column == 0);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end: line==0",   ctx.cursor_line == 0);
    check("end: col==3",    ctx.cursor_column == 3);

    // 한글 조합 중: 기존 "abc" 이후에 가 조합 → cursor=3(col=3) 유지
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가 (composing, cursor=3)
    check("hangul composing: col==3",   ctx.cursor_column == 3);
    check("hangul composing: line==0",  ctx.cursor_line == 0);

    // RIGHT → 조합 확정 + cursor 전진 → cursor=4, col=4
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("after right: col==4",    ctx.cursor_column == 4);
    check("after right: line==0",   ctx.cursor_line == 0);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 17: cursor_line / cursor_column (멀티 라인)
/////////////////////////////////////////////////////////////////////////////
static void test_cursor_pos_multiline()
{
    std::cout << "[test_cursor_pos_multiline]\n";
    krc_wchar_t buf[128] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 128, KRC_TRUE); // multiline=true

    // "ab" 입력
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    check("ab: line==0",    ctx.cursor_line == 0);
    check("ab: col==2",     ctx.cursor_column == 2);

    // ENTER: \r\n 삽입 → line=1, col=0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    check("enter: line==1", ctx.cursor_line == 1);
    check("enter: col==0",  ctx.cursor_column == 0);

    // "cd" 입력 → line=1, col=2
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    check("cd: line==1",    ctx.cursor_line == 1);
    check("cd: col==2",     ctx.cursor_column == 2);

    // 두 번째 줄에서 ENTER → 세 번째 줄
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E);
    check("3rd: line==2",   ctx.cursor_line == 2);
    check("3rd: col==1",    ctx.cursor_column == 1);

    // UP: line2→line1, col=min(1,2)=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    check("up: line==1",    ctx.cursor_line == 1);
    check("up: col==1",     ctx.cursor_column == 1);

    // UP: line1→line0
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    check("up: line==0",    ctx.cursor_line == 0);
    check("up: col==1",     ctx.cursor_column == 1);

    // DOWN: line0→line1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DOWN);
    check("down: line==1",  ctx.cursor_line == 1);
    check("down: col==1",   ctx.cursor_column == 1);

    // HOME: 현재 줄 맨 앞
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    check("home: line==1",  ctx.cursor_line == 1);
    check("home: col==0",   ctx.cursor_column == 0);

    // END: 현재 줄 맨 끝 (cd → col=2)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end: line==1",   ctx.cursor_line == 1);
    check("end: col==2",    ctx.cursor_column == 2);

    // BS: 'd' 삭제 → col=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    check("bs: line==1",    ctx.cursor_line == 1);
    check("bs: col==1",     ctx.cursor_column == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 18: 버그 #1 수정 검증 — 조합 중 자음 연속 입력 커서 위치
//
//   krc_inputw_put_char_hangul_fail_jaeum() 에 cursor_advance() 가 추가되어
//   조합 완료된 글자를 지나쳐 커서가 이동한 후 새 자음이 삽입되어야 한다.
//
//   수정 전: 각(cursor=0) + ㄱ → buffer="ㄱ각"  cursor=0  (ㄱ이 앞으로 밀고 들어감)
//   수정 후: 각(cursor=0) + ㄱ → buffer="각ㄱ"  cursor=1  (올바른 순서)
/////////////////////////////////////////////////////////////////////////////
static void test_fix_fail_jaeum()
{
    std::cout << "[test_fix_fail_jaeum]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // ----- 1) 음절(각) + 조합 불가 자음(ㄱ) -----
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // 각
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // 각 + ㄱ → 각ㄱ
    print_buf("각ㄱ", ctx);
    check("fail_jaeum: buf==\"각ㄱ\"",        buf_equals(buf, L"\xAC01\x3131")); // 각=0xAC01, ㄱ=0x3131
    check("fail_jaeum: length==2",           ctx.length == 2);
    check("fail_jaeum: cursor==1",           ctx.cursor_offset == 1);  // ← 버그 수정 핵심 검증
    check("fail_jaeum: composing=true",      ctx.hangul_composing == KRC_TRUE);

    // ----- 2) 각ㄱ에서 ㅏ → 각가 (새 자음이 올바른 위치에 있으므로 정상 조합) -----
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // ㄱ+ㅏ → 가
    print_buf("각가", ctx);
    check("fail_jaeum+ㅏ: buf==\"각가\"",    buf_equals(buf, L"\xAC01\xAC00"));
    check("fail_jaeum+ㅏ: cursor==1",        ctx.cursor_offset == 1);
    check("fail_jaeum+ㅏ: composing=true",   ctx.hangul_composing == KRC_TRUE);

    // ----- 3) 각가 + ㄴ → 각간 -----
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S); // ㅏ + ㄴ → 간
    print_buf("각간", ctx);
    check("fail_jaeum+ㄴ: buf==\"각간\"",    buf_equals(buf, L"\xAC01\xAC04"));
    check("fail_jaeum+ㄴ: cursor==1",        ctx.cursor_offset == 1);

    // ----- 4) 각간 + ㅏ → 각가나 (간의 ㄴ이 나의 초성으로 분리) -----
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);
    print_buf("각가나", ctx);
    check("fail_jaeum 최종: buf==\"각가나\"", buf_equals(buf, L"\xAC01\xAC00\xB098"));
    check("fail_jaeum 최종: length==3",       ctx.length == 3);
    check("fail_jaeum 최종: cursor==2",        ctx.cursor_offset == 2);

    // ----- 5) 단독 자음 + 조합 불가 자음: ㄱ(composing) + ㄷ → ㄱㄷ -----
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ (composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E); // ㄷ — ㄱ과 조합 불가 → ㄱㄷ
    print_buf("ㄱㄷ", ctx);
    check("단독ㄱ+ㄷ: buf==\"ㄱㄷ\"",        buf_equals(buf, L"\x3131\x3137"));
    check("단독ㄱ+ㄷ: length==2",             ctx.length == 2);
    check("단독ㄱ+ㄷ: cursor==1",             ctx.cursor_offset == 1);
    check("단독ㄱ+ㄷ: composing=true",        ctx.hangul_composing == KRC_TRUE);

    // ----- 6) 연속 자음 3개: ㄱ → ㄷ → ㅅ → ㄱㄷㅅ -----
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_E); // ㄷ → ㄱㄷ, cursor=1
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_T); // ㅅ → ㄱㄷㅅ, cursor=2
    print_buf("ㄱㄷㅅ", ctx);
    check("연속ㄱㄷㅅ: buf==\"ㄱㄷㅅ\"",     buf_equals(buf, L"\x3131\x3137\x3145"));
    check("연속ㄱㄷㅅ: length==3",            ctx.length == 3);
    check("연속ㄱㄷㅅ: cursor==2",            ctx.cursor_offset == 2);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 19: ESC 키
/////////////////////////////////////////////////////////////////////////////
static void test_esc_key()
{
    std::cout << "[test_esc_key]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;

    // 가(composing) + ESC → composing=false, 글자 유지, cursor=0
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R);   // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K);   // 가 (composing)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ESC);
    print_buf("가(ESC후)", ctx);
    check("ESC: buf==\"가\"",        buf_equals(buf, L"\xAC00"));
    check("ESC: composing=false",    ctx.hangul_composing == KRC_FALSE);
    check("ESC: cursor==1",          ctx.cursor_offset == 1);
    check("ESC: length==1",          ctx.length == 1);

    // ESC 후 영문 입력 → cursor=1에서 insert → '가' 뒤에 삽입
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HANGUL); // 영문 전환
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);      // 'a' at cursor=1 → "가a"
    print_buf("가a", ctx);
    check("ESC후 latin: buf==\"가a\"", buf_equals(buf, L"\xAC00" L"a"));
    check("ESC후 latin: cursor==2",    ctx.cursor_offset == 2);
    check("ESC후 latin: length==2",    ctx.length == 2);

    // 조합 없는 상태에서 ESC → no-op (글자, 커서, 길이 변화 없음)
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ESC);
    check("ESC no-op: buf==\"a\"",   buf_equals(buf, L"a"));
    check("ESC no-op: cursor==1",    ctx.cursor_offset == 1);
    check("ESC no-op: composing=false", ctx.hangul_composing == KRC_FALSE);

    // 종성 포함 음절에서 ESC → 그대로 유지
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_S); // 간 (composing)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ESC);
    check("ESC 간: buf==\"간\"",          buf_equals(buf, L"\xAC04"));
    check("ESC 간: composing=false",      ctx.hangul_composing == KRC_FALSE);
    check("ESC 간: cursor==1",            ctx.cursor_offset == 1);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 20: CapsLock
/////////////////////////////////////////////////////////////////////////////
static void test_capslock()
{
    std::cout << "[test_capslock]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    check("init: capslock=false", ctx.capslock_mode == KRC_FALSE);

    // CAPSLOCK ON → 영문 대문자
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_CAPSLOCK);
    check("CAPSLOCK ON: capslock=true", ctx.capslock_mode == KRC_TRUE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H); // H
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_I); // I
    print_buf("HI", ctx);
    check("CAPSLOCK: buf==\"HI\"",  buf_equals(buf, L"HI"));
    check("CAPSLOCK: length==2",    ctx.length == 2);

    // CAPSLOCK OFF → 소문자 복귀
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_CAPSLOCK);
    check("CAPSLOCK OFF: capslock=false", ctx.capslock_mode == KRC_FALSE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_H); // h
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_I); // i
    print_buf("HIhi", ctx);
    check("CAPSLOCK OFF: buf==\"HIhi\"", buf_equals(buf, L"HIhi"));

    // 한글 모드에서 CAPSLOCK → shift 미적용 (한글에는 대소문자 없음)
    //   key_mode==HANGUL 이면 capslock 조건 불충족 → shift=false → ㄱ (ㄲ 아님)
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_CAPSLOCK); // capslock ON
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // 한글 모드: shift 없으므로 ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // ㅏ → 가
    print_buf("가(capslock in hangul)", ctx);
    check("CAPSLOCK in hangul: buf==\"가\"",      buf_equals(buf, L"\xAC00")); // ㄲ(0x3132) 아닌 ㄱ(0x3131)
    check("CAPSLOCK in hangul: composing=true",   ctx.hangul_composing == KRC_TRUE);

    // CAPSLOCK 키 토글로 캡스락 제어 검증
    init_ctx(ctx, buf, 64);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_CAPSLOCK); // capslock ON
    check("set_capslock TRUE: capslock=true", ctx.capslock_mode == KRC_TRUE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A); // 'A'
    check("set_capslock: buf[0]=='A'",        buf[0] == L'A');
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_CAPSLOCK); // capslock OFF
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A); // 'a'
    check("set_capslock off: buf[1]=='a'",    buf[1] == L'a');
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 21: Tab 키
/////////////////////////////////////////////////////////////////////////////
static void test_tab_key()
{
    std::cout << "[test_tab_key]\n";
    krc_wchar_t buf[64] = {};
    krc_inputw_t ctx;
    init_ctx(ctx, buf, 64);

    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_TAB);
    check("TAB: buf[0]=='\\t'",  buf[0] == L'\t');
    check("TAB: length==1",      ctx.length == 1);
    check("TAB: cursor==1",      ctx.cursor_offset == 1);

    // 탭 연속 3개
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_TAB);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_TAB);
    check("TAB x3: length==3",   ctx.length == 3);
    check("TAB x3: cursor==3",   ctx.cursor_offset == 3);

    // 한글 조합 중 TAB → 조합 commit 후 탭 삽입
    //   가(composing, cursor=0) + TAB → commit(cursor=1) → insert '\t' at 1 → cursor=2
    init_ctx(ctx, buf, 64);
    set_hangul(ctx);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_R); // ㄱ
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_K); // 가 (composing, cursor=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_TAB);
    print_buf("가\\t", ctx);
    check("TAB after hangul: buf[0]==가",     buf[0] == 0xAC00);
    check("TAB after hangul: buf[1]=='\\t'",  buf[1] == L'\t');
    check("TAB after hangul: length==2",      ctx.length == 2);
    check("TAB after hangul: cursor==2",      ctx.cursor_offset == 2);
    check("TAB after hangul: composing=false", ctx.hangul_composing == KRC_FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// 테스트 22: 버그 #5 수정 검증 — 멀티라인 줄 합침 후 cursor_line
//
//   Backspace/Delete로 개행(\n)을 삭제한 후 cursor_line이 올바르게 계산되는지 검증.
//   current_line_offset 캐시가 무효화된 상태에서 partial scan이 방지되어야 한다.
/////////////////////////////////////////////////////////////////////////////
static void test_multiline_line_merge()
{
    std::cout << "[test_multiline_line_merge]\n";
    krc_wchar_t buf[128] = {};
    krc_inputw_t ctx;

    // ----- 1) Backspace로 줄 합침 -----
    init_ctx(ctx, buf, 128, KRC_TRUE);
    // "ab\r\ncd" 구성
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_D);
    check("setup: line==1",   ctx.cursor_line == 1);
    check("setup: col==2",    ctx.cursor_column == 2);
    check("setup: length==6", ctx.length == 6); // a,b,\r,\n,c,d

    // HOME(line1) → cursor=4, BS → \n 삭제 → "ab\rcd"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_HOME);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("BS removes \\n", ctx);
    check("BS \\n: line==0",    ctx.cursor_line == 0);
    check("BS \\n: length==5",  ctx.length == 5);

    // BS → \r 삭제 → "abcd" (단일 라인)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_BACKSPACE);
    print_buf("BS removes \\r", ctx);
    check("BS \\r: line==0",    ctx.cursor_line == 0);
    check("BS \\r: col==2",     ctx.cursor_column == 2);
    check("BS \\r: length==4",  ctx.length == 4);

    // END → 줄 끝 (line=0, col=4)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("end after merge: line==0", ctx.cursor_line == 0);
    check("end after merge: col==4",  ctx.cursor_column == 4);

    // ----- 2) Delete로 줄 합침 -----
    init_ctx(ctx, buf, 128, KRC_TRUE);
    // "a\r\nb" 구성
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    // cursor=3(end, line=1, col=1)

    // UP → line0, END → cursor at '\r' (col=1)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("del setup: line==0",  ctx.cursor_line == 0);
    check("del setup: col==1",   ctx.cursor_column == 1);

    // DELETE × 2 → \r 삭제 후 \n 삭제 → "ab"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE); // \r 삭제
    check("DEL \\r: line==0",    ctx.cursor_line == 0);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE); // \n 삭제
    print_buf("после DEL merges lines", ctx);
    check("DEL \\n: line==0",    ctx.cursor_line == 0);
    check("DEL \\n: length==2",  ctx.length == 2);
    check("DEL \\n: col==1",     ctx.cursor_column == 1);

    // RIGHT → 'b' 위치 (col=2, line=0)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_RIGHT);
    check("RIGHT after merge: line==0", ctx.cursor_line == 0);
    check("RIGHT after merge: col==2",  ctx.cursor_column == 2);

    // ----- 3) 3줄 → 가운데 줄을 Delete로 합침 -----
    init_ctx(ctx, buf, 128, KRC_TRUE);
    // "a\r\nb\r\nc"
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_A);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_B);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_ENTER);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_C);
    check("3line: line==2",   ctx.cursor_line == 2);
    check("3line: col==1",    ctx.cursor_column == 1);

    // UP×2 → line0, END → at '\r'
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_UP);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_END);
    check("3line up: line==0",   ctx.cursor_line == 0);

    // DELETE×2 → \r,\n 삭제 → "ab\r\nc" (line0="ab", line1="c")
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DELETE);
    check("3line DEL: line==0",    ctx.cursor_line == 0);
    check("3line DEL: length==5",  ctx.length == 5); // a,\r,\n,b,\r,\n,c(7) → \r,\n 삭제 → ab\r\nc(5)

    // DOWN → line1 (c)
    krc_inputw_put_key(&ctx, KRC_INPUT_KEY_DOWN);
    check("3line down: line==1",   ctx.cursor_line == 1);
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void krc_input_main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "===== krc_inputw 테스트 =====\n\n";

    test_latin_basic();               std::cout << "\n";
    test_latin_shift();               std::cout << "\n";
    test_hangul_syllable();           std::cout << "\n";
    test_hangul_jongseong();          std::cout << "\n";
    test_hangul_split_jongseong();    std::cout << "\n";
    test_hangul_compound_jongseong(); std::cout << "\n";
    test_hangul_compound_jungseong(); std::cout << "\n";
    test_hangul_shift();              std::cout << "\n";
    test_backspace();                 std::cout << "\n";
    test_delete_key();                std::cout << "\n";
    test_cursor_movement();           std::cout << "\n";
    test_hangul_latin_toggle();       std::cout << "\n";
    test_insert_overwrite();          std::cout << "\n";
    test_buffer_limit();              std::cout << "\n";
    test_put_char_direct();           std::cout << "\n";
    test_cursor_pos_singleline();     std::cout << "\n";
    test_cursor_pos_multiline();      std::cout << "\n";
    test_fix_fail_jaeum();            std::cout << "\n";
    test_esc_key();                   std::cout << "\n";
    test_capslock();                  std::cout << "\n";
    test_tab_key();                   std::cout << "\n";
    test_multiline_line_merge();      std::cout << "\n";

    std::cout << "===== 결과: PASS=" << s_pass << "  FAIL=" << s_fail << " =====\n";
}
