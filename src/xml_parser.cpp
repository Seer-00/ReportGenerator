#include "xml_parser.h"

extern JSONParser config;
extern JSONParser input_dft;
extern JSONParser input_usr;
extern map<string, string> inputflags_map;

// 定义全局变量
Labels labels; // page2 颜色标示
map<string, string> generated_image_map; // key: 视图名称，value: 视图路径

/* XMLParser */
XMLParser::XMLParser() { }

XMLParser::~XMLParser() { }

void XMLParser::parse_file(const string& filename)
{
	xml_parse_result result = xml.load_file(filename.c_str(), pugi::parse_default, pugi::encoding_auto);
	if (!result) {
		throw XMLParserException("[" + filename + "] parsed with errors:\n" + result.description());
	}
}

/* TemplateParser */
TemplateParser::TemplateParser() { }

TemplateParser::~TemplateParser() { }

TemplateParser* TemplateParser::get_instance()
{
	static TemplateParser instance;
	return &instance;
}

void TemplateParser::init()
{
	// 初始化 switch_map
	if (switch_map.empty()) {
		switch_map.insert(pair<string, int>("beginpage",    _NODE_BEG_PAGE));
		switch_map.insert(pair<string, int>("endpage",      _NODE_END_PAGE));
		switch_map.insert(pair<string, int>("image",        _NODE_IMAGE));
		switch_map.insert(pair<string, int>("table",        _NODE_TABLE));
		switch_map.insert(pair<string, int>("textline",     _NODE_TEXTLINE));
		switch_map.insert(pair<string, int>("textflow",     _NODE_TEXTFLOW));
		switch_map.insert(pair<string, int>("annotation3d", _NODE_ANNOTATION_3D));
	}
}

void TemplateParser::parse_template()
{
	vector<string> enabled_pages;
	for (xml_node page_node : xml.children("page")) {
		string page_id = page_node.child_value("id");
		if (is_enabled(enabled_pages, page_id)) {
			//cout << "#------ Start parsing page " << page_id << endl;
			parse_node_page(page_node);
			//cout << endl;
		}
	}
}

void TemplateParser::parse_node_page(xml_node& page_node)
{
	for (xml_node child : page_node.children()) {
		switch (switch_map[child.name()]) {
		case _NODE_BEG_PAGE: {
			//cout << "parse_node_beg_page" << endl;
			parse_node_beg_page(child);
			break;
		}
		case _NODE_END_PAGE: {
			//cout << "parse_node_end_page" << endl;
			parse_node_end_page(child);
			break;
		}
		case _NODE_IMAGE: {
			//cout << "parse_node_img" << endl;
			parse_node_img(child);
			break;
		}
		case _NODE_TABLE: {
			//cout << "parse_node_tbl" << endl;
			parse_node_tbl(child);
			break;
		}
		case _NODE_TEXTLINE: {
			//cout << "parse_node_tl" << endl;
			parse_node_tl(child);
			break;
		}
		case _NODE_TEXTFLOW: {
			//cout << "parse_node_tf" << endl;
			parse_node_tf(child);
			break;
		}
		case _NODE_ANNOTATION_3D: {
			//cout << "parse_node_3d_annotation" << endl;
			parse_node_3d_annotation(child);
			break;
		}
		default:
			break;
		}
	}
}

void TemplateParser::parse_node_beg_page(xml_node& beg_page_node)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string width  = beg_page_node.child_value("width");
	string height = beg_page_node.child_value("height");
	string opt    = beg_page_node.child_value("optlist");
	pdf->get_pdflib().begin_page_ext(str2double(width), str2double(height), str2wstr(opt));
}

void TemplateParser::parse_node_end_page(xml_node& end_page_node)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string opt = end_page_node.child_value("optlist");
	pdf->get_pdflib().end_page_ext(str2wstr(opt));
}

