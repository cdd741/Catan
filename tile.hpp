#pragma once
#ifndef TILE_H
#define TILE_H

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
class Tile
{
public:
	Tile(unsigned int roll) : roll{ roll } {}

	// this is not necessary though, or not perferred imo (i don't like it personally)
	// you may convince me to keep/remove this block though
	// (another approach is to let Address perform those actions instead of letting Tile's know details of Builder(s))
	// (just some random thoughts), this works though
	virtual void produce(unsigned int diceRoll)
	{
		if (!bProduction || diceRoll != roll) return;
		for (auto& owner : owners)
			owner->addResources();
		throw NotImplementedException("Tile.produce()");
	}

	// a bit awk to have this flag in public
	// although Tile is really just a very simple struct-like class
	bool bProduction = true;

	/* TODO: better naming */
	void add_owner(Builder * owner)
	{
		owners.insert(owner);
	}

	void remove_owner(Builder * owner)
	{
		owners.erase(owner);
	}

protected:
	unsigned int roll;	// associated roll # for this Tile to produce resources

	// ensuring each tile could only produce once for each Builder
	std::unordered_set<Builder*> owners;

};

#endif
