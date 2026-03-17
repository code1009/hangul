/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <Windows.h>

#include "../lib/bf/bf.h"
#include "../lib/krc/krc.h"

#include <iostream>
#include <vector>

#include "windows_charset.hpp"





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
				if (font_bitmap->font_bitmap_pointer[y * font_bitmap->font_bitmap_stride + x / 8u] & mask[x % 8u])
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
			uint32_t char_length;
			uint16_t char_code;

			if ((unsigned char)*p < 0x80u)
			{
				char_length = 1u;
				char_code = (uint16_t)(unsigned char)*p;
			}
			else if (p > str && (unsigned char)*(p - 1) >= 0x80u)
			{
				char_length = 2u;
				char_code = ((uint16_t)(unsigned char)*(p - 1) << 8) | (uint16_t)(unsigned char)*p;
			}
			else
			{
				if (depth == 0) break;
				depth--;
				p--;
				continue;
			}

			p -= (int)char_length;

			if (char_code == '\b')
			{
				depth++;
			}
			else if (char_code == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
			}
			else
			{
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_cp949_bitmap(ctx, char_code, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
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
			size_t char_length;
			uint16_t char_code;

			if (*(src + 1) == '\0')
			{
				char_length = 1u;
			}
			else
			{
				if((unsigned char)*src < 0x80u)
				{
					char_length = 1u;
				}
				else
				{
					char_code = (((unsigned char)*src) << 8) | (unsigned char)*(src + 1);
					if (KRC_CODE_RANGE_UNKNOWN == krc_code_range_cp949(char_code))
					{
						char_length = 1u;
					}
					else
					{
						char_length = 2u;
					}
				}
			}
			if (1u == char_length)
			{
				char_code = (unsigned char)*src;
				src += 1u;
			}
			else
			{
				char_code = (((unsigned char)*src) << 8) | (unsigned char)*(src + 1);
				src += 2u;
			}

		
			if (char_code == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				line_start = src;
				continue;
			}
			else if (char_code == '\r')
			{
				x = 0u;
				line_start = src;
				continue;
			}
			else if (char_code == '\t')
			{
				x += tab_width();
				continue;
			}
			else if (char_code == '\b')
			{
				uint32_t bs_width = calc_cp949_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				continue;
			}


			bf_get_cp949_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}

	uint32_t calc_unicode_backspace_width(bf_context_t* ctx, const wchar_t* str, const wchar_t* pos)
	{
		const wchar_t* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			const uint32_t char_length = 1u;
			const bf_uint16_t char_code = (bf_uint16_t)*p;
			p -= (int)char_length;

			if (char_code == '\b')
			{
				depth++;
			}
			else if (char_code == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
			}
			else
			{
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_unicode_bitmap(ctx, char_code, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
			}
		}
		return bs_width;
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
			bf_uint16_t char_code = (bf_uint16_t)*src;
			src += 1u;

			if (char_code == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				line_start = src;
				continue;
			}
			else if (char_code == '\r')
			{
				x = 0u;
				line_start = src;
				continue;
			}
			else if (char_code == '\t')
			{
				x += tab_width();
				continue;
			}
			else if (char_code == '\b')
			{
				uint32_t bs_width = calc_unicode_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				continue;
			}

			bf_get_unicode_bitmap(ctx, char_code, &font_bitmap);
			draw_font_bitmap(start_x + x, start_y + y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;

			font_bitmap_drawn = true;
		}
	}

	uint32_t calc_utf8_backspace_width(bf_context_t* ctx, const uint8_t* str, const uint8_t* pos)
	{
		const uint8_t* p = pos - 1;
		int depth = 0;
		uint32_t bs_width = 8u;
		while (p >= str)
		{
			const uint8_t* char_start = p;
			while (char_start > str && (*char_start & 0xC0u) == 0x80u)
				char_start--;

			uint32_t char_code;
			uint32_t char_length = (uint32_t)krc_utf8_to_unicode_char((const krc_char_t*)char_start, (krc_uchar32_t*)&char_code);
			if (char_length == 0u) { char_code = (uint32_t)'?'; char_length = 1u; }
			p = char_start - 1;

			if (char_code == '\b')
			{
				depth++;
			}
			else if (char_code == '\t')
			{
				if (depth == 0) { bs_width = tab_width(); break; }
				depth--;
			}
			else
			{
				if (depth == 0)
				{
					bf_font_bitmap_t prev_bm;
					bf_get_unicode_bitmap(ctx, char_code, &prev_bm);
					bs_width = prev_bm.font_bitmap_cx;
					break;
				}
				depth--;
			}
		}
		return bs_width;
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
			uint32_t char_length;
			uint32_t char_code;
			
			char_length = (uint32_t)krc_utf8_to_unicode_char((const krc_char_t*)src, (krc_uchar32_t*)&char_code);
			if (0u == char_length)
			{
				char_code = '?';
				src += 1u;
			}
			else
			{
				src += char_length;
			}

			if (char_code == '\n')
			{
				x = 0u;
				y += (font_bitmap_drawn ? font_bitmap.font_bitmap_cy : 16u);
				line_start = src;
				continue;
			}
			else if (char_code == '\r')
			{
				x = 0u;
				line_start = src;
				continue;
			}
			else if (char_code == '\t')
			{
				x += tab_width();
				continue;
			}
			else if (char_code == '\b')
			{
				uint32_t bs_width = calc_utf8_backspace_width(ctx, line_start, src);
				x = (x < bs_width) ? 0u : (x - bs_width);
				continue;
			}

			bf_get_unicode_bitmap(ctx, char_code, &font_bitmap);
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
	c.draw_utf8_string(2u, 2u, (const uint8_t*)utf8.c_str());
	c.output();
}

void bf_main()
{
    std::cout << "bf_main()!\n";

    bf_context_default_setup();

	test1();
}
