#include "board.hpp"
#include "builder.hpp"
#include "resources.hpp"

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
	unordered_set<int> s{3, 5, 7};

	//  a graph is intepr. as a row-coln relationship
	for (auto row_idx = 0; row_idx < 9; row_idx++)
		for (auto i=0; i<row_ct[row_idx]; i++)
			while (in >> type >> roll)
			{
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
				
				if (row_idx == 8)	// last row
					graph[make_pair(row_idx, 2)] = pTile;
				else if (s.count(row_idx))
					graph[make_pair(row_idx, i+1)] = pTile;
				else
					graph[make_pair(row_idx, i)] = pTile;
			}
}
