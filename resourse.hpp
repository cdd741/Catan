#ifndef RESOURSE_H
#define RESOURSE_H

#include <string>
#include <istream>
#include <unordered_map>
enum class resourceType { BRICK, ENERGY, GLASS, HEAT, WIFI };

static const std::unordered_map<std::string, resourceType> mapping =
{
	{ "BRICK", resourceType::BRICK },
	{ "ENERGY", resourceType::ENERGY },
	{ "GLASS", resourceType::GLASS },
	{ "HEAT", resourceType::HEAT },
	{ "WIFI", resourceType::WIFI },
};

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

#endif // !RESOURSE_H
