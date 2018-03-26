#pragma once
#ifndef ADDRESS_H
#define ADDRESS_H

#include <vector>
#include <unordered_map>

#include "exception.hpp"
#include "tile.hpp"

class Builder;
class Tile;

struct Road
{
	bool built = false;
	Builder* owner = nullptr;
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
		if (owner)
			for (auto& pTile : tiles)
				pTile->produce(diceRoll, owner);
	}

	bool build(Builder * who, bool bInitial = false);	// bSucceeded

	// a default param of houseType could extend the possibilities for upgrading in different paths (skipping stages)
	// or if there are multiple non-intersecting paths, creating another status for such cases would be a good soln
	// in the worst case, improve() gets derived in the subclasses for non-default paths/addresses
	virtual bool improve() throw(NotImplementedException);

	std::vector<Tile*> tiles;	// tiles this address is allowed to collect

	void connect(const Address* other)
	{
		if (!neighbours[other])
			neighbours[other] = new Road();
	}

	bool isConnected(const Address* other) const
	{
		return neighbours.at(other);
	}

protected:
	std::unordered_map<const Address*, Road*> neighbours;

	Builder* owner = nullptr;
	HouseType type = None;
};


#endif
