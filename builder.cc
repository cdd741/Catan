#include "builder.hpp"

#include "dice.hpp"
#include "address.hpp"

void Builder::roll(Dice& dice)
{
	auto v = dice.roll();
	for (auto& prop : properties)
	{
		prop->collect(v);
	}
}
