/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <vector>

#include "../lib/bf/bf.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class text_bitmap
{
private:
	uint32_t _cx;
	uint32_t _cy;
	std::vector<std::string> _bitmap_buffer;

public:
	text_bitmap() :
		_cx(128u), _cy(16u)
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
		_bitmap_buffer[y][x] = pixel ? '*' : '.';
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

	void draw_string(const uint32_t start_x, const uint32_t start_y, const char* str)
	{
		// 기본 컨텍스트 가져오기
		bf_context_t* ctx = bf_context_default_get();
		bf_font_bitmap_t font_bitmap;

		uint32_t x = start_x;
		uint32_t y = start_y;

		const char* src = str;
		while (*src)
		{
			bf_uint16_t char_code;

			if ((unsigned char)*src < 0x80)
			{
				char_code = (bf_uint16_t)(unsigned char)*src;
				src += 1;
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
			draw_font_bitmap(x, y, &font_bitmap);
			x += font_bitmap.font_bitmap_cx;
		}
	}
};

static void test1()
{
	text_bitmap tb;
	tb.draw_string(1u, 0u, "A한글!ф詰");
	tb.output();
}

void bf_main()
{
    std::cout << "bf_main()!\n";

    bf_context_default_init();

	test1();
}
