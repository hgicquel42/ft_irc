#pragma once

#include <string>

using namespace std;

class Exception: public exception
{
	private:
		string	msg;
	public:
		Exception(string msg) throw();
		~Exception(void) throw();
		virtual const char* what() const throw();
};

class Numeric: public exception
{
	private:
		string	msg;
	public:
		Numeric(string msg) throw();
		~Numeric(void) throw();
		virtual const char* what() const throw();
};

