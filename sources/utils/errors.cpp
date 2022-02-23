#include "errors.hpp"

using namespace std;

Exception::Exception(string msg) throw():
	msg(msg)
{}

Exception::~Exception(void) throw()
{}

const char *Exception::what(void) const throw()
{
	return (this->msg.c_str());
}

Error::Error(string msg) throw():
	msg(msg)
{}

Error::~Error(void) throw()
{}

const char *Error::what(void) const throw()
{
	return (this->msg.c_str());
}