#include "address.hpp"

#include "builder.hpp"

bool Address::build(Builder * who, bool bInitial)	// bSucceeded
{
	// criteria checks
	// 1: no adjacent residences
	// 2: at least one road adjacent is built
	if (!bInitial)
	{
		size_t nBuilt = 0;
		for (auto& pRoad : roads)
		{
			if (pRoad->ends.size())	// contains adjacent
				return false;
			if (pRoad->built)
				nBuilt++;
		}
		if (!nBuilt) return false;	// if no roads connecting to it

									// reject if not enough resources
		if (!who->useResources(1, 1, 1, 0, 1)) return false;
	}

	type = Basement;
	owner = who;

	return true;
}

bool Address::improve()	// allowing deriving possibilities
{
	switch (type)
	{
	case None:
	case Tower:
		return false;

	case Basement:	// upgrade to House
		if (!owner->useResources()) return false;
		type = House;
		return true;
	case House:	// upgrade to Tower
		if (!owner->useResources()) return false;
		type = Tower;
		return true;
	default:
		throw NotImplementedException("the current houseType is not handled in Address.improve(), perhaps a new houseType is added while handlers are not updated to reflect that.");
	}
}
