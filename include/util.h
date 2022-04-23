#pragma once
#pragma warning(disable:4996) // util.cpp: localtime
#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

std::string str_to_utf8(const std::string& str);

std::string wstr_to_str(const std::wstring& ws);

std::wstring int_to_wstr(const int& i);

std::wstring str_to_wstr(const std::string& s);

void fill_today_date(std::vector<std::wstring>& ymd);

#endif /* UTIL_H */
