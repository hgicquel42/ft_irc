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

Numeric::Numeric(string msg) throw():
	msg(msg)
{}

Numeric::~Numeric(void) throw()
{}

const char *Numeric::what(void) const throw()
{
	return (this->msg.c_str());
}