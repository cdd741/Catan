#include "board.hpp"

#include "exception.hpp"
#include <iostream>

using namespace std;

/*
anything connecting piece in the lower half of any shape
counting from left to right with i->n, start with the X+1, Y->Y+i
dne's auto -> nullptr
*/
void Layout::load(istream& in)
{
	int type, roll;

	int r_ct[]{ 1,2,3,4,5,6,7,8 };


	int index = 0;
	//  a graph is intepr. as a row-coln relationship
	for (auto row_idx = 0; row_idx < 9; row_idx++)
	{
		size_t max_ct = row_idx + 1;
		for (auto i = 0; i < row_ct[row_idx]; i++)
		{
			auto base = (max_ct - row_ct[row_idx]) / 2;
			in >> type >> roll;

			Tile* pTile;
			switch (type)
			{
			case 0:
				pTile = new Tile(roll, index, 1, TileType::Brick);
				break;
			case 1:
				pTile = new Tile(roll, index, 1, TileType::Energy);
				break;
			case 2:
				pTile = new Tile(roll, index, 1, TileType::Glass);
				break;
			case 3:
				pTile = new Tile(roll, index, 1, TileType::Heat);
				break;
			case 4:
				pTile = new Tile(roll, index, 1, TileType::Wifi);
				break;
			case 5:
				pTile = new Tile(roll, index, 1, TileType::Park);
				break;
			default:
				throw NotImplementedException("undefined layout format");
			}
			graph[make_pair(row_idx, base + i)] = pTile;
			tiles[index++] = pTile;
		}
	}

}

Status Board::movingGeese(int tileidx) {
	if (!layout->tiles[tileidx] || Geese == layout->tiles[tileidx]) return Status::cantPutHere;
	if (Geese) Geese->changeProduction();
	Geese = layout->tiles[tileidx];
	Geese->changeProduction();
	return Status::OK;
}

Status Board::buildRoad(Builder* player, int address, bool bInitial) {
	return road_map[address]->build(player, bInitial);
}

Status Board::buildRes(Builder* player, int address, bool bInitial) {
	return addr_map[address]->build(player, bInitial);
}

Status Board::improve(Builder* player, int address) {
	return player->improve(address);
}

//// distribute resourses or goto geeseOcccur if dice is 7
void Board::diceRoll(int dice) {
	bool anyoneGained = false;
	for (auto & t : layout->tiles) {
		if (t.second == Geese) continue;
		if (t.second && t.second->roll == dice) {
			t.second->distribute();
		}
	}

	for (int i = 0; i<4; ++i) {
		if (!(builders[i]->iHeat == 0 && builders[i]->iBrick == 0 && builders[i]->iEnergy == 0
			&& builders[i]->iWifi == 0 && builders[i]->iGlass == 0)) {
			cout << "Builder " << Player::to_string(builders[i]->colour) << " gained:" << endl;
			if (builders[i]->iHeat) cout << builders[i]->iHeat << " Heat" << endl;
			if (builders[i]->iBrick) cout << builders[i]->iBrick << " Brick" << endl;
			if (builders[i]->iEnergy) cout << builders[i]->iEnergy << " Energy" << endl;
			if (builders[i]->iWifi) cout << builders[i]->iWifi << " Wifi" << endl;
			if (builders[i]->iGlass) cout << builders[i]->iGlass << " Glass" << endl;
			anyoneGained = true;
			builders[i]->iHeat = 0;
			builders[i]->iBrick = 0;
			builders[i]->iEnergy = 0;
			builders[i]->iWifi = 0;
			builders[i]->iGlass = 0;
		}
	}

	if (!anyoneGained) {
		cout << "No builders gained resources." << endl;
	}
}

