#include "my_exception.h"

/* LoadException */

inline LoadException::LoadException() {
    this->message = "LoadException.";
}

inline LoadException::LoadException(string s) {
    this->message = "LoadException: " + s;
}

LoadException::LoadException(wstring ws)
{
    this->message = "LoadException: " + wstr_to_str(ws);
}

LoadException::LoadException(wstring wres, wstring wmsg)
{
    this->message = "LoadException: " + wstr_to_str(wres) + "\n" + wstr_to_str(wmsg);
}

LoadException::~LoadException() {}

inline const char* LoadException::what() const throw () {
    return (this->message).c_str();
}

/* CreateFileException */

inline CreateFileException::CreateFileException() {
    this->message = "CreateFileException.";
}

inline CreateFileException::CreateFileException(string s) {
    this->message = "CreateFileException: " + s;
}

CreateFileException::CreateFileException(wstring ws)
{
    this->message = "CreateFileException: " + wstr_to_str(ws);
}

CreateFileException::CreateFileException(wstring wfile, wstring wmsg)
{
    this->message = "CreateFileException: " + wstr_to_str(wfile) + "\n" + wstr_to_str(wmsg);
}

CreateFileException::~CreateFileException() {}

inline const char* CreateFileException::what() const throw () {
    return (this->message).c_str();
}

/* TextException */

inline TextException::TextException() {
    this->message = "TextException.";
}

inline TextException::TextException(string s) {
    this->message = "TextException: " + s;
}

TextException::TextException(wstring ws)
{
    this->message = "TextException: " + wstr_to_str(ws);
}

TextException::TextException(wstring wtext, wstring wmsg)
{
    this->message = "TextException: " + wstr_to_str(wtext) + "\n" + wstr_to_str(wmsg);
}

TextException::~TextException() {}

inline const char* TextException::what() const throw () {
    return (this->message).c_str();
}

/* TableException */

inline TableException::TableException() {
    this->message = "TableException.";
}

inline TableException::TableException(string s) {
    this->message = "TableException: " + s;
}

TableException::TableException(wstring ws)
{
    this->message = "TableException: " + wstr_to_str(ws);
}

TableException::TableException(wstring wtable, wstring wmsg)
{
    this->message = "TableException: " + wstr_to_str(wtable) + "\n" + wstr_to_str(wmsg);
}

TableException::TableException(int col, int row, wstring wmsg)
{
    this->message = "TableException: col " + std::to_string(col) + " row " + std::to_string(row) + 
        "\n" + wstr_to_str(wmsg);
}

TableException::~TableException() {}

inline const char* TableException::what() const throw () {
    return (this->message).c_str();
}

/* ThreeDException */

inline ThreeDException::ThreeDException() {
    this->message = "ThreeDException.";
}

inline ThreeDException::ThreeDException(string s) {
    this->message = "ThreeDException: " + s;
}

ThreeDException::ThreeDException(wstring ws)
{
    this->message = "ThreeDException: " + wstr_to_str(ws);
}

ThreeDException::ThreeDException(wstring wusr, wstring wmsg)
{
    this->message = "ThreeDException: " + wstr_to_str(wusr) + "\n" + wstr_to_str(wmsg);
}

ThreeDException::~ThreeDException() {}

inline const char* ThreeDException::what() const throw () {
    return (this->message).c_str();
}
