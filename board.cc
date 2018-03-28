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

	int row_ct[]{1, 2, 3, 2, 3, 2, 3, 2, 1};
	int r_ct[]{ 1,2,3,4,5,6,7,8 };
	unordered_set<int> s{3, 5, 7};

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
		}
	}

}


Status Board::buildRoad(Builder* player, int address) {
	roads[address]->build(player);
}

Status Board::buildRes(Builder* player, int address) {
	buildings[address]->build(player);
}

Status Board::improve(Builder* player, int address) {
	buildings[address]->build(player);
}

Status Board::trade(Builder* player1, Builder* player2, resouseType item1, resouseType item2) {
	player1->trade(item1, item2);
	player2->trade(item2, item1);
}

// distribute resourses or goto geeseOcccur if dice is 7
Status Board::diceRoll(int dice) {
	if (dice == 7) this->geeseOccur();
	else for (auto& tile : tiles) {
		tile->produce(dice)
	}
}

