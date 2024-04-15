#pragma once
#include <string>
#include <format>

class Logger
{
public:
	// string -> wstring
	static std::wstring ConvertString(const std::string& str);
	// wstring -> string
	static std::string ConvertString(const std::wstring& str);
};

