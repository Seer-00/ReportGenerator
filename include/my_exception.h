#pragma once
#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H

#include <exception>
#include <string>

#include <util.h>

using namespace std;

class LoadException : public exception
{
public:
    LoadException();
    LoadException(string s);
    LoadException(wstring ws);
    LoadException(wstring wres, wstring wmsg);
    ~LoadException();

    const char* what() const throw ();

private:
    string message;
};

class CreateFileException : public exception
{
public:
    CreateFileException();
    CreateFileException(string s);
    CreateFileException(wstring ws);
    CreateFileException(wstring wfile, wstring wmsg);
    ~CreateFileException();

    const char* what() const throw ();

private:
    string message;
};

class TextException : public exception
{
public:
    TextException();
    TextException(string s);
    TextException(wstring ws);
    TextException(wstring wtext, wstring wmsg);
    ~TextException();

    const char* what() const throw ();

private:
    string message;
};

class TableException : public exception
{
public:
    TableException();
    TableException(string s);
    TableException(wstring ws);
    TableException(wstring wtable, wstring wmsg);
    TableException(int col, int row, wstring wmsg);
    ~TableException();

    const char* what() const throw ();

private:
    string message;
};

class ThreeDException : public exception
{
public:
    ThreeDException();
    ThreeDException(string s);
    ThreeDException(wstring ws);
    ThreeDException(wstring wusr, wstring wmsg);
    ~ThreeDException();

    const char* what() const throw ();

private:
    string message;
};

#endif // !MY_EXCEPTION_H


