#include "board.hpp"

#include "exception.hpp"
#include "resources.hpp"
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
				pTile = new BRICK(roll);
				break;
			case 1:
				pTile = new ENERGY(roll);
				break;
			case 2:
				pTile = new GLASS(roll);
				break;
			case 3:
				pTile = new HEAT(roll);
				break;
			case 4:
				pTile = new WIFI(roll);
				break;
			case 5:
				pTile = new PARK(roll);
				break;
			default:
				throw NotImplementedException("undefined layout format");
			}
			graph[make_pair(row_idx, base + i)] = pTile;
			tiles[index] = pTile;
		}
	}

}

Status Board::movingGeese(int tileidx) {
	if (!layout->tiles[tileidx]) return Status::notOK;
	if (Geese == layout->tiles[tileidx])
		return Status::notOK;
	Geese->changeProduction();
	Geese = layout->tiles[tileidx];
	Geese->changeProduction();
	return Status::OK;
}

Status Board::buildRoad(Builder* player, int address) {
	road_map[address]->build(player);
	return Status::OK;
}

Status Board::buildRes(Builder* player, int address) {
	addr_map[address]->build(player);
	return Status::OK;
}

Status Board::improve(Builder* player, int address) {
	player->improve(address);
	return Status::OK;
}

//Status Board::trade(Builder* player1, Builder* player2, resourceType item1, resourceType item2) {
//	player1->trade(item1, item2);
//	player2->trade(item2, item1);
//}
//
//// distribute resourses or goto geeseOcccur if dice is 7
void Board::diceRoll(int dice) {
	bool anyoneGained = false;
	for (auto & t : layout->tiles) {
		if (t.second->roll == dice) {
			t.second->distribute();
		}
	}

	for (int i = 0; i<4; ++i) {
		if (builders[i]->iHeat == 0 && builders[i]->iBrick == 0 && builders[i]->iEnergy == 0
			&& builders[i]->iWifi == 0 && builders[i]->iGlass == 0) continue;
		else {
			anyoneGained = true;
			break;
		}
	}

	if (anyoneGained) {

		for (int i = 0; i<4; ++i) {
			if (i == 0) cout << "Builder Blue gained:" << endl;
			if (i == 1) cout << "Builder Red gained:" << endl;
			if (i == 2) cout << "Builder Orange gained:" << endl;
			if (i == 3) cout << "Builder Orange gained:" << endl;
			cout << builders[i]->iHeat << " Heat" << endl;
			cout << builders[i]->iBrick << " Brick" << endl;
			cout << builders[i]->iEnergy << " Energy" << endl;
			cout << builders[i]->iWifi << " Wifi" << endl;
			cout << builders[i]->iGlass << " Glass" << endl;
			builders[i]->iHeat = 0;
			builders[i]->iBrick = 0;
			builders[i]->iEnergy = 0;
			builders[i]->iWifi = 0;
			builders[i]->iGlass = 0;
		}

	}
	else {
		cout << "No builders gained resources." << endl;
	}
}