int TemplateParser::parse_node_img(xml_node& img_node)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string type		= img_node.child_value("type");
	string filename = img_node.child_value("filename");
	string opt		= img_node.child_value("optlist");
	handle_flag(img_node, filename, "filename");

	int img_handle	= pdf->load_image(str2wstr(type), str2wstr(filename), str2wstr(opt));

	xml_node fit_node = img_node.child("fit");
	if (fit_node) {
		string x = fit_node.child_value("x");
		string y = fit_node.child_value("y");
		string o = fit_node.child_value("optlist");
		pdf->get_pdflib().fit_image(img_handle, str2double(x), str2double(y), str2wstr(o));
		pdf->get_pdflib().close_image(img_handle);
		return -1;
	}
	else { 
		return img_handle; // 不直接填充，仅仅加载image资源，并返回句柄。例如，用于填充表格单元格。
	}
}

void TemplateParser::parse_node_tbl(xml_node& tbl_node)
{
	int tbl = -1;
	PDFGenerator* pdf = PDFGenerator::get_instance();
	for (xml_node child : tbl_node.children("cell")) {
		parse_node_tbl_cell(child, tbl);
	}
	xml_node fit_node = tbl_node.child("fit");
	if (fit_node) {
		string llx = fit_node.child_value("llx");
		string lly = fit_node.child_value("lly");
		string urx = fit_node.child_value("urx");
		string ury = fit_node.child_value("ury");
		string opt = fit_node.child_value("optlist");
		pdf->fit_table(tbl, str2double(llx), str2double(lly), str2double(urx), str2double(ury), str2wstr(opt));
	}
	// delete_table 的参数2 (即optlist) 只有一个可选项 keephandles，默认为false
	// 即删除 table 时，也会一并删除 image, textflow 等句柄
	// 鉴于本课题的需求，在这里直接令 optlist 为 L"" 即可
	pdf->get_pdflib().delete_table(tbl, L"");
}

void TemplateParser::parse_node_tbl_cell(xml_node& tbl_cell_node, int& tbl)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string col  = tbl_cell_node.child_value("col");
	string row  = tbl_cell_node.child_value("row");
	string text = tbl_cell_node.child_value("text");
	string opt  = tbl_cell_node.child_value("optlist");
	string type = tbl_cell_node.child_value("celltype");
	handle_flag(tbl_cell_node, text, "text");
	// 向单元格嵌入image
	if (type == "image") {
		xml_node img_node = tbl_cell_node.child("image");
		int img = parse_node_img(img_node);
		opt += (" image=" + int2str(img));
	}
	// 向单元格嵌入textflow
	else if (type == "textflow") {
		xml_node tf_node = tbl_cell_node.child("textflow");
		int tf = parse_node_tf(tf_node);
		opt += (" textflow=" + int2str(tf));
	}
	tbl = pdf->add_table_cell(tbl, str2int(col), str2int(row), str2wstr(text), str2wstr(opt));
}

void TemplateParser::parse_node_tl(xml_node& tl_node)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string text = tl_node.child_value("text");
	string x	= tl_node.child_value("x");
	string y	= tl_node.child_value("y");
	string opt	= tl_node.child_value("optlist");
	handle_flag(tl_node, text, "text");
	pdf->get_pdflib().fit_textline(str2wstr(text), str2double(x), str2double(y), str2wstr(opt));
}

int TemplateParser::parse_node_tf(xml_node& tf_node)
{
	int tf = -1;
	PDFGenerator* pdf = PDFGenerator::get_instance();
	for (xml_node child : tf_node.children("item")) {
		parse_node_tf_item(child, tf);
	}
	xml_node fit_node = tf_node.child("fit");
	if (fit_node) {
		string llx = fit_node.child_value("llx");
		string lly = fit_node.child_value("lly");
		string urx = fit_node.child_value("urx");
		string ury = fit_node.child_value("ury");
		string opt = fit_node.child_value("optlist");
		pdf->fit_textflow(tf, str2double(llx), str2double(lly), str2double(urx), str2double(ury), str2wstr(opt));
		pdf->get_pdflib().delete_textflow(tf);
		return -1;
	}
	else {
		return tf; // 不直接填充，仅仅创建 textflow 资源，并返回句柄。例如，用于填充表格单元格。
	}
}

