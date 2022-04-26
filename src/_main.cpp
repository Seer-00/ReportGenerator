#include "function.h"

using namespace std;

int main()
{
	try {
		init();
		parse_template();
	}
	catch (PDFlib::Exception& ex) {
		wcerr << L"PDFlib exception occurred:" << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cerr << "Generic C++ exception occurred!" << endl;
	}
	
    return 0;
}
