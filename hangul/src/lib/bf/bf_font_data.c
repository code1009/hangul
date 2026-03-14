/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define BF_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "bf_config.h"
#include "bf_type.h"

//===========================================================================
#include "bf_font/bf_font_data_16x16_unknown.h"

#include "bf_font/bf_font_data_08x16_ascii_low.h"

#include "bf_font/bf_font_data_16x16_hangul_johab844.h"

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL)
#include "bf_font/bf_font_data_16x16_cp949_special_1128.h"
#endif

#if (1==BF_CONFIG_SUPPORT_FONT_CP949_HANJA)
#include "bf_font/bf_font_data_16x16_cp949_hanja_4888.h"
#endif




