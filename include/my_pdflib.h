#pragma once
#ifndef MY_PDFLIB_H
#define MY_PDFLIB_H

#include "pdflib.hpp"

#include "util.h"
#include "my_exception.h"

using namespace std;
using namespace pdflib;

class MyPDFlib
{
public:
	MyPDFlib();
	~MyPDFlib();

	PDFlib& get_p();

	int load_font(const wstring& fontname, const wstring& encoding, const wstring& optlist);

	int load_image(const wstring& imagetype, const wstring& filename, const wstring& optlist);

	int load_3ddata(const wstring& filename, const wstring& optlist);

	int add_textflow(int textflow, const wstring& text, const wstring& optlist);

	int add_table_cell(int table, int column, int row, const wstring& text, const wstring& optlist);

	int create_3dview(const wstring& username, const wstring& optlist);

	wstring fit_textflow(int textflow, double llx, double lly, double urx, double ury, const wstring& optlist);

	wstring fit_table(int table, double llx, double lly, double urx, double ury, const wstring& optlist);

private:
	PDFlib p;
};

#endif // !MY_PDFLIB_H


