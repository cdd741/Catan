#include "address.hpp"

#include "builder.hpp"

#include <iostream>
#include <cassert>
#include "board.hpp"

#include "board.hpp"

using namespace std;

Status Road::build(Builder * owner) {
	for (auto& b : neighbours) {
		if (!built) break;
		else if (b->owned() || b->checkRoadNeighbour()) {
			if(owner->useResources(0, 0, 0, 1, 1))
				return Status::OK;
			// Not Enough
			else return Status::notOK;
		}
	}
	// You cannot build here
	return Status::notOK;
}

bool Building::checkRoadNeighbour() {
	for (auto & p : neighbours)
	{
		if (p.second->built)
			return true;
	}
	return false;
}

TerminalGrid &operator<<(TerminalGrid& out, const Road &r)
{
	out.setLocation(out.desired[(void*)&r]);
	assert(out.desired[(void*)&r] == make_coord(-1, -1));

	if (r.ID < 10) out << to_string(' ') << to_string(r.ID);
	else out << to_string(r.ID);
	return out;
}

Status Building::build(Builder * who, bool bInitial)	// bSucceeded
{
	// criteria checks
	// 1: no adjacent residences
	// 2: at least one road adjacent is built
	if (!bInitial)
	{
		size_t nBuilt = 0;
		for (auto& nb : neighbours)
		{
			if (nb.second && nb.second->built) nBuilt++;
			if (nb.first->type != None) 
				return Status::notOK;	// adjacent
		}
		if (!nBuilt) 
			return Status::notOK;	// if no roads connecting to it
									// reject if not enough resources
		if (!who->useResources(1, 1, 1, 0, 1)) 
			return Status::notOK;
	}
	type = Basement;
	owner = who;
	who->properties.push_back(this);
	return Status::OK;
}

Status Building::improve()	// allowing deriving possibilities
{
	switch (type)
	{
	case None:
	case Tower:
		return Status::notOK; // You cannot build here

	case Basement:	// upgrade to House
		if (!owner->useResources(0, 0, 2, 3, 0)) return Status::notOK; // Resources Not Enough
		type = House;
		return Status::OK;
	case House:	// upgrade to Tower
		if (!owner->useResources(3, 2, 2, 2, 1)) return Status::notOK; // Resources Not Enough
		type = Tower;
		return Status::OK;
	default:
		throw;
	}
}


void Building::connect(Building* other)
{ 
	if (!neighbours[other])
	{
		neighbours[other] = new Road();
		other->neighbours[this] = neighbours[other];
	}
}

Road* Building::isConnected(const Building* other) const
{
	return neighbours.at(other);
}
