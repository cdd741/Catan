#pragma once
#ifndef ADDRESS_H
#define ADDRESS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "exception.hpp"
#include "tile.hpp"
#include "status.hpp"

class Builder;
class Tile;

class Road{
	
	Builder* owner = nullptr;
	std::unordered_set<Building*> neighbours;
public:
	bool built = false;
	bool owned() const { return owner; }
	Status build(Builder * owner);
};


class Building
{
public:
	enum Type
	{
		None, Basement, House, Tower
	};

	bool checkRoadNeighbour();
	Status build(Builder * who, bool bInitial = false);	// bSucceeded
	Status improve();
	
	void connect(Building* other);
	bool isConnected(const Building* other) const;

	friend TerminalGrid &operator<<(TerminalGrid &out, const Builder &b);
	unsigned int ID = -1;

	bool owned() const { return owner; }
protected:
	std::unordered_map<const Building*, Road*> neighbours;
	Builder* owner = nullptr;
	Type type = None;
};


#endif
