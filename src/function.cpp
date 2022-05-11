#include "function.h"

extern JSONParser config;
extern JSONParser input_dft;
extern JSONParser input_usr;
extern map<string, string> inputflags_map;

void load_config(string config_path)
{
	/* 读配置文件，初始化各模块，config.json 可能含中文，注意转码 */

	// 解析配置文件     config.json
	config.parse_file(config_path);

	Value cfg = config.get_root();
	// 解析用户输入文件 input_user.json
	const string res_input_user = utf8_to_str(cfg["res_input_user"].asString());
	input_usr.parse_file(res_input_user);

	// 解析默认输入文件 input_default.json
	const string res_input_dft  = utf8_to_str(cfg["res_input_dft"].asString());
	input_dft.parse_file(res_input_dft);

	// 解析模型信息文件 model_info.xml
	const string res_model_info   = utf8_to_str(cfg["res_model_info"].asString());
	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	p_model_info->parse_file(res_model_info);

	// 解析报告模板文件 report_template.xml
	const string res_report_template  = utf8_to_str(cfg["res_report_template"].asString());
	TemplateParser* p_template_parser = TemplateParser::get_instance();
	p_template_parser->parse_file(res_report_template); 
	p_template_parser->init();

	// 解析对应规则文件 inputflags.json
	const string res_inputflags = utf8_to_str(cfg["res_inputflags"].asString());
	JSONParser parser(res_inputflags);
	Value root = parser.get_root();
	// 可能先后多次导入配置文件，需清空
	inputflags_map.clear(); 
	for (string key : root.getMemberNames()) {
		// 保持utf8，无需转换
		// 因为 “经过 inputflags_map 进行对应” 的全过程都发生在程序内部，不涉及转码
		inputflags_map.insert(pair<string, string>(key, root[key].asString()));
	}

	/* 初始化待生成的 PDF 文件的各项属性，即 optlist */
	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	p_pdfg->load_pdflib_config();
}

void generate_report()
{
	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	TemplateParser* p_template_parser = TemplateParser::get_instance();

	Value cfg = config.get_root();
	const string output_dir  = utf8_to_str(cfg["output_dir"].asString());
	const string report_name = utf8_to_str(cfg["report_name"].asString());
	const string report_path = output_dir + "/" + report_name;
	const wstring w_report_path = str2wstr(report_path);

	p_pdfg->renew_pdflib();

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
		//CREATE_NO_WINDOW, // 不创建新的命令行窗口
		CREATE_NEW_CONSOLE, // 创建新的命令行窗口，以观察bat脚本执行情况
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
		throw Exception("Could not create process when automatically converting ply to u3d.");
	}
}
