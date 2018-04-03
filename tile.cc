#include "tile.hpp"
#include "address.hpp"
#include "builder.hpp"
#include "player.hpp"
#include "resources.hpp"

using namespace std;

void Tile::distribute() {
	for (auto & b : buildings) {
		if (type == TileType::Brick) {
			if (b->type == Building::Basement) {
				(b->owned())->addResources(1, 0, 0, 0, 0);
			}
			if (b->type == Building::House) {
				(b->owned())->addResources(2, 0, 0, 0, 0);
			}
			if (b->type == Building::Tower) {
				(b->owned())->addResources(3, 0, 0, 0, 0);
			}
		}
		if (type == TileType::Energy) {
			if (b->type == Building::Basement) {
				(b->owned())->addResources(0, 1, 0, 0, 0);
			}
			if (b->type == Building::House) {
				(b->owned())->addResources(0, 2, 0, 0, 0);
			}
			if (b->type == Building::Tower) {
				(b->owned())->addResources(0, 3, 0, 0, 0);
			}
		}
		if (type == TileType::Glass) {
			if (b->type == Building::Basement) {
				(b->owned())->addResources(0, 0, 1, 0, 0);
			}
			if (b->type == Building::House) {
				(b->owned())->addResources(0, 0, 2, 0, 0);
			}
			if (b->type == Building::Tower) {
				(b->owned())->addResources(0, 0, 3, 0, 0);
			}
		}
		if (type == TileType::Heat) {
			if (b->type == Building::Basement) {
				(b->owned())->addResources(0, 0, 0, 1, 0);
			}
			if (b->type == Building::House) {
				(b->owned())->addResources(0, 0, 0, 2, 0);
			}
			if (b->type == Building::Tower) {
				(b->owned())->addResources(0, 0, 0, 3, 0);
			}
		}
		if (type == TileType::Wifi) {
			if (b->type == Building::Basement) {
				(b->owned())->addResources(0, 0, 0, 0, 1);
			}
			if (b->type == Building::House) {
				(b->owned())->addResources(0, 0, 0, 0, 2);
			}
			if (b->type == Building::Tower) {
				(b->owned())->addResources(0, 0, 0, 0, 3);
			}
		}
	}
}

void Tile::changeProduction() {
	if (bProduction) bProduction = false;
	else bProduction = true;
	return;
}
