#include "address.hpp"

#include "builder.hpp"

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

	return Status::OK;
}

Status Building::improve()	// allowing deriving possibilities
{
	switch (type)
	{
	case None:
	case Tower:
		return Status::notOK;

	case Basement:	// upgrade to House
		if (!owner->useResources()) return Status::notOK;
		type = House;
		return Status::OK;
	case House:	// upgrade to Tower
		if (!owner->useResources()) return Status::notOK;
		type = Tower;
		return Status::OK;
	default:
		throw NotImplementedException("the current houseType is not handled in Building.improve(), perhaps a new houseType is added while handlers are not updated to reflect that.");
	}
}
