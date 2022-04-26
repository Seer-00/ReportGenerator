#include "json_parser.h"

// ����ȫ�ֱ���
JSONParser config("./config.json"); // �����ļ�:     config.json
JSONParser input_dft;				// Ĭ�������ļ�: input_default.json
JSONParser input_usr;				// �û������ļ�: input_user.json
map<string, string> inputflags_map; // ��Ӧת���ļ�: inputflags.json

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
