#include "address.hpp"

#include "builder.hpp"

#include <iostream>
#include <cassert>
#include "board.hpp"

#include "board.hpp"

using namespace std;

Status Road::build(Builder * owner) {
	for (auto& b : neighbours) {
		if (owned()) break;
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
		if (p.second->owned())
			return true;
	}
	return false;
}

ostream &operator<<(ostream& out, const Road &r)
{
	if (r.ID < 10) out << ' ' << r.ID;
	else out << r.ID;
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
			if (nb.second && nb.second->owned()) nBuilt++;
			if (nb.first->type != None) 
				return Status::notOK;	// adjacent
		}
		if (!nBuilt) 
			return Status::notOK;	// if no roads connecting to it
									// reject if not enough resources
		if (!who->useResources(1, 1, 1, 0, 1)) 
			return Status::notOK;
	}
	else if(owned()) return Status::notOK; // You cannot build here
	else {
		type = Basement;
		owner = who;
		who->properties.insert(this);
	}
	cout << Player::to_string(who->colour) << " has buildt:" << endl;
	cout << ID << " Basement" << endl;
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

ostream &operator<<(ostream& out, const Building &b)
{
	out << '|';
	if (b.owned() == nullptr)
	{
		if (b.ID < 10)
			out << ' ' << b.ID;
		else
			out << b.ID;
		return out << '|';
	}
	switch (b.owned()->colour) {
	case Player::Blue:
		out << 'B';
		break;
	case Player::Red:
		out << 'R';
		break;
	case Player::Orange:
		out << 'O';
		break;
	case Player::Yellow:
		out << 'Y';
		break;
	default:
		if (b.ID < 10)
			out << ' ' << b.ID;
		else
			out << b.ID;
		return out << '|';
	}
	assert(b.getType() != Building::None);
	switch (b.getType()) {
	case Building::Basement:
		out << 'B';
		break;
	case Building::House:
		out << 'H';
		break;
	case Building::Tower:
		out << 'T';
		break;
	}
	return out << '|';
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
	try
	{
		return neighbours.at(other);
	}
	catch (std::out_of_range& e)
	{
		return nullptr;
	}
}

