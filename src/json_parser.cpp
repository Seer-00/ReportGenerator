#include "json_parser.h"

// 定义全局变量
JSONParser config("./config.json"); // 配置文件:     config.json
JSONParser input_dft;				// 默认输入文件: input_default.json
JSONParser input_usr;				// 用户输入文件: input_user.json
map<string, string> inputflags_map; // 对应转换文件: inputflags.json

JSONParser::JSONParser()
{
}

JSONParser::JSONParser(const string& filename)
{
	parse_file(filename);
}

JSONParser::~JSONParser()
{
	if (ifs) {
		ifs.close();
	}
}

Value& JSONParser::get_root()
{
	return root;
}

void JSONParser::parse_file(const string& filename)
{
	ifs.open(filename);
	if (!ifs) {
		throw JSONParserException("Open file [" + filename + "] failed.");
		ifs.close();
	}
	if (!reader.parse(ifs, root)) {
		throw JSONParserException("Parse file [" + filename + "] failed.");
		ifs.close();
	}
}
