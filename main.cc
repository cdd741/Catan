#include "board.hpp"
#include "builder.hpp"
#include "resource.hpp"
#include "dice.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	vector<Builder*> players;
	string fname{ "layout.txt" };
	
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == "-seed") {
			stringstream ss{ argv[++i] };
			int n;
			ss >> n;
			Dice::seed(n);
		}
		else if (argv[i] == "-load") {}
		else if (argv[i] == "-board") {
			fname = argv[++i];
		}
		else if (argv[i] == "-random-board") {}
	}

	ifstream layout_in(fname);
	Board board(new Layout(layout_in, 9));
	
	if (players.size() == 0) {
		players[0] = new Builder{ Player::Blue };
		players[1] = new Builder{ Player::Red };
		players[2] = new Builder{ Player::Orange };
		players[3] = new Builder{ Player::Yellow };
	}
    
    board->builders = players;
    
	// Add Code Here
	string cmd;
	int currTurn = 0;
	
	// game play
	while (true) {
		if (currTurn == 4) currTurn -= 4;
		Builder* player = players[currTurn];
		//set dice && roll
		while (true) {
			cin >> cmd;
			bool bLoaded = false;
			if (cmd == "load")
			{
				bLoaded = true;
				//d = new LoadedDice(2);
			}
			else if (cmd == "fair")
			{
				//set to fairdice
				bLoaded = false;
			}
			else if (cmd == "roll")
			{
				int roll;
				//check dicetype && roll.
				if (bLoaded) {
					cout << "Input a roll between 2 and 12:" << endl;
					
					while (true) {
						cin >> roll;
						if (roll <= 12 && roll >= 2) break;
						cout << "Invalid roll." << endl;
					}
					// set fair dice
				}
				else
				{
					FairDice fd;
					roll = fd.roll();
				}

				if (roll == 7)
				{
					// in/out geese loc
					board.geeseOccur();
					int place;
					do
					{
						cout << "Choose where to place the GEESE." << endl;
						cin >> place;
					} 
					while (board.movingGeese(place) != Status::OK);
					bool hasNeighbours = false;
					unordered_set<Builder*> nei;
					for (auto& b : board.neighbours(place))
					{
						if (b->owned() && b->owned() != player)
						{
							hasNeighbours = true;
							nei.insert(b->owned());
						}
					}
					if (hasNeighbours)
					{
						cout << "Builder " << Player::to_string(player->colour) << " can shoose to steal from ";
						stringstream ss;
						for (auto & n : nei)
						{
							ss << Player::to_string(n->colour) << ',';
						}
						cout << ss.str().substr(0, ss.str().length() - 1) << '.';
						cout << "Choose a builder to steal from." << endl;
						string tok;
						cin >> tok;
						bool bHandled = false;
						while (!bHandled)
						{
							for (auto& n : nei)
							{
								if (Player::to_string(n->colour) == tok)
								{
									bHandled = true;
									cout << "Builder " << Player::to_string(player->colour) << " steals " << n->loseRandom() << " from builder " << Player::to_string(n->colour);

								}
							}
							cout << "Invalid." << endl;
						}

					}
					else
						cout << "Builder " << Player::to_string(player->colour) << " has no builders to steal from." << endl;
				
				}
				else
					board.diceRoll(roll);
				break;
			}
			else cout << "Invalid input." << endl;

		}

		while (true) {
			cin >> cmd;
			if (cmd == "board") {
				cout << board;
			}
			else if (cmd == "status") {
				for (int i = 0; i <= 3; ++i) {
					cout << &players[i]; //no overloaded operand
				}
			}
			else if (cmd == "residences") {
				for (auto &prop : players[currTurn]->properties) {
					cout << prop->ID << " ";
				}
			}
			else if (cmd == "build-road") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.buildRoad(player, criteriaNum);
				//output depends on s: cant build/ no enough resources
				if (s == Status::notOK) cout << "can't build here" << endl;
			}
			else if (cmd == "build-res") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.buildRes(player, criteriaNum);
				//output depends on s: cant build/ no enough resources
				if (s == Status::notOK) cout << "can't build here" << endl;
			}
			else if (cmd == "improve") {
				int criteriaNum;
				cin >> criteriaNum;
				Status s = board.improve(player, criteriaNum);
				//output depends on s: no enough resources
				if (s == Status::notOK) cout << "no enough resources" << endl;
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
				++currTurn;
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
