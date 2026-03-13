/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "bf_config.h"
#include "bf_type.h"

//===========================================================================
#include "bf_font/bf_font_data_16x16_unknown.h"

#if (1==BF_CONFIG_FONT_DATA_ASCII_LOW)
#include "bf_font/bf_font_data_08x16_ascii_low.h"
#endif

#if (1==BF_CONFIG_FONT_DATA_HANGUL)
#include "bf_font/bf_font_data_16x16_hangul_johab844.h"
#endif

#if (1==BF_CONFIG_FONT_DATA_SPECIAL)
#include "bf_font/bf_font_data_16x16_cp949_special_1128.h"
#endif

#if (1==BF_CONFIG_FONT_DATA_HANJA)
#include "bf_font/bf_font_data_16x16_cp949_hanja_4888.h"
#endif




