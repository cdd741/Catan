#include "board.hpp"
#include "builder.hpp"
#include "resource.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) 
{
	string cmd;
	int currTurn;
	ifstream layout_in("layout.txt");
	Board board(new Layout(layout_in, 9));
	vector<Builder*> players;
	// Read command && construct/load Board
	// Add Code Here
	int dice;
	if (currTurn == 4) currTurn -= 4;
	Builder* player = players[currTurn];
	// game play
	while (true) {
		//set dice && roll
		while (true) {
		

		}

		while (true) {
			cin >> cmd;
			if (cmd == "board") {
				cout << board;
			}
			else if (cmd == "status") {
				for (int i = 0; i <= 3; ++i) {
					cout << &players[i];
				}
			}
			else if (cmd == "residences") {
				//player->residences();
			}
			else if (cmd == "build-road") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.buildRoad(player, criteriaNum);
			}
			else if (cmd == "build-res") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.buildRes(player, criteriaNum);
			}
			else if (cmd == "improve") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.improve(player, criteriaNum);
			}
			else if (cmd == "trade") {
				string colour, responce;
				resourceType give, take;
				cin >> colour >> give >> take;
				cout << "Does " << colour << "accept this offer?" << endl;
				cin >> responce;
				while (true) {
					if (responce == "yes") {
						Builder * player2;
						if (colour == "Blue") player2 = players[0];
						else if (colour == "Red") player2 = players[1];
						else if (colour == "Orange") player2 = players[2];
						else if (colour == "Yello") player2 = players[3];
						/* self-assign check?*/
						Status s = player->trade(player2, give, take);
						continue;
					}
					else if (responce == "no") continue;
					else cout << "Invalid command" << endl;
				}

			}
			else if (cmd == "help") {
				cout << "Valid commands:" << endl << "board" << endl;
				cout << "status" << endl << "criteria" << endl;
				cout << "achieve <goal>" << endl;
				cout << "complete <criterion>" << endl;
				cout << "improve <criterion>" << endl;
				cout << "trade <colour> <give> <take>" << endl;
				cout << "next" << endl << "save <file>" << endl;
				cout << "help" << endl;
			}
			else if (cmd == "save") {
				string file;
				cin >> file;
				// Add Code Here
			}
			else if (cmd == "next") {
				if (currTurn == 3) /*pInd = 0;*/;
				else ++currTurn;
				break;
				// next turn
			}
			else {
				cout << "Invalid command" << endl;
			}
	}
	}
	
	return 0;

}
