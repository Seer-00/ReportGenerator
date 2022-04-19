#include "my_pdflib.h"

MyPDFlib::MyPDFlib()
{
}

MyPDFlib::~MyPDFlib()
{
}

PDFlib& MyPDFlib::get_p()
{
	return this->p;
}

int MyPDFlib::load_font(const wstring& fontname, const wstring& encoding, const wstring& optlist)
{
	int font = p.load_font(fontname, encoding, optlist);
	if (font == -1) {
		throw LoadException(L"font resource: " + fontname, p.get_errmsg());
	}
	return font;
}

int MyPDFlib::load_image(const wstring& imagetype, const wstring& filename, const wstring& optlist)
{
	int image = p.load_image(imagetype, filename, optlist);
	if (image == -1) {
		throw LoadException(L"image resource: " + filename, p.get_errmsg());
	}
	return image;
}

int MyPDFlib::load_3ddata(const wstring& filename, const wstring& optlist)
{
	int data = p.load_3ddata(filename, optlist);
	if (data == -1) {
		throw LoadException(filename, p.get_errmsg());
	}
	return data;
}

int MyPDFlib::add_textflow(int textflow, const wstring& text, const wstring& optlist)
{
	int tf = p.add_textflow(textflow, text, optlist);
	if (tf == -1) {
		throw TextException(text, p.get_errmsg());
	}
	return tf;
}

int MyPDFlib::add_table_cell(int table, int column, int row, const wstring& text, const wstring& optlist)
{
	int tb = p.add_table_cell(table, column, row, text, optlist);
	if (tb == -1) {
		throw TableException(column, row, p.get_errmsg());
	}
	return tb;
}

int MyPDFlib::create_3dview(const wstring& username, const wstring& optlist)
{
	int view = p.create_3dview(username, optlist);
	if (view == -1) {
		throw ThreeDException(username, p.get_errmsg());
	}
	return view;
}

wstring MyPDFlib::fit_textflow(int textflow, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = p.fit_textflow(textflow, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_nextpage") throw TextException(L"Need a next page (caused by a form feed character U+000C)");
		else if (result == L"_boxfull") throw TextException(L"The fitbox is too small to place all texts.");
		else if (result == L"_boxempty") throw TextException(L"The box doesn¡¯t contain any text at all after processing.");
		else { /* other results */ }
	}
	return wstring();
}

wstring MyPDFlib::fit_table(int table, double llx, double lly, double urx, double ury, const wstring& optlist)
{
	wstring result = p.fit_table(table, llx, lly, urx, ury, optlist);
	if (!(result == L"_stop")) { // !ok
		if (result == L"_error") throw TableException(p.get_errmsg());
		else if (result == L"_boxfull") throw TableException(L"The table box is too small to place all contents.");
		else { /* other results */ }
	}
	return result;
}
