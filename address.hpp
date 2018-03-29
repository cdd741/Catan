#pragma once
#ifndef ADDRESS_H
#define ADDRESS_H

#include <vector>
#include <unordered_map>

#include "exception.hpp"
#include "tile.hpp"
#include "status.hpp"

class Builder;
class Tile;

struct Road
{
	bool built = false;
	Builder* owner = nullptr;
	Status build(Builder * who);
};

class Building
{
public:
	enum Type
	{
		None, Basement, House, Tower
	};

	/* a dice roll has occured, collect resources to owner */
	void collect(){
		if (owner)
			for (auto& pTile : 1 )
				pTile->produce(diceRoll, owner);
	}

	Status build(Builder * who, bool bInitial = false);	// bSucceeded

	// a default param of houseType could extend the possibilities for upgrading in different paths (skipping stages)
	// or if there are multiple non-intersecting paths, creating another status for such cases would be a good soln
	// in the worst case, improve() gets derived in the subclasses for non-default paths/Buildings
	virtual Status improve() throw(NotImplementedException);

	std::vector<Tile*> tiles;	// tiles this address is allowed to collect

	void connect(Building* other)
	{
		if (!neighbours[other])
		{
			neighbours[other] = new Road();
			other->neighbours[this] = neighbours[other];
		}
	}

	bool isConnected(const Building* other) const
	{
		return neighbours.at(other);
	}

protected:
	std::unordered_map<const Building*, Road*> neighbours;

	Builder* owner = nullptr;
	Type type = None;
};


#endif
