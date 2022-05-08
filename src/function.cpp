#include "function.h"

extern JSONParser config;
extern JSONParser input_dft;
extern JSONParser input_usr;
extern map<string, string> inputflags_map;

void init()
{
	/* 读配置文件，初始化各模块，config.json 可能含中文，注意转码 */

	// 解析配置文件     config.json
	config.parse_file("./config.json");

	Value cfg = config.get_root();
	// 解析用户输入文件 input_user.json
	const string res_input_user = utf8_to_str(cfg["res_input_user"].asString());
	input_usr.parse_file(res_input_user);

	// 解析默认输入文件 input_default.json
	const string res_input_dft  = utf8_to_str(cfg["res_input_dft"].asString());
	input_dft.parse_file(res_input_dft);

	// 解析模型信息文件 model_info.xml
	const string res_model_info = utf8_to_str(cfg["res_model_info"].asString());
	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	p_model_info->parse_file(res_model_info);

	// 解析默认模板文件 template_default.xml
	const string res_template_default = utf8_to_str(cfg["res_template_default"].asString());
	TemplateParser* p_template_parser = TemplateParser::get_instance();
	p_template_parser->parse_file(res_template_default); 
	p_template_parser->init();

	// 解析对应规则文件 inputflags.json
	const string res_inputflags = utf8_to_str(cfg["res_inputflags"].asString());
	JSONParser parser(res_inputflags);
	Value root = parser.get_root();
	for (string key : root.getMemberNames()) {
		// 保持utf8，无需转换
		// 因为 “经过 inputflags_map 进行对应” 的全过程都发生在程序内部，不涉及转码
		inputflags_map.insert(pair<string, string>(key, root[key].asString()));
	}

	/* 初始化待生成的 PDF 文件的各项属性，即 optlist */
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
			for (unsigned int i = 0; i < opt[key].size(); i++) {
				wval = str2wstr(utf8_to_str(opt[key][i].asString()));
				optlist.append(wval).append(L" ");
			}
			optlist.append(L"} ");
			// e.g. optlist: "fontoutline={calibri=calibri.ttf msyh=MicrosoftYaHei.ttf} "
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

void generate_report()
{
	//// 先生成 视图 和 渲染后的3D模型
	//generate_views(); // .png
	//generate_PLY();   // .ply

	//// 不同的方式生成 u3d
	//bool auto_to_u3d = true;
	//if (auto_to_u3d) { 
	//	convert_PLY_TO_U3D(); 
	//}
	//else {
	//	cout << "等待手动转换得到 U3D 文件 ..." << endl;
	//	system("pause");
	//}
	//
	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	TemplateParser* p_template_parser = TemplateParser::get_instance();

	Value cfg = config.get_root();
	const string output_dir  = utf8_to_str(cfg["output_dir"].asString());
	const string report_name = utf8_to_str(cfg["report_name"].asString());
	const string report_path = output_dir + "/" + report_name;
	const wstring w_report_path = str2wstr(report_path);
	//if (pdf.pdflib().begin_document(output_file, L"compatibility=1.7ext3") == -1) {
	if (p_pdfg->get_pdflib().begin_document(w_report_path, L"") == -1) {
		throw CreateFileException(w_report_path, p_pdfg->get_pdflib().get_errmsg());
	}

	p_template_parser->parse_template();

	p_pdfg->get_pdflib().end_document(L"");
}

void show_models()
{
	STLParser* p_stlp = STLParser::get_instance();
	p_stlp->load_stl_files();
	p_stlp->show_models();
}

void generate_views()
{
	STLParser* p_stlp = STLParser::get_instance();
	p_stlp->load_stl_files();
	p_stlp->capture_views();
}

void generate_PLY()
{
	STLParser* p_stlp = STLParser::get_instance();
	p_stlp->load_stl_files();
	p_stlp->write_ply();
}

void convert_PLY_TO_U3D()
{
	bool new_window = true;
	DWORD creation_flag = new_window ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	bool create_process = CreateProcess(
		PLY_TO_U3D_PATH, // 负责转换 .ply 到 .u3d 的 bat 脚本路径
		NULL,
		NULL,
		NULL,
		FALSE,
		creation_flag,
		NULL,
		NULL,
		&si, &pi
	);
	if (create_process) {
		WaitForSingleObject(pi.hProcess, INFINITE); // 等待bat执行结束
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		throw Exception("Errors happended in converting ply to u3d.");
	}
}
