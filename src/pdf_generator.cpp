#include "pdf_generator.h"

extern JSONParser config;

PDFGenerator::PDFGenerator()
{
	p_pdflib = new PDFlib;
}

PDFGenerator::~PDFGenerator()
{
	delete p_pdflib;
	p_pdflib = NULL;
}

void PDFGenerator::renew_pdflib()
{
	/* TODO                                                                      */
	/* 测试过程中发现：如果某次生成PDF出现异常，则此后每次生成都会报错：         */
	/* [2100] begin_document: Function must not be called in 'page' scope"       */
	/* 猜测是 begin_document 之后，没有正确调用 end_document                     */
	/* 因工程时间紧，在每次生成PDF前，采用此方法重新创建 pdflib 对象，保证正确性 */

	if (p_pdflib != NULL) { delete p_pdflib; }
	p_pdflib = new PDFlib;

	// 重新加载相关的配置
	load_pdflib_config();
}

void PDFGenerator::load_pdflib_config()
{
	PDFGenerator* p_pdfg = PDFGenerator::get_instance();
	wstring wkey, wval, optlist = L"";

	Value opt = config.get_root()["optlist"];
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

	Value info = config.get_root()["file_info"];
	for (string key : info.getMemberNames()) {
		wkey = str2wstr(utf8_to_str(key));
		wval = str2wstr(utf8_to_str(info[key].asString()));
		// e.g. wkey: "Title", wval: "3D 建模可视化报告"
		p_pdfg->get_pdflib().set_info(wkey, wval);
	}
}

PDFGenerator* PDFGenerator::get_instance()
{
	static PDFGenerator instance;
	return &instance;
}

PDFlib& PDFGenerator::get_pdflib()
{
	return *(this->p_pdflib);
}

int PDFGenerator::load_image(const wstring& imagetype, const wstring& filename, const wstring& optlist)
{
	int image = p_pdflib->load_image(imagetype, filename, optlist);
	if (image == -1) {
		throw LoadException(L"image resource: " + filename, p_pdflib->get_errmsg());
	}
	return image;
}

int PDFGenerator::load_3ddata(const wstring& filename, const wstring& optlist)
{
	int data = p_pdflib->load_3ddata(filename, optlist);
	if (data == -1) {
		throw LoadException(filename, p_pdflib->get_errmsg());
	}
	return data;
}

int PDFGenerator::add_textflow(int textflow, const wstring& text, const wstring& optlist)
{
	int tf = p_pdflib->add_textflow(textflow, text, optlist);
	if (tf == -1) {
		throw TextException(text, p_pdflib->get_errmsg());
	}
	return tf;
}

int PDFGenerator::add_table_cell(int table, int column, int row, const wstring& text, const wstring& optlist)
{
	int tb = p_pdflib->add_table_cell(table, column, row, text, optlist);
	if (tb == -1) {
		throw TableException(column, row, p_pdflib->get_errmsg());
	}
	return tb;
}

int PDFGenerator::create_3dview(const wstring& username, const wstring& optlist)
{
	int view = p_pdflib->create_3dview(username, optlist);
	if (view == -1) {
		throw ThreeDException(username, p_pdflib->get_errmsg());
	}
	return view;
}

wstring PDFGenerator::fit_textflow(int textflow, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = p_pdflib->fit_textflow(textflow, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_nextpage") throw TextException(L"Need a next page (caused by a form feed character U+000C)");
		else if (result == L"_boxfull") throw TextException(L"The fitbox is too small to place all texts.");
		else if (result == L"_boxempty") throw TextException(L"The box doesn’t contain any text at all after processing.");
		else { /* other results */ }
	}
	return wstring();
}

wstring PDFGenerator::fit_table(int table, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = p_pdflib->fit_table(table, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_error") throw TableException(p_pdflib->get_errmsg());
		else if (result == L"_boxfull") throw TableException(L"The table box is too small to place all contents.");
		else { /* other results */ }
	}
	return result;
}
