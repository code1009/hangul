/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <vector>

#include <Windows.h>

#include "../lib/bf/bf.h"
#include "../lib/krc/krc.h"


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring mbcs_to_wcs(std::string input, UINT codepage)
{
	int len = MultiByteToWideChar(codepage, 0, input.c_str(), -1, NULL, 0);


	if (len > 0)
	{
		std::vector<wchar_t> buf(len);


		MultiByteToWideChar(codepage, 0, input.c_str(), -1, &buf[0], len);

		return std::wstring(&buf[0]);
	}

	return std::wstring();
}

std::string wcs_to_mbcs(std::wstring input, UINT codepage)
{
	int len = WideCharToMultiByte(codepage, 0, input.c_str(), -1, NULL, 0, NULL, NULL);


	if (len > 0)
	{
		std::vector<char> buf(len);


		WideCharToMultiByte(codepage, 0, input.c_str(), -1, &buf[0], len, NULL, NULL);

		return std::string(&buf[0]);
	}

	return std::string();
}

std::string utf8_to_mbcs(std::string /*input*/utf8, UINT codepage)
{
	//	std::string  utf8 ;
	std::wstring utf16;
	std::string  mbcs;


	//	utf8  = input;
	utf16 = mbcs_to_wcs(utf8, CP_UTF8);
	mbcs = wcs_to_mbcs(utf16, codepage);

	return mbcs;
}

std::string mbcs_to_utf8(std::string /*input*/mbcs, UINT codepage)
{
	std::string  utf8;
	std::wstring utf16;
	//	std::string  mbcs ;


	//	mbcs  = input;
	utf16 = mbcs_to_wcs(mbcs, codepage);
	utf8 = wcs_to_mbcs(utf16, CP_UTF8);

	return utf8;
}

std::string wcs_to_utf8(std::wstring input)
{
	return wcs_to_mbcs(input, CP_UTF8);
}

