#pragma once
#ifndef ADDRESS_H
#define ADDRESS_H

#include <vector>

#include "exception.hpp"
#include "tile.hpp"

class Builder;
class Tile;

// a road connects ends of Addresses (allows the possibility to extend this into multiple ended roads
// you may want to convert this to a class if you prefer
struct Road
{
	bool built = false;
	Builder* owner = nullptr;
	std::vector<Address*> ends;
};

class Address
{
public:
	enum HouseType
	{
		None, Basement, House, Tower
	};

	/* a dice roll has occured, collect resources to owner */
	void collect(unsigned int diceRoll)
	{
		for (auto& pTile : tiles)
			pTile->produce(diceRoll, owner);
	}

	bool build(bool bInitial = false);	// bSucceeded

	// a default param of houseType could extend the possibilities for upgrading in different paths (skipping stages)
	// or if there are multiple non-intersecting paths, creating another status for such cases would be a good soln
	// in the worst case, improve() gets derived in the subclasses for non-default paths/addresses
	virtual bool improve() throw(NotImplementedException);

protected:
	Builder* owner = nullptr;
	std::vector<Road*> roads;	// roads to this address
	std::vector<Tile*> tiles;	// tiles this address is allowed to collect
	HouseType type = None;
};


#endif
