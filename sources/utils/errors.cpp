#include "errors.hpp"

Exception::Exception(std::string msg) throw():
	msg(msg)
{}

Exception::~Exception(void) throw()
{}

const char *Exception::what(void) const throw()
{
	return (this->msg.c_str());
}