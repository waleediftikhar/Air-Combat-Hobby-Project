#ifndef EXCEPTIONS_HPP_INCLUDED
#define EXCEPTIONS_HPP_INCLUDED

#include <exception>
#include <string>



class FileException : public std::exception
{
private:
	std::string description;

public:

	FileException(std::string desc) : description(desc) {}

	virtual const char* what() const throw()
	{
		return description.c_str();
	}
};

#endif // EXCEPTIONS_HPP_INCLUDED
