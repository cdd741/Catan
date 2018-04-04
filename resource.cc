
#include <algorithm>
#include "resource.hpp"
#include "exception.hpp"

std::istream &operator>>(std::istream &in, resourceType &b)
{
	std::string token;
	in >> token;
	std::transform(token.begin(), token.end(), token.begin(), ::toupper);

	if (mapping.count(token))
		b = mapping.at(token);
	else if(in.eof())
		throw EOFException();
	else throw InvalidInputException();

	return in;
}
