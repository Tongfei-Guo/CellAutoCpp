#ifndef CA_EXCEPTION_H
#define CA_EXCEPTION_H

#include <string>
#include <stdexcept>

class nonexist_type : public std::out_of_range
{
public:
	nonexist_type(const std::string &&m) : out_of_range("map::at"), message(std::move(m)){}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
private:
	std::string message;
};

class combine_error : public std::exception
{
public:
	combine_error(const std::string &&m) : message(std::move(m)){}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
private:
	std::string message;
};

class internal_error : public std::exception
{
public:
	internal_error(const std::string &&m) : message(std::move(m)){}
	virtual const char* what() const throw()
	{
		return ("Library internal error at function call : " + message).c_str();
	}
private:
	std::string message;
};

#endif
