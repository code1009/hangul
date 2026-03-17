/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <assert.h>
#include <Windows.h>

#include "../lib/krc/krc.h"

#include <iostream>
#include <vector>

#include "windows_charset.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring generate_unicode_hangul_11172()
{
    std::wstring str;
    wchar_t code;
    for (code = 0xAC00u; code <= 0xD7A3u; code++)
    {
        str += code;
    }
	
    assert(str.length() == 11172);

    return str;
}

std::wstring generate_unicode_hangul_51()
{
    std::wstring str;
    wchar_t code;
    for (code = 0x3131u; code <= 0x3163u; code++)
    {
        str += code;
    }

    assert(str.length() == 51);

    return str;
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string generate_cp949_hangul_51()
{
    std::string str;
    uint16_t code;
    for (code = 0xA4A1u; code <= 0xA4D3u; code++)
    {
        str += static_cast<char>(code);
		str += static_cast<char>(code >> 8);
    }

    assert(str.length() == 51);

    return str;
}

std::string generate_cp949_hangul_euckr_2350()
{
    std::string str;
    uint16_t code_page;
    uint16_t code_page_count;
    uint16_t code_offset;
    uint16_t code_count;
    uint16_t code;

    // 0xB0A1u '가' - 0xC8FEu '힝'
    // 94(=6*16-1-1)자 * 25영역 = 2350자
    code_page_count = 0u;
    for (code_page = 0xB000u; code_page < 0xC900u; code_page+=0x0100u, code_page_count++)
    {
        code_count = 0u;
        for (code_offset = 0xA1u; code_offset <= 0xFEu; code_offset++, code_count++)
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
            str += static_cast<char>(code >> 8u);
        }
		assert(code_count == 94u);
    }

    assert(code_page_count == 25u);
    assert(str.length() == 2350u * 2u);

    return str;
}

std::string generate_cp949_hangul_ex_a_5696()
{
    std::string str;
    uint16_t code_page;
    uint16_t code_offset;
    uint16_t code;

    // 0x8141u '갂' - 0xA0FEu '좤' 
    // 178(=12*16-1-6-6-1)자 * 32영역 = 5696자
    for (code_page = 0x8100u; code_page <= 0xA000u; code_page += 0x0100u)
    {
        // 유효 2nd 바이트: 0x41~0x5A(26) + 0x61~0x7A(26) + 0x81~0xFE(126)
        for (code_offset = 0x41u; code_offset <= 0x5Au; code_offset++) // 26자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
        for (code_offset = 0x61u; code_offset <= 0x7Au; code_offset++) // 26자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
        for (code_offset = 0x81u; code_offset <= 0xFEu; code_offset++) // 126자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
    }

    assert(str.length() == 5696 * 2); // 5,696개의 2바이트 문자
    return str;
}

std::string generate_cp949_hangul_ex_b_3108()
{
    std::string str;
    uint16_t code_page;
    uint16_t code_offset;
    uint16_t code;

    // 0xA141u '좥' - 0xC5A0u '힋' 
    // 84(=7*16-1-6-6-14-1)자 * 37영역 = 3108자
    for (code_page = 0xA100u; code_page <= 0xC500u; code_page += 0x0100u)
    {
        // 유효 2nd 바이트: 0x41~0x5A(26) + 0x61~0x7A(26) + 0x81~0xA0(32)
        for (code_offset = 0x41u; code_offset <= 0x5Au; code_offset++) // 26자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
        for (code_offset = 0x61u; code_offset <= 0x7Au; code_offset++) // 26자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
        for (code_offset = 0x81u; code_offset <= 0xA0u; code_offset++) // 32자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
    }

    assert(str.length() == 3108 * 2); // 3,108개의 2바이트 문자
    return str;
}

std::string generate_cp949_hangul_ex_c_18()
{
    std::string str;
    uint16_t code;

    // 0xC641u '힍' - 0xC652u '힣' 
    // 18자 * 1영역 = 18자
    for (code = 0xC641u; code <= 0xC652u; code++)
    {
        str += static_cast<char>(code >> 8);
        str += static_cast<char>(code);
    }

    assert(str.length() == 18 * 2); // 18개의 2바이트 문자
    return str;
}

std::string generate_cp949_special_1128()
{
    std::string str;
    uint16_t code_page;
    uint16_t code_offset;
    uint16_t code;

    // 0xA1A1 ~ 0xACFE : 12페이지 x 94자 = 1,128자
    for (code_page = 0xA100u; code_page <= 0xAC00u; code_page += 0x0100u)
    {
        for (code_offset = 0xA1u; code_offset <= 0xFEu; code_offset++) // 94자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
    }

    assert(str.length() == 1128 * 2); // 1,128개의 2바이트 문자
    return str;
}

std::string generate_cp949_hanja_4888()
{
    std::string str;
    uint16_t code_page;
    uint16_t code_offset;
    uint16_t code;

    // 0xCAA1 ~ 0xFDFE : 52페이지 x 94자 = 4,888자
    for (code_page = 0xCA00u; code_page <= 0xFD00u; code_page += 0x0100u)
    {
        for (code_offset = 0xA1u; code_offset <= 0xFEu; code_offset++) // 94자
        {
            code = code_page + code_offset;
            str += static_cast<char>(code >> 8);
            str += static_cast<char>(code);
        }
    }

    assert(str.length() == 4888 * 2); // 4,888개의 2바이트 문자
    return str;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_1()
{
    std::wstring u11172 = generate_unicode_hangul_11172();
    std::wstring u51 = generate_unicode_hangul_51();

    std::string cp51 = generate_cp949_hangul_51();
    assert(cp51.length() == 51 * 2);

    std::string cp2350 = generate_cp949_hangul_euckr_2350();
    std::string cp5696 = generate_cp949_hangul_ex_a_5696();
    std::string cp3108 = generate_cp949_hangul_ex_b_3108();
    std::string cp18 = generate_cp949_hangul_ex_c_18();
    
    std::string cp_hangul_all = cp2350 + cp5696 + cp3108 + cp18;
    assert(cp_hangul_all.length() == 11172 * 2);

    std::string cp_spec = generate_cp949_special_1128();
    std::string cp_hanja = generate_cp949_hanja_4888();

    std::cout << "All Hangul generation tests passed.\n";
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void krc_main()
{
    std::cout << "krc_main()!\n";
    //test_1();
}

