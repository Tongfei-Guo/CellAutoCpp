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

#endif