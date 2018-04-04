#ifndef RESOURCE_H
#define RESOURCE_H

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



std::istream &operator>>(std::istream &in, resourceType &b);

#endif // !RESOURCE_H
