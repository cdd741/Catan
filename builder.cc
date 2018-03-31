#include "builder.hpp"

#include "dice.hpp"
#include "address.hpp"

using namespace std;
bool Builder::winCheck() {
	return score == 10;
}

bool Builder::useResources(size_t nBrick, size_t nEnergy, size_t nGlass, size_t nHeat, size_t nWifi)
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

void Builder::addResources(size_t nBrick, size_t nEnergy, size_t nGlass, size_t nHeat, size_t nWifi)
{
	this->nBrick += nBrick;
	this->nEnergy += nEnergy;
	this->nGlass += nGlass;
	this->nHeat += nHeat;
	this->nWifi += nWifi;
}

Status Builder::trade(Builder* other, resourceType item1, resourceType item2) {
	if (chkResource(item1) && other->chkResource(item2)) {
		useResource(item2); 
		other->useResource(item1);
		return Status::OK;
	}
	// Not enough resources
	return Status::notOK;
}

bool Builder::chkResource(resourceType typ, size_t ct)
{
	switch (typ)
	{
	case resourceType::BRICK:
		return nBrick >= ct;
	case resourceType::ENERGY:
		return nEnergy >= ct;
	case resourceType::GLASS:
		return nGlass >= ct;
	case resourceType::HEAT:
		return nHeat >= ct;
	case resourceType::WIFI:
		return nWifi >= ct;
	}
}

bool Builder::useResource(resourceType typ, size_t ct)
{
	switch (typ)
	{
	case resourceType::BRICK:
		return nBrick >= ct;
	case resourceType::ENERGY:
		return nEnergy >= ct;
	case resourceType::GLASS:
		return nGlass >= ct;
	case resourceType::HEAT:
		return nHeat >= ct;
	case resourceType::WIFI:
		return nWifi >= ct;
	}
}

Status Builder::improve(int address) {
	Status s;
	for (auto building : properties) {
		if (building->ID == address)
			return building->improve();
	}
	// You cannot build here
	return Status::notOK;
}


