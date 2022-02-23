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

class Error: public exception
{
	private:
		string	msg;
	public:
		Error(string msg) throw();
		~Error(void) throw();
		virtual const char* what() const throw();
};

