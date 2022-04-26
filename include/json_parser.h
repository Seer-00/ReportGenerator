#pragma once
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <map>
#include <fstream>

#include "jsoncpp/json.h"

#include "program_exception.h"

using namespace std;
using namespace Json;

class JSONParser
{
public:
	JSONParser();
	JSONParser(const string& filename);
	~JSONParser();

	Value& get_root();

	void parse_file(const string& filename);

private:
	ifstream ifs;
	Reader reader;
	Value root;
};

#endif // !JSON_PARSER_H


