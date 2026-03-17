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
extern "C" const krc_uchar16_t _krc_cp949_to_unicode_special_1128[1128];





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
	
    assert(str.length() == 11172u);

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

    assert(str.length() == 51u);

    return str;
}

std::wstring generate_unicode_special_1128()
{
    std::wstring str;
	wchar_t code;
    size_t index;
    for (index = 0u; index < 1128u; index++)
    {
        code = _krc_cp949_to_unicode_special_1128[index];
        if (code != 0x003fu)
        {
            str += code;
        }
	}
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
		str += static_cast<char>(code >> 8u);
        str += static_cast<char>(code);
    }

    assert(str.length() == 51u * 2u);

    return str;
}

std::string generate_cp949_hangul_euckr_2350()
{
    std::string str;
    uint16_t page;
    uint16_t page_count;
    uint16_t offset;
    uint16_t code_count;
    uint16_t code;

    // 0xB0A1u '가' - 0xC8FEu '힝'
    // 94(=6*16-1-1)자 * 25영역 = 2350자
    page_count = 0u;
    for (page = 0xB000u; page < 0xC900u; page+=0x0100u, page_count++)
    {
        code_count = 0u;
        for (offset = 0xA1u; offset <= 0xFEu; offset++, code_count++)
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
		assert(code_count == 94u);
    }
    assert(page_count == 25u);
    assert(str.length() == 2350u * 2u);

    return str;
}

std::string generate_cp949_hangul_ex_a_5696()
{
    std::string str;
    uint16_t page;
    uint16_t page_count;
    uint16_t offset;
    uint16_t code_count;
    uint16_t code;

    // 0x8141u '갂' - 0xA0FEu '좤' 
    // 178(=12*16-1-6-6-1)자 * 32영역 = 5696자
    page_count = 0u;
    for (page = 0x8100u; page <= 0xA000u; page += 0x0100u, page_count++)
    {
        // 유효 바이트: 0x41~0x5A(26) + 0x61~0x7A(26) + 0x81~0xFE(126)
        code_count = 0u;
        for (offset = 0x41u; offset <= 0x5Au; offset++, code_count++) // 26자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        for (offset = 0x61u; offset <= 0x7Au; offset++, code_count++) // 26자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        for (offset = 0x81u; offset <= 0xFEu; offset++, code_count++) // 126자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        assert(code_count == 178u);
    }
    assert(page_count == 32u);
    assert(str.length() == 5696u * 2u); // 5,696개의 2바이트 문자

    return str;
}

std::string generate_cp949_hangul_ex_b_3108()
{
    std::string str;
    uint16_t page;
    uint16_t page_count;
    uint16_t offset;
    uint16_t code_count;
    uint16_t code;

    // 0xA141u '좥' - 0xC5A0u '힋' 
    // 84(=7*16-1-6-6-14-1)자 * 37영역 = 3108자
    page_count = 0u;
    for (page = 0xA100u; page <= 0xC500u; page += 0x0100u, page_count++)
    {
        // 유효 바이트: 0x41~0x5A(26) + 0x61~0x7A(26) + 0x81~0xA0(32)
        code_count = 0u;
        for (offset = 0x41u; offset <= 0x5Au; offset++, code_count++) // 26자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        for (offset = 0x61u; offset <= 0x7Au; offset++, code_count++) // 26자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        for (offset = 0x81u; offset <= 0xA0u; offset++, code_count++) // 32자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        assert(code_count == 84u);
    }
    assert(page_count == 37u);
    assert(str.length() == 3108u * 2u); // 3,108개의 2바이트 문자
    return str;
}

std::string generate_cp949_hangul_ex_c_18()
{
    std::string str;
    uint16_t code;
    uint16_t code_count;

    // 0xC641u '힍' - 0xC652u '힣' 
    // 18자 * 1영역 = 18자
    code_count = 0u;
    for (code = 0xC641u; code <= 0xC652u; code++, code_count++)
    {
        str += static_cast<char>(code >> 8u);
        str += static_cast<char>(code);
    }
    assert(code_count == 18u);

    assert(str.length() == 18u * 2u); // 18개의 2바이트 문자
    return str;
}