void TemplateParser::parse_node_tf_item(xml_node& tf_item_node, int& tf)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string text = tf_item_node.child_value("text");
	string opt  = tf_item_node.child_value("optlist");
	handle_flag(tf_item_node, text, "text");
	tf = pdf->add_textflow(tf, str2wstr(text), str2wstr(opt));
}

int TemplateParser::parse_node_3d_view(xml_node& view_3d_node)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string usr	  = view_3d_node.child_value("username");
	string camera = view_3d_node.child_value("camera2world");
	string opt	  = view_3d_node.child_value("optlist");
	handle_flag(view_3d_node, camera, "camera2world");
	opt.append(" camera2world=").append(camera); // 将相机信息拼接到选项列表
	int view = pdf->create_3dview(str2wstr(usr), str2wstr(opt));
	return view;
}

int TemplateParser::parse_node_3d_data(xml_node& data_3d_node, vector<int>& views)
{
	PDFGenerator* pdf = PDFGenerator::get_instance();
	string file = data_3d_node.child_value("file");
	string opt  = data_3d_node.child_value("optlist");
	handle_flag(data_3d_node, file, "file");
	opt.append(" views={"); 
	for (int v : views) { // 支持多个view，最后多一个空格不影响
		opt.append(int2str(v)).append(" ");
	}
	opt.append("}");
	int data = pdf->load_3ddata(str2wstr(file), str2wstr(opt));
	return data;
}

void TemplateParser::parse_node_3d_annotation(xml_node& annotation_3d_node)
{
	vector<int> views;
	for (xml_node view_3d_node : annotation_3d_node.children("view3d")) {
		views.push_back(parse_node_3d_view(view_3d_node));
	}

	xml_node data_3d_node = annotation_3d_node.child("data3d");
	int data = parse_node_3d_data(data_3d_node, views);

	PDFGenerator* pdf = PDFGenerator::get_instance();
	string llx  = annotation_3d_node.child_value("llx");
	string lly  = annotation_3d_node.child_value("lly");
	string urx  = annotation_3d_node.child_value("urx");
	string ury  = annotation_3d_node.child_value("ury");
	string type = annotation_3d_node.child_value("type");
	string opt  = annotation_3d_node.child_value("optlist");
	// 这里默认第1个 view 作为 3Dinitialview
	opt.append(" 3Ddata=").append(int2str(data)).append(" 3Dinitialview=").append(int2str(views[0]));
	pdf->get_pdflib().create_annotation(
		str2double(llx), str2double(lly), str2double(urx), str2double(ury), str2wstr(type), str2wstr(opt)
	);
}

void TemplateParser::handle_flag(xml_node& _node, string& field, const char* _childname)
{
	/* 处理 inputflag, generateflag，将最终结果记录在 field 中                 */
	/* [!] 按当前的设计：一个 node 中最多只含一个 flag，只会走下面三个分支之一 */
	string inputflag    = _node.child_value("inputflag");
	string generateflag = _node.child_value("generateflag");
	if (inputflag.empty() && generateflag.empty()) {
		// flag 都空，说明该内容由模板决定，直接读取模板内容，并处理转义字符即可
		// [!] 模板是 gb2312，无需转换编码
		// e.g. val: "3D 建模可视化报告"
		string val = _node.child_value(_childname);
		field = handle_escapechar(_node, val);
	}
	if (!inputflag.empty())    { field = handle_inputflag(_node, _childname); }
	if (!generateflag.empty()) { field = handle_generateflag(_node, _childname); }
}

