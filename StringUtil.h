#pragma once
#include <Windows.h>
#include <string>
#include <format>

std::wstring ConvertString(const std::string& str);

std::string ConvertString(const std::wstring& str);