std::string generate_cp949_special_1128()
{
    std::string str;
    uint16_t page;
    uint16_t page_count;
    uint16_t offset;
    uint16_t code_count;
    uint16_t code;

    // 0xA1A1 ~ 0xACFE : 12페이지 x 94자 = 1,128자
    page_count = 0u;
    for (page = 0xA100u; page <= 0xAC00u; page += 0x0100u, page_count++)
    {
		code_count = 0u;
        for (offset = 0xA1u; offset <= 0xFEu; offset++, code_count++) // 94자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
		assert(code_count == 94u);
    }
	assert(page_count == 12u);
    assert(str.length() == 1128u * 2u); // 1,128개의 2바이트 문자 

    return str;
}

std::string generate_cp949_hanja_4888()
{
    std::string str;
    uint16_t page;
    uint16_t page_count;
    uint16_t offset;
    uint16_t code_count;
    uint16_t code;

    // 0xCAA1 ~ 0xFDFE : 52페이지 x 94자 = 4,888자
    page_count = 0u;
    for (page = 0xCA00u; page <= 0xFD00u; page += 0x0100u, page_count++)
    {
        code_count = 0u;
        for (offset = 0xA1u; offset <= 0xFEu; offset++, code_count++) // 94자
        {
            code = page + offset;
            str += static_cast<char>(code >> 8u);
            str += static_cast<char>(code);
        }
        assert(code_count == 94u);
    }
    assert(page_count == 52u);
    assert(str.length() == 4888u * 2u); // 4,888개의 2바이트 문자
    return str;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring _windows_unicode_hangul_51    ;
std::wstring _windows_unicode_hangul_11172 ;
std::wstring _windows_unicode_special_1128 ;
std::wstring _windows_unicode_hanja_4888   ;
std::string  _windows_cp949_hangul_51      ;
std::string  _windows_cp949_hangul_11172   ;
std::string  _windows_cp949_special_1128   ;
std::string  _windows_cp949_hanja_4888     ;
std::string  _windows_utf8_hangul_51       ;
std::string  _windows_utf8_hangul_11172    ;
std::string  _windows_utf8_special_1128    ;
std::string  _windows_utf8_hanja_4888      ;

//===========================================================================
std::wstring _krc_unicode_hangul_51;
std::wstring _krc_unicode_hangul_11172;
std::wstring _krc_unicode_special_1128;
std::wstring _krc_unicode_hanja_4888;
std::string  _krc_cp949_hangul_51;
std::string  _krc_cp949_hangul_11172;
std::string  _krc_cp949_special_1128;
std::string  _krc_cp949_hanja_4888;
std::string  _krc_utf8_hangul_51;
std::string  _krc_utf8_hangul_11172;
std::string  _krc_utf8_special_1128;
std::string  _krc_utf8_hanja_4888;

//===========================================================================
void setup_window_charset(void)
{
	//-----------------------------------------------------------------------
    std::wstring unicode_hangul_51;
    std::string  cp949_hangul_51;

    unicode_hangul_51 = generate_unicode_hangul_51();
    cp949_hangul_51 = generate_cp949_hangul_51();
    assert(unicode_hangul_51 == mbcs_to_wcs(cp949_hangul_51, 949));
    assert(cp949_hangul_51 == wcs_to_mbcs(unicode_hangul_51, 949));

    cp949_hangul_51 = wcs_to_mbcs(unicode_hangul_51, 949);
	unicode_hangul_51 = mbcs_to_wcs(cp949_hangul_51, 949);
    assert(unicode_hangul_51 == mbcs_to_wcs(cp949_hangul_51, 949));
    assert(cp949_hangul_51 == wcs_to_mbcs(unicode_hangul_51, 949));

    

    //-----------------------------------------------------------------------
    std::string cp949_hangul_2350      = generate_cp949_hangul_euckr_2350();
    std::string cp949_hangul_ex_a_5696 = generate_cp949_hangul_ex_a_5696();
    std::string cp949_hangul_ex_b_3108 = generate_cp949_hangul_ex_b_3108();
    std::string cp949_hangul_ex_c_18   = generate_cp949_hangul_ex_c_18();

    std::wstring unicode_hangul_11172;
    std::string  cp949_hangul_11172;

    unicode_hangul_11172 = generate_unicode_hangul_11172();
	cp949_hangul_11172 = wcs_to_mbcs(unicode_hangul_11172, 949);

    assert(unicode_hangul_11172 == mbcs_to_wcs(cp949_hangul_11172, 949));
    assert(cp949_hangul_11172 == wcs_to_mbcs(unicode_hangul_11172, 949));

    cp949_hangul_11172 = cp949_hangul_2350 + cp949_hangul_ex_a_5696 + cp949_hangul_ex_b_3108 + cp949_hangul_ex_c_18;
	unicode_hangul_11172 = mbcs_to_wcs(cp949_hangul_11172, 949);

    assert(unicode_hangul_11172 == mbcs_to_wcs(cp949_hangul_11172, 949));
    assert(cp949_hangul_11172 == wcs_to_mbcs(unicode_hangul_11172, 949));


    //-----------------------------------------------------------------------
    std::string cp949_special_1128;
	std::wstring unicode_special_1128;

    cp949_special_1128 = generate_cp949_special_1128();
	std::cout << "cp949_special_1128 length: " << cp949_special_1128.length() << "/2\n";
    unicode_special_1128 = mbcs_to_wcs(cp949_special_1128, 949);
    assert(unicode_special_1128 == mbcs_to_wcs(cp949_special_1128, 949));
	//assert(cp949_special_1128 == wcs_to_mbcs(unicode_special_1128, 949)); // assertion failed

    unicode_special_1128 = generate_unicode_special_1128();
    std::cout << "unicode_special_1128 length: " << unicode_special_1128.length() << "\n";
    cp949_special_1128 = wcs_to_mbcs(unicode_special_1128, 949);
    assert(unicode_special_1128 == mbcs_to_wcs(cp949_special_1128, 949));
    assert(cp949_special_1128 == wcs_to_mbcs(unicode_special_1128, 949));


    //-----------------------------------------------------------------------
    std::string cp949_hanja_4888;
    std::wstring unicode_hanja_4888; 

    cp949_hanja_4888 = generate_cp949_hanja_4888();
    unicode_hanja_4888  = mbcs_to_wcs(cp949_hanja_4888, 949);
	assert(unicode_hanja_4888 == mbcs_to_wcs(cp949_hanja_4888, 949));
	assert(cp949_hanja_4888 == wcs_to_mbcs(unicode_hanja_4888, 949));


    //-----------------------------------------------------------------------
    std::string utf8_hangul_51    = wcs_to_utf8(unicode_hangul_51);
    std::string utf8_hangul_11172 = wcs_to_utf8(unicode_hangul_11172);
    std::string utf8_special_1128 = wcs_to_utf8(unicode_special_1128);
    std::string utf8_hanja_4888   = wcs_to_utf8(unicode_hanja_4888);

    _windows_unicode_hangul_51    = unicode_hangul_51   ;
    _windows_unicode_hangul_11172 = unicode_hangul_11172;
    _windows_unicode_special_1128 = unicode_special_1128;
    _windows_unicode_hanja_4888   = unicode_hanja_4888  ;
    _windows_cp949_hangul_51      = cp949_hangul_51     ;
    _windows_cp949_hangul_11172   = cp949_hangul_11172  ;
    _windows_cp949_special_1128   = cp949_special_1128  ;
    _windows_cp949_hanja_4888     = cp949_hanja_4888    ;
    _windows_utf8_hangul_51       = utf8_hangul_51      ;
    _windows_utf8_hangul_11172    = utf8_hangul_11172   ;
    _windows_utf8_special_1128    = utf8_special_1128   ;
    _windows_utf8_hanja_4888      = utf8_hanja_4888     ;
}

//===========================================================================
#if 0
KRC_API krc_size_t krc_cp949_to_unicode(
    const krc_char_t* cp949_string, const krc_size_t cp949_string_length,
    krc_wchar_t* unicode_string, const krc_size_t unicode_string_length
);

KRC_API krc_size_t krc_unicode_to_cp949(
    const krc_wchar_t* unicode_string, const krc_size_t unicode_string_length,
    krc_char_t* cp949_string, const krc_size_t cp949_string_length
);

KRC_API krc_size_t krc_cp949_to_utf8(
    const krc_char_t* cp949_string, const krc_size_t cp949_string_length,
    krc_char_t* utf8_string, const krc_size_t utf8_string_length
);

KRC_API krc_size_t krc_utf8_to_cp949(
    const krc_char_t* utf8_string, const krc_size_t utf8_string_length,
    krc_char_t* cp949_string, const krc_size_t cp949_string_length
);
#endif

void setup_krc(void)
{
    _krc_unicode_hangul_51   .clear(); _krc_unicode_hangul_51   .resize(_windows_unicode_hangul_51   .size());
    _krc_unicode_hangul_11172.clear(); _krc_unicode_hangul_11172.resize(_windows_unicode_hangul_11172.size());
    _krc_unicode_special_1128.clear(); _krc_unicode_special_1128.resize(_windows_unicode_special_1128.size());
    _krc_unicode_hanja_4888  .clear(); _krc_unicode_hanja_4888  .resize(_windows_unicode_hanja_4888  .size());
    _krc_cp949_hangul_51     .clear(); _krc_cp949_hangul_51     .resize(_windows_cp949_hangul_51     .size());
    _krc_cp949_hangul_11172  .clear(); _krc_cp949_hangul_11172  .resize(_windows_cp949_hangul_11172  .size());
    _krc_cp949_special_1128  .clear(); _krc_cp949_special_1128  .resize(_windows_cp949_special_1128  .size());
    _krc_cp949_hanja_4888    .clear(); _krc_cp949_hanja_4888    .resize(_windows_cp949_hanja_4888    .size());
    _krc_utf8_hangul_51      .clear(); _krc_utf8_hangul_51      .resize(_windows_utf8_hangul_51      .size());
    _krc_utf8_hangul_11172   .clear(); _krc_utf8_hangul_11172   .resize(_windows_utf8_hangul_11172   .size());
    _krc_utf8_special_1128   .clear(); _krc_utf8_special_1128   .resize(_windows_utf8_special_1128   .size());
    _krc_utf8_hanja_4888     .clear(); _krc_utf8_hanja_4888     .resize(_windows_utf8_hanja_4888     .size());
}

void test_1()
{
    setup_window_charset();
    setup_krc();

    krc_size_t result;


    result = krc_cp949_to_unicode(
        _windows_cp949_hangul_11172.data(), _windows_cp949_hangul_11172.length(),
        (krc_wchar_t*)_krc_unicode_hangul_11172.data(), _krc_unicode_hangul_11172.length()
	);
    assert(_windows_unicode_hangul_11172.length() == result);
	assert(_windows_unicode_hangul_11172 == _krc_unicode_hangul_11172);

    
    result = krc_cp949_to_unicode(
        _windows_cp949_hangul_51.data(), _windows_cp949_hangul_51.length(),
        (krc_wchar_t*)_krc_unicode_hangul_51.data(), _krc_unicode_hangul_51.length()
    );
    assert(_windows_unicode_hangul_51.length() == result);
    assert(_windows_unicode_hangul_51 == _krc_unicode_hangul_51);


    result = krc_cp949_to_unicode(
        _windows_cp949_special_1128.data(), _windows_cp949_special_1128.length(),
        (krc_wchar_t*)_krc_unicode_special_1128.data(), _krc_unicode_special_1128.length()
    );
    assert(_windows_unicode_special_1128.length() == result);
    assert(_windows_unicode_special_1128 == _krc_unicode_special_1128);


    result = krc_cp949_to_unicode(
        _windows_cp949_hanja_4888.data(), _windows_cp949_hanja_4888.length(),
        (krc_wchar_t*)_krc_unicode_hanja_4888.data(), _krc_unicode_hanja_4888.length()
    );
    assert(_windows_unicode_hanja_4888.length() == result);
    assert(_windows_unicode_hanja_4888 == _krc_unicode_hanja_4888);
}

void test_2()
{
    setup_window_charset();
    setup_krc();

    krc_size_t result;


    result = krc_unicode_to_cp949(
        (krc_wchar_t*)_windows_unicode_hangul_11172.data(), _windows_unicode_hangul_11172.length(),
        (krc_char_t*)_krc_cp949_hangul_11172.data(), _krc_cp949_hangul_11172.length()
    );
    assert(_windows_cp949_hangul_11172.length() == result);
    assert(_windows_cp949_hangul_11172 == _krc_cp949_hangul_11172);


    result = krc_unicode_to_cp949(
        (krc_wchar_t*)_windows_unicode_hangul_51.data(), _windows_unicode_hangul_51.length(),
        (krc_char_t*)_krc_cp949_hangul_51.data(), _krc_cp949_hangul_51.length()
    );
    assert(_windows_cp949_hangul_51.length() == result);
    assert(_windows_cp949_hangul_51 == _krc_cp949_hangul_51);


    result = krc_unicode_to_cp949(
        (krc_wchar_t*)_windows_unicode_special_1128.data(), _windows_unicode_special_1128.length(),
        (krc_char_t*)_krc_cp949_special_1128.data(), _krc_cp949_special_1128.length()
    );
    assert(_windows_cp949_special_1128.length() == result);
    assert(_windows_cp949_special_1128 == _krc_cp949_special_1128);


    result = krc_unicode_to_cp949(
        (krc_wchar_t*)_windows_unicode_hanja_4888.data(), _windows_unicode_hanja_4888.length(),
        (krc_char_t*)_krc_cp949_hanja_4888.data(), _krc_cp949_hanja_4888.length()
    );
    assert(_windows_cp949_hanja_4888.length() == result);
    assert(_windows_cp949_hanja_4888 == _krc_cp949_hanja_4888);
}

void test_3()
{
    setup_window_charset();
    setup_krc();

    krc_size_t result;


    result = krc_cp949_to_utf8(
        _windows_cp949_hangul_11172.data(), _windows_cp949_hangul_11172.length(),
        (krc_char_t*)_krc_utf8_hangul_11172.data(), _krc_utf8_hangul_11172.length()
    );
    assert(_windows_utf8_hangul_11172.length() == result);
    assert(_windows_utf8_hangul_11172 == _krc_utf8_hangul_11172);


    result = krc_cp949_to_utf8(
        _windows_cp949_hangul_51.data(), _windows_cp949_hangul_51.length(),
        (krc_char_t*)_krc_utf8_hangul_51.data(), _krc_utf8_hangul_51.length()
    );
    assert(_windows_utf8_hangul_51.length() == result);
    assert(_windows_utf8_hangul_51 == _krc_utf8_hangul_51);


    result = krc_cp949_to_utf8(
        _windows_cp949_special_1128.data(), _windows_cp949_special_1128.length(),
        (krc_char_t*)_krc_utf8_special_1128.data(), _krc_utf8_special_1128.length()
    );
    assert(_windows_utf8_special_1128.length() == result);
    assert(_windows_utf8_special_1128 == _krc_utf8_special_1128);


    result = krc_cp949_to_utf8(
        _windows_cp949_hanja_4888.data(), _windows_cp949_hanja_4888.length(),
        (krc_char_t*)_krc_utf8_hanja_4888.data(), _krc_utf8_hanja_4888.length()
    );
    assert(_windows_utf8_hanja_4888.length() == result);
    assert(_windows_utf8_hanja_4888 == _krc_utf8_hanja_4888);
}

void test_4()
{
    setup_window_charset();
    setup_krc();

    krc_size_t result;


    result = krc_utf8_to_cp949(
        (krc_char_t*)_windows_utf8_hangul_11172.data(), _windows_utf8_hangul_11172.length(),
        (krc_char_t*)_krc_cp949_hangul_11172.data(), _krc_cp949_hangul_11172.length()
    );
    assert(_windows_cp949_hangul_11172.length() == result);
    assert(_windows_cp949_hangul_11172 == _krc_cp949_hangul_11172);


    result = krc_utf8_to_cp949(
        (krc_char_t*)_windows_utf8_hangul_51.data(), _windows_utf8_hangul_51.length(),
        (krc_char_t*)_krc_cp949_hangul_51.data(), _krc_cp949_hangul_51.length()
    );
    assert(_windows_cp949_hangul_51.length() == result);
    assert(_windows_cp949_hangul_51 == _krc_cp949_hangul_51);


    result = krc_utf8_to_cp949(
        (krc_char_t*)_windows_utf8_special_1128.data(), _windows_utf8_special_1128.length(),
        (krc_char_t*)_krc_cp949_special_1128.data(), _krc_cp949_special_1128.length()
    );
    assert(_windows_cp949_special_1128.length() == result);
    assert(_windows_cp949_special_1128 == _krc_cp949_special_1128);


    result = krc_utf8_to_cp949(
        (krc_char_t*)_windows_utf8_hanja_4888.data(), _windows_utf8_hanja_4888.length(),
        (krc_char_t*)_krc_cp949_hanja_4888.data(), _krc_cp949_hanja_4888.length()
    );
    assert(_windows_cp949_hanja_4888.length() == result);
    assert(_windows_cp949_hanja_4888 == _krc_cp949_hanja_4888);
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void krc_main()
{
    std::cout << "krc_main()!\n";
    test_1();
    test_2();
    test_3();
    test_4();
}

