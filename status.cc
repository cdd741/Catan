#include "status.hpp"

using namespace std;

ostream &operator<<(ostream &out, const Status &s) {
	switch (s)
	{
	case Status::cantBuildHere:
		out << "You cannot build here.";
		break;
	case Status::notEnoughResources:
		out << "Not enough resources.";
		break;
	case Status::cantPutHere:
		out << "You cannot put here.";
		break;
	default:
		break;
	}
	return out;
}

