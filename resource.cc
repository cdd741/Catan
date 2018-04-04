#include "resource.hpp"

std::istream &operator>>(std::istream &in, resourceType &b)
{
	std::string token;
	in >> token;

	if (mapping.count(token))
		b = mapping.at(token);
	else if(in.eof())
		throw "eof";
	else throw "Invalid";

	return in;
}