string TemplateParser::handle_inputflag(xml_node& _node, const char* _childname)
{
	/* inputflag 不为空，说明该内容由用户决定，   */
	/* 返回 普通内容、默认内容、生成内容 其中之一 */
	// [!] 用户输入是 utf8，需转换编码
	// e.g.
	//   flag:	"p-1-img-1"
	//   page:	"page1" (i.e. "page" + "1")
	//   key:	"logo"
	//   val:	"page_1_img_1_demo.png"

	string flag = _node.child_value("inputflag");

	Value root  = input_usr.get_root();
	string page = "page" + flag.substr(2, 1);
	string key  = inputflags_map[flag];
	string val  = root[page][key].asString();

	// 【默认内容】读取 input_default.json 中的内容
	if (val == DEFAULT) {
		Value dft = input_dft.get_root();
		val = dft[page][key].asString();
	}
	// [!] 这里不能用 else-if，
	// 因为可能在 input_default.json 中得到 val 为 __GENERATE__

	// 【生成内容】调用 相应函数 完成内容的生成
	if (val == GENERATE) {
		// 程序内判断 key 与给定字符串是否相等前，需将 key 从 utf8 转成 str
		key = utf8_to_str(key);
		if (key == "年" || key == "月" || key == "日" || key == "建模日期") {
			val = get_date(key);
			return val; // 无需转换 utf8_to_str
		}
		if (key == "概览图" || key == "上面观" || key == "下面观" || key == "正面观" 
			|| key == "背面观" || key == "左侧面观" || key == "右侧面观") {
			// 同 STLParser::capture_views() 中生成 PNG 图像的路径
			val = generated_image_map[key];
			return val; // 无需转换 utf8_to_str
		}
		if (key == "数据") {
			Value cfg = config.get_root();
			const string out = utf8_to_str(cfg["output_dir"].asString());
			val = U3D_MODEL_PATH(out);
			return val; // 如果 U3D_MODEL_PATH(out) 仅含英文字符，则无需转换 utf8_to_str
		}
		val = GENERATE; // [!] 不应该走到这里
	}
	// 【普通内容】无需进一步处理 val

	val = handle_escapechar(_node, val);
	val = utf8_to_str(val);

	return val;
}

string TemplateParser::handle_generateflag(xml_node& _node, const char* _childname)
{
	/* generateflag 不为空，说明该内容必须由程序生成 */
	// [!] 相关数据流只存于程序内部，不涉及外部输入，无需编码转换
	// e.g. label-img
	//   flag	: "p-2-label-img-5"
	//   i		: 13 (i.e. flag[i] == '-')
	//   map_idx: 5

	string flag = _node.child_value("generateflag");
	string val;
	if (flag.find("label-img") != string::npos) { // page2 颜色标示 图片列
		int i       = flag.find_last_of('-');
		int map_idx = str2int(flag.substr(i + 1));
		string name = labels.name_map[map_idx];
		if (name.empty()) { 
			// name_map 返回 空串时，表明该单元格不对应任何模型，使用预生成的白色标签图片
			name = LABEL_IMAGE_WHITE;
		}
		Value cfg = config.get_root();
		const string out = utf8_to_str(cfg["output_dir"].asString());
		val = LABEL_IMAGE_PATH(out, name);
	}
	if (flag.find("label-text") != string::npos) { // page2 颜色标示 名称列
		int i       = flag.find_last_of('-');
		int map_idx = str2int(flag.substr(i + 1));
		val = labels.name_map[map_idx]; // 返回空串时，向单元格填入空串即可
	}
	// if ...

	return val;
}

