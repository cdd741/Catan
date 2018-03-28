#include "builder.hpp"

#include "dice.hpp"
#include "address.hpp"

bool Builder::winCheck() {
	return score == 10;
}

bool Builder::useResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0)
{
	// returns true if the resources are deducted from the account (successful attempt to use the resources)
	// false otherwise

	if (this->nBrick >= nBrick &&
		this->nEnergy >= nEnergy &&
		this->nGlass >= nGlass &&
		this->nHeat >= nHeat &&
		this->nWifi >= nWifi)
	{
		this->nBrick -= nBrick;
		this->nEnergy -= nEnergy;
		this->nGlass -= nGlass;
		this->nHeat -= nHeat;
		this->nWifi -= nWifi;

		return true;
	}

	return false;
}

void Builder::addResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0)
{
	this->nBrick += nBrick;
	this->nEnergy += nEnergy;
	this->nGlass += nGlass;
	this->nHeat += nHeat;
	this->nWifi += nWifi;
}

bool Builder::trade(resouseType item1, resouseType item2) {
	if (item1 == resouseType::BRICK && nBrick > 0) --nBrick;
	else if (item1 == resouseType::ENERGY && nEnergy > 0) --nEnergy;
	else if (item1 == resouseType::GLASS && nGlass > 0) --nGlass;
	else if (item1 == resouseType::HEAT && nHeat > 0) --nHeat;
	else if (item1 == resouseType::WIFI && nWifi > 0) --nWifi;
	else return false;

	return true;
}

void Builder::roll(Dice& dice)
{
	auto v = dice.roll();
	for (auto& prop : properties)
	{
		prop->collect(v);
	}
}