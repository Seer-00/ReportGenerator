#include "function.h"

extern JSONParser config;
extern JSONParser input_dft;
extern JSONParser input_usr;
extern map<string, string> inputflags_map;

void init()
{
	/* 读配置文件，初始化各模块 */
	/* config.json 可能含中文，注意转码 */
	Value cfg = config.get_root();
	const string res_input_user = utf8_to_str(cfg["res_input_user"].asString());
	const string res_input_dft = utf8_to_str(cfg["res_input_dft"].asString());
	input_usr.parse_file(res_input_user); // 解析用户输入文件 input_user.json
	input_dft.parse_file(res_input_dft);  // 解析默认输入文件 input_default.json

	const string res_inputflags = utf8_to_str(cfg["res_inputflags"].asString());
	JSONParser parser(res_inputflags);
	Value root = parser.get_root();
	for (string key : root.getMemberNames()) {
		// 保持utf8，无需转换，因为后续要作为 input_demo.json 的 key
		// inputflags_map.insert(pair<string, string>(key, utf8_to_str(root[key].asString())));
		inputflags_map.insert(pair<string, string>(key, root[key].asString()));
	}

	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	wstring wkey, wval;

	Value opt = cfg["optlist"];
	wstring optlist = L"";
	for (string key : opt.getMemberNames()) {
		wkey = str2wstr(utf8_to_str(key));
		if (opt[key].isString()) {
			wval = str2wstr(utf8_to_str(opt[key].asString()));
			optlist.append(wkey).append(L"=").append(wval).append(L" ");
			// e.g. optlist: "errorpolicy=return "
		}
		else if (opt[key].isArray()) {
			optlist.append(wkey).append(L"={");
			for (int i = 0; i < opt[key].size(); i++) {
				wval = str2wstr(utf8_to_str(opt[key][i].asString()));
				optlist.append(wval).append(L" ");
			}
			optlist.append(L"} ");
			// e.g. optlist: "FontOutline={calibri=calibri.ttf msyh=MicrosoftYaHei.ttf} "
		}
	}
	p_pdfg->get_pdflib().set_option(optlist);

	Value info = cfg["file_info"];
	for (string key : info.getMemberNames()) {
		wkey = str2wstr(utf8_to_str(key));
		wval = str2wstr(utf8_to_str(info[key].asString()));
		// e.g. wkey: "Title", wval: "3D 建模可视化报告"
		p_pdfg->get_pdflib().set_info(wkey, wval);
	}
}

void parse_template()
{
	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	XMLParser* p_xmlp = XMLParser::get_instance();

	Value cfg = config.get_root();
	const wstring output_file = str2wstr(utf8_to_str(cfg["output_file"].asString()));

	//if (pdf.pdflib().begin_document(output_file, L"compatibility=1.7ext3") == -1) {
	if (p_pdfg->get_pdflib().begin_document(output_file, L"") == -1) {
		throw CreateFileException(output_file, p_pdfg->get_pdflib().get_errmsg());
	}

	p_xmlp->parse_xml_document();

	p_pdfg->get_pdflib().end_document(L"");
}
