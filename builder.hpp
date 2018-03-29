#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <vector>
#include <iostream>
#include "player.hpp"
#include "resourse.hpp"
#include "status.hpp"

class Dice;
class Building;
class TerminalGrid;
class Builder
{
public:
	// default vals to zero for future extensions
	// note: another possible soln is to use a vector/unordered_set to pass with {type, count}
	// more flexible, but slower

	// while this soln does the job and doesn't need code changes in the future
	// the price is the need to insert a number of zeroes ahead
	// e.g., using 1 wifi would have to use a call "useResources(0, 0, 0, 0, 1)" which is less intuitive

	bool winCheck();
	bool useResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0);
	void addResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0);
	Status trade(Builder* other, resourceType item1, resourceType item2);
	Status improve(int address);
	friend std::ostream &operator>>(std::istream &in, const Builder &b);
	bool chkResource(resourceType typ, size_t ct = 1);
	bool useResource(resourceType typ, size_t ct = 1);

	std::vector<Building*> properties;

protected:
	// the Builder knows exactly what property (s)he owns
	
	Player colour;
	size_t nBrick = 0, nEnergy = 0, nGlass = 0, nHeat = 0, nWifi = 0;
	size_t score = 0;
	
};

#endif
