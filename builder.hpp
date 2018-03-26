#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <vector>

class Dice;
class Building;

class Builder
{
public:
	// default vals to zero for future extensions
	// note: another possible soln is to use a vector/unordered_set to pass with {type, count}
	// more flexible, but slower

	// while this soln does the job and doesn't need code changes in the future
	// the price is the need to insert a number of zeroes ahead
	// e.g., using 1 wifi would have to use a call "useResources(0, 0, 0, 0, 1)" which is less intuitive

	bool useResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0)
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

	void addResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0)
	{
		this->nBrick += nBrick;
		this->nEnergy += nEnergy;
		this->nGlass += nGlass;
		this->nHeat += nHeat;
		this->nWifi += nWifi;
	}

	void roll(Dice& dice);	// note: the dice is only rolled once in this func call

protected:
	// the Builder knows exactly what property (s)he owns
	std::vector<Building*> properties;

	size_t nBrick = 0, nEnergy = 0, nGlass = 0, nHeat = 0, nWifi = 0;
	
};

#endif
