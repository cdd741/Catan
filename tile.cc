#include "tile.hpp"
#include "address.hpp"
#include "builder.hpp"
#include "player.hpp"
#include "resources.hpp"

using namespace std;

void Tile::distribute() {
    for (auto & b : buildings) {
        if (type == TileType::Brick) {
            (b->owned())->addResources(nResources,0,0,0,0);
            (b->owned())->iBrick += nResources;
        }
        if (type == TileType::Energy) {
            (b->owned())->addResources(0,nResources,0,0,0);
            (b->owned())->iEnergy += nResources;
        }
        if (type == TileType::Glass) {
            (b->owned())->addResources(0,0,nResources,0,0);
            (b->owned())->iGlass += Resources;
        }
        if (type == TileType::Heat) {
            (b->owned())->addResources(0,0,0,nResources,0);
            (b->owned())->iHeat += nResources;
        }
        if (type == TileType::Wifi) {
            (b->owned())->addResources(0,0,0,0,nResources);
            (b->owned())->iWifi += nResources;
        }
        if (b->type == Type::Basement){
            (b->owned())->score += 1;
        }
        if (b->type == Type::House) {
            (b->owned())->score += 2;
        }
        if (b->type == Type::Tower) {
            (b->owned())->score += 3;
        }
    }
}

void Tile::changeProduction() {
	if (bProduction) bProduction = false;
	else bProduction = true;
	return;
}
