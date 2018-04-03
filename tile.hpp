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

struct info_cons
{
	Building *l = nullptr, *lu = nullptr, *ll = nullptr, *r = nullptr, *ru = nullptr, *rl = nullptr;
	void merge(const info_cons& other)	// in place
	{
		// assertions needed, cant have conflict without one side being nullptr
		l = l ? l : other.l;
		lu = lu ? lu : other.lu;
		ll = ll ? ll : other.ll;
		r = r ? r : other.r;
		ru = ru ? ru : other.ru;
		rl = rl ? rl : other.rl;
	}
};
typedef std::pair<int, int> Coordinate2D;


enum class TileType { Brick, Energy, Glass, Heat, Wifi, Park, UNDEFINED };
class TerminalGrid;
class Tile
{
	TileType type;
	std::unordered_set<Road*> roads;		// save adjacent roads     (from bz)

public:
	Tile(unsigned int roll, unsigned int index, size_t nResources = 1, TileType typ = TileType::UNDEFINED) : nResources{ nResources }, roll{ roll }, index{ index }, type{ typ } {}

	// this is not necessary though, or not perferred imo (i don't like it personally)
	// you may convince me to keep/remove this block though
	// (another approach is to let Building perform those actions instead of letting Tile's know details of Builder(s))
	// (just some random thoughts), this works though
/*	void produce(unsigned int diceRoll, Builder* owner)
	{
		if (!bProduction || diceRoll != roll) return;
		produce_res(owner);
	}   */

	void distribute();


	// a bit awk to have this flag in public
	// although Tile is really just a very simple struct-like class
	bool bProduction = true;
	void changeProduction();
	std::unordered_set<Building*> buildings;

	info_cons info;
	Coordinate2D coord;
	unsigned int index = -1;
	unsigned int roll;	// associated roll # for this Tile to produce resources

protected:
	size_t nResources;
};

#endif
