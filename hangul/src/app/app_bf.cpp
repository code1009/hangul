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

std::string utf8_to_mbcs(std::string utf8, UINT codepage)
{
	std::wstring utf16;
	std::string  mbcs;
	utf16 = mbcs_to_wcs(utf8, CP_UTF8);
	mbcs = wcs_to_mbcs(utf16, codepage);
	return mbcs;
}

std::string mbcs_to_utf8(std::string mbcs, UINT codepage)
{
	std::string  utf8;
	std::wstring utf16;
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
		_cx(128u+2), _cy(32u+2)
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
		static const uint8_t mask[8] = { 0x80u, 0x40u, 0x20u, 0x10u, 0x08u, 0x04u, 0x02u, 0x01u };

		uint32_t x;
		uint32_t y;

		for (y = 0u; y < font_bitmap->font_bitmap_cy; y++)
		{
			for (x = 0u; x < font_bitmap->font_bitmap_cx; x++)
			{
				if (font_bitmap->font_bitmap_pointer[y * font_bitmap->font_bitmap_cx_size + x / 8u] & mask[x % 8u])
				{
					draw_pixel(start_x + x, start_y + y, 1u);
				}
			}
		}
	}

	uint32_t inline tab_width(void)
	{
		return 8u * 8u;
	}

	uint32_t calc_cp949_backspace_width(bf_context_t* ctx, const char* str, const char* pos)
	{
		const char* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			unsigned char c = (unsigned char)*p;
			if (c == '\b')
			{
				depth++;
				p--;
			}
			else if (c == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
				p--;
			}
			else if (c < 0x80u)
			{
				bf_uint16_t code = (c < 0x20u || c == 0x7Fu) ? (bf_uint16_t)'?' : (bf_uint16_t)c;
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_cp949_bitmap(ctx, code, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
				p--;
			}
			else
			{
				if (p > str && (unsigned char)*(p - 1) >= 0x80u)
				{
					if (depth == 0)
					{
						bf_uint16_t code = ((bf_uint16_t)(unsigned char)*(p - 1) << 8) | (bf_uint16_t)c;
						bf_font_bitmap_t prev_bm;
						bf_get_cp949_bitmap(ctx, code, &prev_bm);
						bs_width = prev_bm.font_bitmap_cx;
						break;
					}
					depth--;
					p -= 2;
				}
				else
				{
					if (depth == 0) { break; }
					depth--;
					p--;
				}
			}
		}
		return bs_width;
	}

	uint32_t calc_unicode_backspace_width(bf_context_t* ctx, const wchar_t* str, const wchar_t* pos)
	{
		const wchar_t* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			wchar_t c = *p;
			if (c == L'\b')
			{
				depth++;
				p--;
			}
			else if (c == L'\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
				p--;
			}
			else
			{
				bf_uint16_t code = (c < 0x20 || c == 0x7F) ? (bf_uint16_t)'?' : (bf_uint16_t)c;
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_unicode_bitmap(ctx, code, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
				p--;
			}
		}
		return bs_width;
	}

	uint32_t calc_utf8l_backspace_width(bf_context_t* ctx, const uint8_t* str, const uint8_t* pos)
	{
		const uint8_t* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			uint8_t c = *p;
			if (c == '\b')
			{
				depth++;
				p--;
			}
			else if (c == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
				p--;
			}
			else if (c < 0x20u || c == 0x7Fu)
			{
				if (depth == 0)
				{
					uint8_t dummy = '?';
					bf_font_bitmap_t prev_bm;
					bf_get_utf8l_bitmap(ctx, &dummy, 1u, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
				p--;
			}
			else
			{
				const uint8_t* char_start = p;
				while (char_start > str && (*char_start & 0xC0u) == 0x80u)
					char_start--;
				uint32_t char_len = (uint32_t)krc_utf8_char_size((const krc_char_t*)char_start);
				if (char_len == 0u) char_len = 1u;
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_utf8l_bitmap(ctx, char_start, char_len, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
				p = char_start - 1;
			}
		}
		return bs_width;
	}

	uint32_t calc_utf8_backspace_width(bf_context_t* ctx, const uint8_t* str, const uint8_t* pos)
	{
		const uint8_t* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			uint8_t c = *p;
			if (c == '\b')
			{
				depth++;
				p--;
			}
			else if (c == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
				p--;
			}
			else if (c < 0x20u || c == 0x7Fu)
			{
				if (depth == 0)
				{
					uint8_t dummy = '?';
					bf_font_bitmap_t prev_bm;
					bf_get_utf8_bitmap(ctx, &dummy, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
				p--;
			}
			else
			{
				const uint8_t* char_start = p;
				while (char_start > str && (*char_start & 0xC0u) == 0x80u)
					char_start--;
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_utf8_bitmap(ctx, char_start, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				uint32_t char_len = (uint32_t)krc_utf8_char_size((const krc_char_t*)char_start);
				if (char_len == 0u) char_len = 1u;
				depth--;
				p = char_start - 1;
			}
		}
		return bs_width;
	}

	void draw_cp949_string(const uint32_t start_x, const uint32_t start_y, const char* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = 0u;
		uint32_t y = 0u;

		const char* src = str;
		const char* line_start = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			bf_uint16_t char_code;

			if ((unsigned char)*src < 0x80u)
			{
				if (*src == '\n')
				{
					x = 0u;
					y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
					src += 1u;
					line_start = src;
					continue;
				}
				else if (*src == '\r')
				{
					x = 0u;
					src += 1u;
					line_start = src;
					continue;
				}
				else if (*src == '\t')
				{
					x += tab_width();
					src += 1u;
					continue;
				}
				else if (*src == '\b')
				{
					uint32_t bs_width = calc_cp949_backspace_width(ctx, line_start, src);
					x = (x < bs_width) ? 0u : (x - bs_width);
					src += 1u;
					continue;
				}
				else if (*src < 0x20)
				{
					char_code = '?';
					src += 1u;
				}
				else if (*src == 0x7F)
				{
					char_code = '?';
					src += 1u;
				}
				else
				{
					char_code = (bf_uint16_t)(unsigned char)*src;
					src += 1u;
				}
			}
			else
			{
				if (*(src + 1) != '\0')
				{
					char_code = ((bf_uint16_t)(unsigned char)*src << 8) | (bf_uint16_t)(unsigned char)*(src + 1);
					src += 2u;
				}
				else
				{
					char_code = '?';
					src += 1u;
				}
			}

			bf_get_cp949_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}

	void draw_unicode_string(const uint32_t start_x, const uint32_t start_y, const wchar_t* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = 0u;
		uint32_t y = 0u;

		const wchar_t* src = str;
		const wchar_t* line_start = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			bf_uint16_t char_code;
			if (*src == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\r')
			{
				x = 0u;
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\t')
			{
				x += tab_width();
				src += 1u;
				continue;
			}
			else if (*src == '\b')
			{
				uint32_t bs_width = calc_unicode_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				src += 1u;
				continue;
			}
			else if (*src < 0x20)
			{
				char_code = '?';
				src += 1u;
			}
			else if (*src == 0x7F)
			{
				char_code = '?';
				src += 1u;
			}
			else
			{
				char_code = (bf_uint16_t)*src;
				src += 1u;
			}

			bf_get_unicode_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}

	void draw_utf8l_string(const uint32_t start_x, const uint32_t start_y, const uint8_t* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = 0u;
		uint32_t y = 0u;

		const uint8_t* src = str;
		const uint8_t* line_start = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			uint8_t utf8_char;
			const uint8_t* utf8_char_pointer = NULL;
			uint32_t utf8_char_length = 0u;

			if (*src == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\r')
			{
				x = 0u;
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\t')
			{
				x += tab_width();
				src += 1u;
				continue;
			}
			else if (*src == '\b')
			{
				uint32_t bs_width = calc_utf8l_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				src += 1u;
				continue;
			}
			else if (*src < 0x20u)
			{
				utf8_char = '?';
				src += 1u;

				utf8_char_pointer = &utf8_char;
				utf8_char_length = 1u;
			}
			else if (*src == 0x7Fu)
			{
				utf8_char = '?';
				src += 1u;

				utf8_char_pointer = &utf8_char;
				utf8_char_length = 1u;
			}
			else
			{
				utf8_char_pointer = (uint8_t*)src;
				utf8_char_length = (uint32_t)krc_utf8_char_size((const krc_char_t*)src);

				if (utf8_char_length == 0u)
				{
					utf8_char = '?';
					utf8_char_pointer = &utf8_char;
					utf8_char_length = 1u;

					src += 1u;
				}
				else
				{
					src += utf8_char_length;
				}
			}

			bf_get_utf8l_bitmap(ctx, utf8_char_pointer, utf8_char_length , &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}

	void draw_utf8_string(const uint32_t start_x, const uint32_t start_y, const uint8_t* str)
	{
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = 0u;
		uint32_t y = 0u;

		const uint8_t* src = str;
		const uint8_t* line_start = str;
		bool font_bitmap_drawn = false;

		while (*src)
		{
			uint8_t utf8_char;
			const uint8_t* utf8_char_pointer = NULL;

			if (*src == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\r')
			{
				x = 0u;
				src += 1u;
				line_start = src;
				continue;
			}
			else if (*src == '\t')
			{
				x += tab_width();
				src += 1u;
				continue;
			}
			else if (*src == '\b')
			{
				uint32_t bs_width = calc_utf8_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				src += 1u;
				continue;
			}
			else if (*src < 0x20u)
			{
				utf8_char = '?';
				src += 1u;

				utf8_char_pointer = &utf8_char;
			}
			else if (*src == 0x7Fu)
			{
				utf8_char = '?';
				src += 1u;

				utf8_char_pointer = &utf8_char;
			}
			else
			{
				krc_size_t char_size = krc_utf8_char_size((const krc_char_t*)src);
				if (char_size == 0u)
				{
					utf8_char = '?';
					src += 1u;

					utf8_char_pointer = &utf8_char;
				}
				else
				{
					utf8_char_pointer = (uint8_t*)src;
					src += char_size;
				}
			}

			bf_get_utf8_bitmap(ctx, utf8_char_pointer, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}
};

static void test1()
{
	std::string utf8;
	text_canvas c;


	c.draw_cp949_string(0u, 0u, "A 한글 똠방각하 안녕?\n!ф詰");
	c.output();


	printf("\r\n\r\n");


	c.clear();
	c.draw_unicode_string(10u, 1u, L"유니코드\n안녕?");
	c.output();


	printf("\r\n\r\n");


	c.clear();
	utf8 = mbcs_to_utf8("유니코드\nUTF-8 ABCDEFG", CP_ACP);
	c.draw_utf8_string(1u, 1u, (const uint8_t*)utf8.c_str());
	c.output();


	printf("\r\n\r\n");


	c.clear();
	utf8 = mbcs_to_utf8("유니코드\nUTF-8 가나다", CP_ACP);
	c.draw_utf8l_string(2u, 2u, (const uint8_t*)utf8.c_str());
	c.output();
}

void bf_main()
{
    std::cout << "bf_main()!\n";

    bf_context_default_setup();

	test1();
}
