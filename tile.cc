#include "tile.hpp"
#include "address.hpp"
#include "builder.hpp"
#include "player.hpp"

using namespace std;
//addResources(size_t nBrick = 0, size_t nEnergy = 0, size_t nGlass = 0, size_t nHeat = 0, size_t nWifi = 0);
void Tile::distribute() {
    for (auto & b : buildings) {
        if (type == resourceType::BRICK) {
            b->owner->addresource(nResources,0,0,0,0);
            b->owner->iBrick += nResources;
        }
        if (type == resourceType::ENERGY) {
            b->owner->addresource(0,nResources,0,0,0);
            b->owner->iEnergy += nResources;
        }
        if (type == resourceType::GLASS) {
            b->owner->addresource(0,0,nResources,0,0);
            b->owner->iGlass += Resources;
        }
        if (type == resourceType::HEAT) {
            b->owner->addresource(0,0,0,nResources,0);
            b->owner->iHeat += nResources;
        }
        if (type == resourceType::WIFI) {
            b->owner->addresource(0,0,0,0,nResources);
            b->owner->iWifi += nResources;
        }
        if (b->type == Type::Basement){
            b->owner->score += 1;
        }
        if (b->type == Type::House) {
            b->owner->score += 2;
        }
        if (b->type == Type::Tower) {
            b->owner->score += 3;
        }
    }
}

void Tile::changeProduction() {
	if (bProduction) bProduction = false;
	else bProduction = true;
	return;
}
