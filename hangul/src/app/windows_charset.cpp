/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <Windows.h>

#include <string>
#include <vector>

#include "windows_charset.hpp"



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
