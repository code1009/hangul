#pragma once




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring mbcs_to_wcs(std::string input, UINT codepage);
std::string wcs_to_mbcs(std::wstring input, UINT codepage);
std::string utf8_to_mbcs(std::string utf8, UINT codepage);
std::string mbcs_to_utf8(std::string mbcs, UINT codepage);
std::string wcs_to_utf8(std::wstring input);
std::wstring utf8_to_wcs(std::string input);