string TemplateParser::handle_escapechar(xml_node& _node, string text)
{
	/* 从 xml_node 获取转义字符，并处理文本 */
	for (xml_node escp_node : _node.children("escapechar")) {
		string escp = escp_node.text().as_string();
		// 换行
		if (escp == "\\n") {
			text = replace_all(text, "\\n", "\n");
		}
		// 制表符
		else if (escp == "\\t") {
			text = replace_all(text, "\\t", "\t");
		}
		// 中文空格
		else if (escp == "\\u3000") {
			text = replace_all(text, "\\u3000", "\u3000");
		}
		// else if ...
	}
	return text;
}

bool TemplateParser::is_enabled(vector<string>& pages, const string& id)
{
	if (pages.empty()) { // 只会加载一次
		Value enpages = config.get_root()["enabled_pages"];
		for (unsigned int i = 0; i < enpages.size(); i++) {
			pages.push_back(enpages[i].asString());
		}
	}
	vector<string>::iterator iter;
	iter = find(pages.begin(), pages.end(), id);
	return iter != pages.end(); // iter 没有遍历到 end，说明 enabled_pages 中存在
}

/* ModelInfoParser */
ModelInfoParser::ModelInfoParser() { }

ModelInfoParser::~ModelInfoParser() { }

ModelInfoParser* ModelInfoParser::get_instance()
{
	static ModelInfoParser instance;
	return &instance;
}

xml_node ModelInfoParser::get_model_node(const string& name, const string& type)
{
	xml_node model_list = xml.child("Project").child("NodeList");
	for (xml_node model_node : model_list.children()) {
		string filename = model_node.child("Data").child("FileName").text().as_string();
		string filetype = model_node.child("Data").child("FileType").text().as_string();
		if (filename == name && filetype == type) { // 可认为找到了相关模型的信息
			return model_node;
		}
	}
	throw XMLParserException("The information of model [" + name + "." + type + "] not found in model_info.xml");
}

string ModelInfoParser::get_organ_name(xml_node& model_node)
{
	xml_node property_list = model_node.child("PropertyList");
	for (xml_node prop_node : property_list.children()) {
		// "organ_name" 要转成 utf8 才能比较
		if (prop_node.attribute("Name").as_string() == str_to_utf8("organ_name")) {
			return prop_node.text().as_string();
		}
	}
	// 当 model_info.xml 没有相应的信息时，可根据需求：抛异常 或 返回默认值
	// throw XMLParserException("The property \"organ_name\" not found in model_info.xml");
	return "ORGAN_NAME"; 
}

double ModelInfoParser::get_opacity(xml_node& model_node)
{
	xml_node property_list = model_node.child("PropertyList");
	for (xml_node prop_node : property_list.children()) {
		if (prop_node.attribute("Name").as_string() == str_to_utf8("opacity")) {
			return prop_node.text().as_double();
		}
	}
	// throw XMLParserException("The property \"opacity\" not found in model_info.xml");
	return 1.0;
}

bool ModelInfoParser::get_visible(xml_node& model_node)
{
	xml_node property_list = model_node.child("PropertyList");
	for (xml_node prop_node : property_list.children()) {
		if (prop_node.attribute("Name").as_string() == str_to_utf8("visible")) {
			return prop_node.text().as_bool();
		}
	}
	// throw XMLParserException("The property \"visible\" not found in model_info.xml");
	return false;
}

void ModelInfoParser::get_color(xml_node& model_node, double* color, int size)
{
	xml_node property_list = model_node.child("PropertyList");
	for (xml_node prop_node : property_list.children()) {
		if (prop_node.attribute("Name").as_string() == str_to_utf8("color")) {
			string color_str = prop_node.text().as_string();
			vector<string> color_list = split_string(color_str, ',');
			if (color_list.size() != size) {
				// throw XMLParserException("The property \"color\" is incorrect in model_info.xml");
				break;
			}
			for (int i = 0; i < size; i++) {
				color[i] = str2double(color_list[i]);
			}
			return;
		}
	}
	// throw XMLParserException("The property \"color\" not found in model_info.xml");
	color[0] = color[1] = color[2] = 0.0;
}
