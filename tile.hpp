#pragma once
#ifndef TILE_H
#define TILE_H

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
	unsigned int roll;	// associated roll # for this Tile to produce resources

public:
	Tile(unsigned int roll) : roll{ roll } {}

	// this is not necessary though, or not perferred imo (i don't like it personally)
	// you may convince me to keep/remove this block though
	// (another approach is to let Address perform those actions instead of letting Tile's know details of Builder(s))
	// (just some random thoughts), this works though
	virtual void produce(unsigned int diceRoll, Builder* builder)
	{
		if (!bProduction) return;
		if (diceRoll == roll)
			builder->addResources();
		throw NotImplementedException("Tile.produce()");
	}

	// a bit awk to have this flag in public
	// although Tile is really just a very simple struct-like class
	bool bProduction = true;
};

#endif
