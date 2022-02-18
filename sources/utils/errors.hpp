#pragma once

#include <string>

class Exception: public std::exception
{
	private:
		std::string	msg;
	public:
		Exception(std::string msg) throw();
		~Exception(void) throw();
		virtual const char* what() const throw();
};

