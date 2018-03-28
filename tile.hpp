#pragma once
#ifndef TILE_H
#define TILE_H

#include <vector>
#include <unordered_set>

#include "exception.hpp"
#include "builder.hpp"

/*
Tiles produces resources if there are no conditions preventing it from doing so. 

note:
either use Factory model to produce Tile* with different XXXTiles derived from it
or adjust the production values within the Tile class itself


it is also possible to make a class for each type of resources and use Resource* to perform those actions
details should be discussed sometime in the future

*/
class Building;
class Road;

enum class TileType { Brick, Energy, Glass, Heat, Wifi, Park };

class Tile
{
	TileType type;
	std::unordered_set<Building*> Buildings; // save adjacent Buildings (from bz)
	std::unordered_set<Road*> roads;		// save adjacent roads     (from bz)

public:
	Tile(unsigned int roll, size_t nResources = 1) : nResources{ nResources }, roll{ roll } {}

	// this is not necessary though, or not perferred imo (i don't like it personally)
	// you may convince me to keep/remove this block though
	// (another approach is to let Building perform those actions instead of letting Tile's know details of Builder(s))
	// (just some random thoughts), this works though
	virtual void produce(unsigned int diceRoll, Builder* owner)
	{
		if (!bProduction || diceRoll != roll) return;
		produce_res(owner);
	}

	void distribute(unsigned int diceRoll);


	// a bit awk to have this flag in public
	// although Tile is really just a very simple struct-like class
	bool bProduction = true;
	std::unordered_set<Building*> buildings;

protected:
	size_t nResources;
	unsigned int roll;	// associated roll # for this Tile to produce resources
	virtual void produce_res(Builder* to) = 0;
};

#endif
