#include "program_exception.h"

/* LoadException */

LoadException::LoadException()
{
    message = "LoadException.";
}

LoadException::LoadException(string s) 
{
    message = "LoadException: " + s;
}

LoadException::LoadException(wstring ws)
{
    message = "LoadException: " + wstr2str(ws);
}

LoadException::LoadException(wstring wres, wstring wmsg)
{
    message = "LoadException: " + wstr2str(wres) + "\n" + wstr2str(wmsg);
}

LoadException::~LoadException() { }

const char* LoadException::what() const throw () 
{
    return message.c_str();
}

/* CreateFileException */

CreateFileException::CreateFileException() 
{
    message = "CreateFileException.";
}

CreateFileException::CreateFileException(string s) 
{
    message = "CreateFileException: " + s;
}

CreateFileException::CreateFileException(wstring ws)
{
    message = "CreateFileException: " + wstr2str(ws);
}

CreateFileException::CreateFileException(wstring wfile, wstring wmsg)
{
    message = "CreateFileException: " + wstr2str(wfile) + "\n" + wstr2str(wmsg);
}

CreateFileException::~CreateFileException() { }

const char* CreateFileException::what() const throw () 
{
    return message.c_str();
}

/* TextException */

TextException::TextException() 
{
    message = "TextException.";
}

inline TextException::TextException(string s) 
{
    message = "TextException: " + s;
}

TextException::TextException(wstring ws)
{
    message = "TextException: " + wstr2str(ws);
}

TextException::TextException(wstring wtext, wstring wmsg)
{
    message = "TextException: " + wstr2str(wtext) + "\n" + wstr2str(wmsg);
}

TextException::~TextException() { }

const char* TextException::what() const throw () 
{
    return message.c_str();
}

/* TableException */

TableException::TableException() 
{
    message = "TableException.";
}

inline TableException::TableException(string s) 
{
    message = "TableException: " + s;
}

TableException::TableException(wstring ws)
{
    message = "TableException: " + wstr2str(ws);
}

TableException::TableException(wstring wtable, wstring wmsg)
{
    message = "TableException: " + wstr2str(wtable) + "\n" + wstr2str(wmsg);
}

TableException::TableException(int col, int row, wstring wmsg)
{
    message = "TableException: col " + std::to_string(col) + " row " + std::to_string(row) +
        "\n" + wstr2str(wmsg);
}

TableException::~TableException() { }

const char* TableException::what() const throw () 
{
    return message.c_str();
}

/* ThreeDException */

ThreeDException::ThreeDException() 
{
    message = "ThreeDException.";
}

ThreeDException::ThreeDException(string s) 
{
    message = "ThreeDException: " + s;
}

ThreeDException::ThreeDException(wstring ws)
{
    message = "ThreeDException: " + wstr2str(ws);
}

ThreeDException::ThreeDException(wstring wusr, wstring wmsg)
{
    message = "ThreeDException: " + wstr2str(wusr) + "\n" + wstr2str(wmsg);
}

ThreeDException::~ThreeDException() { }

const char* ThreeDException::what() const throw () 
{
    return message.c_str();
}

/* XMLParserException */

XMLParserException::XMLParserException() 
{
    message = "XMLParserException.";
}

XMLParserException::XMLParserException(string s) 
{
    message = "XMLParserException: " + s;
}

XMLParserException::XMLParserException(wstring ws)
{
    message = "XMLParserException: " + wstr2str(ws);
}

XMLParserException::~XMLParserException() { }

const char* XMLParserException::what() const throw () 
{
    return message.c_str();
}

/* JSONParserException */

JSONParserException::JSONParserException()
{
    message = "JSONParserException.";
}

JSONParserException::JSONParserException(string s)
{
    message = "JSONParserException: " + s;
}

JSONParserException::JSONParserException(wstring ws)
{
    message = "JSONParserException: " + wstr2str(ws);
}

JSONParserException::~JSONParserException() { }

const char* JSONParserException::what() const throw ()
{
    return message.c_str();
}