std::wstring utf8_to_wcs(std::string input)
{
	return mbcs_to_wcs(input, CP_UTF8);
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class text_canvas
{
private:
	uint32_t _cx;
	uint32_t _cy;
	std::vector<std::string> _bitmap_buffer;

public:
	text_canvas() :
		_cx(128u), _cy(32u)
	{
		setup();
	}

private:
	void setup(void)
	{
		_bitmap_buffer.resize(_cy);
		for (uint32_t y = 0u; y < _cy; y++)
		{
			_bitmap_buffer[y].resize(_cx, '.');
		}
	}

public:
	void clear(void)
	{
		for (uint32_t y = 0u; y < _cy; y++)
		{
			for (uint32_t x = 0u; x < _cx; x++)
			{
				_bitmap_buffer[y][x] = '.';
			}
		}
	}

public:
	void output(void)
	{
		for (uint32_t y = 0u; y < _cy; y++)
		{
			for (uint32_t x = 0u; x < _cx; x++)
			{
				printf("%c", _bitmap_buffer[y][x]);
			}
			printf("\r\n");
		}
	}

public:
	void draw_pixel(const uint32_t x, const uint32_t y, const uint8_t pixel)
	{
		if (x >= _cx || y >= _cy)
		{
			return;
		}
		_bitmap_buffer[y][x] = pixel ? '@' : '.';
	}

	void draw_font_bitmap(const uint32_t start_x, const uint32_t start_y, bf_font_bitmap_t* font_bitmap)
	{
		static const uint8_t mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

		uint32_t x;
		uint32_t y;

		for (y = 0u; y < font_bitmap->font_bitmap_cy; y++)
		{
			for (x = 0u; x < font_bitmap->font_bitmap_cx; x++)
			{
				if (font_bitmap->font_bitmap_pointer[y * font_bitmap->font_bitmap_cx_size + x / 8u] & mask[x % 8])
				{
					draw_pixel(start_x + x, start_y + y, 1);
				}
			}
		}
	}

	void draw_cp949_string(const uint32_t start_x, const uint32_t start_y, const char* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = start_x;
		uint32_t y = start_y;

		const char* src = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			bf_uint16_t char_code;

			if ((unsigned char)*src < 0x80)
			{
				if (*src == '\n')
				{
					x = start_x;
					y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
					src += 1;
					continue;
				}
				else if (*src == '\r')
				{
					x = start_x;
					src += 1;
					continue;
				}
				else if (*src == '\t')
				{
					x += 8*8u;
					src += 1;
					continue;
				}
				else if (*src == '\b')
				{
					x -= (font_bitmap_drawn ? font_bitmap.font_bitmap_cx : 8u);
					src += 1;
					continue;
				}
				else if (*src < 0x20)
				{
					char_code = '?';
					src += 1;
				}
				else if (*src == 0x7F)
				{
					char_code = '?';
					src += 1;
				}
				else
				{
					char_code = (bf_uint16_t)(unsigned char)*src;
					src += 1;
				}
			}
			else
			{
				if (*(src + 1) != '\0')
				{
					char_code = ((bf_uint16_t)(unsigned char)*src << 8) | (bf_uint16_t)(unsigned char)*(src + 1);
					src += 2;
				}
				else
				{
					char_code = '?';
					src += 1;
				}
			}

			bf_get_cp949_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;
		}
	}

	void draw_unicode_string(const uint32_t start_x, const uint32_t start_y, const wchar_t* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = start_x;
		uint32_t y = start_y;

		const wchar_t* src = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			bf_uint16_t char_code;
			if (*src == '\n')
			{
				x = start_x;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				src += 1;
				continue;
			}
			else if (*src == '\r')
			{
				x = start_x;
				src += 1;
				continue;
			}
			else if (*src == '\t')
			{
				x += 8 * 8u;
				src += 1;
				continue;
			}
			else if (*src == '\b')
			{
				x -= (font_bitmap_drawn ? font_bitmap.font_bitmap_cx : 8u);
				src += 1;
				continue;
			}
			else if (*src < 0x20)
			{
				char_code = '?';
				src += 1;
			}
			else if (*src == 0x7F)
			{
				char_code = '?';
				src += 1;
			}
			else
			{
				char_code = (bf_uint16_t)*src;
				src += 1;
			}

			bf_get_unicode_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;
		}
	}

	void draw_utf8_string(const uint32_t start_x, const uint32_t start_y, const uint8_t* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = start_x;
		uint32_t y = start_y;

		const uint8_t* src = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			uint8_t utf8_char;
			const uint8_t* utf8_char_pointer = NULL;
			uint32_t utf8_char_length = 0;

			if (*src == '\n')
			{
				x = start_x;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				src += 1;
				continue;
			}
			else if (*src == '\r')
			{
				x = start_x;
				src += 1;
				continue;
			}
			else if (*src == '\t')
			{
				x += 8 * 8u;
				src += 1;
				continue;
			}
			else if (*src == '\b')
			{
				x -= (font_bitmap_drawn ? font_bitmap.font_bitmap_cx : 8u);
				src += 1;
				continue;
			}
			else if (*src < 0x20)
			{
				utf8_char = '?';
				src += 1;

				utf8_char_pointer = &utf8_char;
				utf8_char_length = 1;
			}
			else if (*src == 0x7F)
			{
				utf8_char = '?';
				src += 1;

				utf8_char_pointer = &utf8_char;
				utf8_char_length = 1;
			}
			else
			{
				utf8_char_pointer = (uint8_t*)src;
				utf8_char_length = (uint32_t)krc_utf8_char_size((const krc_char_t*)src);

				if (utf8_char_length == 0)
				{
					utf8_char = '?';
					utf8_char_pointer = &utf8_char;
					utf8_char_length = 1;

					src += 1;
				}
				else
				{
					src += utf8_char_length;
				}
			}

			bf_get_utf8_bitmap(ctx, utf8_char_pointer, utf8_char_length , &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;
		}
	}
};

static void test1()
{
	text_canvas c;


	c.draw_cp949_string(1u, 0u, "A한글.똠방각하\n!ф詰☎★");
	c.output();


	printf("\r\n\r\n");


	c.clear();
	c.draw_unicode_string(1u, 0u, L"b유니코드\n!ф詰");
	c.output();


	printf("\r\n\r\n");


	c.clear();
	std::string utf8 = mbcs_to_utf8("c유니코드\n!ф詰", CP_ACP);
	c.draw_utf8_string(1u, 0u, (const uint8_t*)utf8.c_str());
	c.output();
}

void bf_main()
{
    std::cout << "bf_main()!\n";

    bf_context_default_init();

	test1();
}
