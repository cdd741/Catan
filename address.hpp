#pragma once
#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "exception.hpp"
#include "tile.hpp"
// #include "status.hpp"


class Builder;
class Tile;

class Road{
public:
	unsigned int ID = -1;
	Status build(Builder * owner, bool bInitial = false);
	friend std::ostream &operator<<(std::ostream &out, const Road &r);	
	Builder* owned() const { return owner; }
protected:
	bool built = false;
	Builder* owner = nullptr;
	std::unordered_set<Building*> neighbours;
};


class Building
{
public:
	Builder * owner = nullptr;
	enum Type
	{
		None, Basement, House, Tower
	};

	bool checkRoadNeighbour();
	Status build(Builder * who, bool bInitial = false);	// bSucceeded
	Status improve();
	
	void connect(Building* other);
	Road* isConnected(const Building* other) const;

	friend std::ostream &operator<<(std::ostream &out, const Building &b);
	unsigned int ID = -1;
	Type getType()const { return type; }
	Builder* owned() const { return owner; }
	Type type = None;
protected:
	std::unordered_map<const Building*, Road*> neighbours;
};


#endif
