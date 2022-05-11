#pragma once
#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include "pdflib/pdflib.hpp"

#include "util.h"
#include "program_exception.h"
#include "json_parser.h"

using namespace std;
using namespace pdflib;

class PDFGenerator // 单例，被调用于创建PDF文件
{
public:
	static PDFGenerator* get_instance();

	PDFlib& get_pdflib();

	int load_image(const wstring& imagetype, const wstring& filename, const wstring& optlist);

	int load_3ddata(const wstring& filename, const wstring& optlist);

	int add_textflow(int textflow, const wstring& text, const wstring& optlist);

	int add_table_cell(int table, int column, int row, const wstring& text, const wstring& optlist);

	int create_3dview(const wstring& username, const wstring& optlist);

	wstring fit_textflow(int textflow, double llx, double lly, double urx, double ury, const wstring& optlist);

	wstring fit_table(int table, double llx, double lly, double urx, double ury, const wstring& optlist);

	void renew_pdflib(); // todo: 似乎生成pdf的过程中，如果pdflib出现一次异常，就无法恢复正常

	void load_pdflib_config(); // 将 config.json 中关于 pdflib 的部分导入并设置

private:
	PDFGenerator();
	~PDFGenerator();
	PDFGenerator(const PDFGenerator&);

private:
	PDFlib* p_pdflib;
};

#endif // !PDF_GENERATOR_H


