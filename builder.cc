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

Status Builder::improve(int address) {
	Status s;
	for (auto building : properties) {
		if (building->ID == address)
			return building->improve();
	}
	// You cannot build here
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
	throw;
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
	throw;
}

void Builder::half() {
	int quantity = nBrick + nEnergy + nGlass + nHeat + nWifi;
	int lBrick, lEnergy, lGlass, lHeat, lWifi;
	int times = quantity / 2;
	if (quantity >= 10) {
		for (int i = 0; i < times; ++i) {
			CustomDice cDice{ 1, quantity };
			unsigned int roll = cDice.roll();
			if ((roll -= nBrick) <= 0) {
				--nBrick;
				++lBrick;
			}
			else if ((roll -= nEnergy) <= 0) {
				--nEnergy;
				++lEnergy;
			}
			else if ((roll -= nGlass) <= 0) {
				--nGlass;
				++lGlass;
			}
			else if ((roll -= nHeat) <= 0) {
				--nHeat;
				++lHeat;
			}
			else {
				--nWifi;
				++lWifi;
			}
			--quantity;
		}
		cout << "Builder " << Player::to_string(colour) << "loses " << times << " resouces to the geese. ";
		cout << "They lose:" << endl;
		if (lBrick != 0) cout << lBrick << "BRICK" << endl;
		if (lEnergy != 0) cout << lEnergy << "ENERGY" << endl;
		if (lGlass != 0) cout << lGlass << "GLASS" << endl;
		if (lHeat != 0) cout << lHeat << "HEAT" << endl;
		if (lWifi != 0) cout << lWifi << "WIFI" << endl;
	}
}



