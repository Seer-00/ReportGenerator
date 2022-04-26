#include "pdf_generator.h"

PDFGenerator::PDFGenerator() { }

PDFGenerator::~PDFGenerator() { }

PDFGenerator* PDFGenerator::get_instance()
{
	static PDFGenerator instance;
	return &instance;
}

PDFlib& PDFGenerator::get_pdflib()
{
	return this->pdflib;
}

int PDFGenerator::load_image(const wstring& imagetype, const wstring& filename, const wstring& optlist)
{
	int image = pdflib.load_image(imagetype, filename, optlist);
	if (image == -1) {
		throw LoadException(L"image resource: " + filename, pdflib.get_errmsg());
	}
	return image;
}

int PDFGenerator::load_3ddata(const wstring& filename, const wstring& optlist)
{
	int data = pdflib.load_3ddata(filename, optlist);
	if (data == -1) {
		throw LoadException(filename, pdflib.get_errmsg());
	}
	return data;
}

int PDFGenerator::add_textflow(int textflow, const wstring& text, const wstring& optlist)
{
	int tf = pdflib.add_textflow(textflow, text, optlist);
	if (tf == -1) {
		throw TextException(text, pdflib.get_errmsg());
	}
	return tf;
}

int PDFGenerator::add_table_cell(int table, int column, int row, const wstring& text, const wstring& optlist)
{
	int tb = pdflib.add_table_cell(table, column, row, text, optlist);
	if (tb == -1) {
		throw TableException(column, row, pdflib.get_errmsg());
	}
	return tb;
}

int PDFGenerator::create_3dview(const wstring& username, const wstring& optlist)
{
	int view = pdflib.create_3dview(username, optlist);
	if (view == -1) {
		throw ThreeDException(username, pdflib.get_errmsg());
	}
	return view;
}

wstring PDFGenerator::fit_textflow(int textflow, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = pdflib.fit_textflow(textflow, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_nextpage") throw TextException(L"Need a next page (caused by a form feed character U+000C)");
		else if (result == L"_boxfull") throw TextException(L"The fitbox is too small to place all texts.");
		else if (result == L"_boxempty") throw TextException(L"The box doesn¡¯t contain any text at all after processing.");
		else { /* other results */ }
	}
	return wstring();
}

wstring PDFGenerator::fit_table(int table, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = pdflib.fit_table(table, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_error") throw TableException(pdflib.get_errmsg());
		else if (result == L"_boxfull") throw TableException(L"The table box is too small to place all contents.");
		else { /* other results */ }
	}
	return result;
}
