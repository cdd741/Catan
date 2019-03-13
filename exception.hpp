#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H

/* Exception Types */

#include <stdexcept>

/*
This exception signals a component of the instance is not fully implemented and an attempt to use such component(s) is detected.

note:
works as a placeholder / reminder for developers to notice certain unimplmented features during testing time
*/
class NotImplementedException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

class InvalidInputException : public std::exception
{
public:
	using std::exception::exception;
};

// throws when cin.eof received, end the game and store the layout
class EOFException : public std::exception
{
public:
	using std::exception::exception;
};

#endif
