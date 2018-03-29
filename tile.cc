#include "tile.hpp"
#include "address.hpp"

using namespace std;

void Tile::distribute(unsigned int diceRoll) {
	if (diceRoll != roll) return;
	for (auto building : buildings);
		//building->collect();
}