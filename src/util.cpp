#include "util.h"

std::string str_to_utf8(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string str_utf8(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return str_utf8;
}

std::string utf8_to_str(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

std::string wstr2str(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

std::string int2str(const int& i)
{
	return std::to_string(i);
}

std::wstring int2wstr(const int& i)
{
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring str2wstr(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

int str2int(const std::string& s)
{
	return std::stoi(s);
}

double str2double(const std::string& s)
{
	return std::stod(s);
}

std::string get_date(const std::string& key)
{
	std::stringstream ss;

	// 获得当前系统的日期/时间
	time_t now = time(0);
	tm* tm = localtime(&now);

	if		(key == "年") ss << (1900 + tm->tm_year);
	else if (key == "月") ss << (1 + tm->tm_mon);
	else if (key == "日") ss << tm->tm_mday;
	else if (key == "建模日期") {
		std::string year  = get_date("年");
		std::string month = get_date("月");
		std::string day   = get_date("日");
		ss << year;
		ss << (month.length() < 2 ? ("0" + month) : month);
		ss << (day.length()   < 2 ? ("0" + day)   : day);
		// e.g 20220101
	}
	else { }
	
	return ss.str();
}

std::string replace_all(std::string& str, const std::string& _old, const std::string& _new)
{
	// O(n) 将str中所有_old替换为_new
	std::string ret = str;
	std::string::size_type pos = 0;
	while ((pos = ret.find(_old)) != std::string::npos) {
		ret.replace(pos, _old.length(), _new);
	}
	return ret;
}

std::vector<std::string> split_string(const std::string& str, const char sep)
{
	std::size_t previous = 0;
	std::size_t current = str.find(sep);
	std::vector<std::string> ret;
	while (current != std::string::npos) {
		if (current > previous) {
			ret.push_back(str.substr(previous, current - previous));
		}
		previous = current + 1;
		current = str.find(sep, previous);
	}
	if (previous != str.size()) {
		ret.push_back(str.substr(previous));
	}
	return ret;
}
