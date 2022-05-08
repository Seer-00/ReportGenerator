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

std::string utf8_to_str(const std::string& str);

std::string wstr2str(const std::wstring& ws);

std::string int2str(const int& i);

std::wstring int2wstr(const int& i);

std::wstring str2wstr(const std::string& s);

int str2int(const std::string& s);

double str2double(const std::string& s);

std::string get_date(const std::string& key);

std::string replace_all(std::string& str, const std::string& _old, const std::string& _new);

std::vector<std::string> split_string(const std::string& str, const char sep);

#endif /* UTIL_H */
