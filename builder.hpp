#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <vector>
#include <iostream>
#include <unordered_set>
#include "player.hpp"
#include "resource.hpp"
#include "status.hpp"
#include "dice.hpp"
class Building;
class TerminalGrid;
class Builder
{
public:
    size_t iBrick = 0, iEnergy = 0, iGlass = 0, iHeat = 0, iWifi = 0;
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
	std::string loseRandom()
	{	
		int nProperties = nBrick + nEnergy + nGlass + nHeat + nWifi;
		if (!nProperties) return "-1";	// TODO, throw not enough to lose
		CustomDice d(0, nProperties-1);
		std::vector<size_t> v{ nBrick, nEnergy, nGlass, nHeat, nWifi };
		auto r = d.roll();
		auto i = 0;
		while (r)
		{
			if (r > v[i])
				r -= v[i];
			else
				break;

		}
		switch (i)
		{
		case 0:
			useResource(resourceType::BRICK);
			return "BRICK";
		case 1:
			useResource(resourceType::ENERGY);
			return "ENERGY";
		case 2:
			useResource(resourceType::GLASS);
			return "GLASS";
		case 3:
			useResource(resourceType::HEAT);
			return "HEAT";
		case 4:
			useResource(resourceType::WIFI);
			return "WIFI";
		default:
			throw;
		}
	}

	std::unordered_set<Building*> properties;
	Builder(Player::col colour) :colour{ colour } {}
	Player::col colour;

protected:
	// the Builder knows exactly what property (s)he owns
	

	size_t nBrick = 0, nEnergy = 0, nGlass = 0, nHeat = 0, nWifi = 0;
	size_t score = 0;
    
    
	
};

#endif
