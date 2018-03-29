#include "resource.hpp"

std::istream &operator>>(std::istream &in, resourceType &b)
{
	std::string token;
	in >> token;

	if (mapping.count(token))
		b = mapping.at(token);
	else
		throw;

	return in;
}
