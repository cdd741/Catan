#include "tile.hpp"

using namespace std;

void Tile::distribute(unsigned int diceRoll) {
	if (diceRoll != roll) return;
	for (auto building : Buildings)
		building->collect();
}