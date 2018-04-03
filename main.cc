#include "board.hpp"
#include "builder.hpp"
#include "resource.hpp"
#include "dice.hpp"
#include "status.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
	Layout * layout = nullptr;
	while(true){
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
			else if (argv[i] == "-random-board") { layout = new RandomLayout(9); }
		}

		if (!layout) layout = new Layout(ifstream(fname), 9);
		Board board(layout);
		board.assignUIIndexes();
		cout << board;


		if (players.size() == 0) {
			players.push_back(new Builder(Player::Blue));
			players.push_back(new Builder(Player::Red));
			players.push_back(new Builder(Player::Orange));
			players.push_back(new Builder(Player::Yellow));
			int i = 0;
			while (true) {
				int idx;
				if (i < 4) idx = i;
				else if (i < 8) idx = 7 - i;
				else break;

				cout << "Builder " << Player::to_string(players[idx]->colour) << ", ";
				cout << "where do you want to build a basement?" << endl << ">";
				int place;
				while (true) {
					cin >> place;
					if (cin.fail()) {
						cin.clear();
						cin.ignore();
						cout << "Invalid input." << endl;
						continue;
					}
					if (place > 53) {
						cout << "Invalid input." << endl;
						continue;
					}
					if (board.buildRes(players[idx], place, true) == Status::OK) break;
					
					cout << Status::cantBuildHere << endl;
				}
				++i;
			}
		}
		board.getPlayers(players);

		// Add Code Here
		string cmd;
		Builder* player;
		bool won = false;
		int currTurn = 0;

		// game play
		while (true) {
			if (currTurn == 4) currTurn -= 4;
			player = players[currTurn];
			cout << "Player " << Player::to_string(player->colour) << " set dice." << endl;
			//set dice && roll
			bool bLoaded = false;
			while (true) {
				cout << ">";
				cin >> cmd;
				if (cmd == "load")
				{
					bLoaded = true;
					cout << "Loaded dice set." << endl;
					//d = new LoadedDice(2);
				}
				else if (cmd == "fair")
				{
					//set to fairdice
					bLoaded = false;
					cout << "Fair dice set." << endl;
				}
				else if (cmd == "roll")
				{
					int roll;
					//check dicetype && roll.
					if (bLoaded) {
						cout << "Input a roll between 2 and 12:" << endl;
						while (true) {
							cout << ">";
							cin >> roll;

							if (cin.fail()) {
								cin.clear();
								cin.ignore();
								cout << "Invalid input." << endl;
							}
							else if (roll <= 12 && roll >= 2) break;
							else cout << "Invalid roll." << endl;
						}
						// set fair dice
					}
					else
					{
						FairDice fd;
						roll = fd.roll();
					}

					cout << "Dice rolled to " << roll << endl;

					if (roll == 7)
					{
						cout << "A SEVEN has occured." << endl;
						// helf player resources
						for (auto p : players) p->half();
						int place = -1;

						auto stat = Status::OK;
						do
						{
							if (stat != Status::OK)
							{
								cout << stat << endl;
							}
							cout << "Choose where to place the GEESE." << endl;
							cin >> place;
							if (cin.fail()) {
								cin.clear();
								cin.ignore();
								cout << "Invalid input." << endl;
							}
							//BUG!!! Throw has occured
						} while ((stat = board.movingGeese(place)) != Status::OK);
						bool hasNeighbours = false;
						unordered_set<Builder*> nei;
						for (auto& b : board.neighbours(place))
						{
							if (b->owned() && b->owned() != player && b->owned()->hasAnyResources())
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
										cout << "Builder " << Player::to_string(player->colour)
											<< " steals " << n->loseRandom()
											<< " from builder " << Player::to_string(n->colour);

									}
								}
								cout << "Invalid." << endl;
							}

						}
						else
							cout << "Builder " << Player::to_string(player->colour)
							<< " has no builders to steal from." << endl;
					}
					else
						board.diceRoll(roll);
					break;
				}
				else cout << "Invalid input." << endl;

			}

			while (true) {
				cout << "Please enter your command, enter <help> for more information." << endl;
				cout << ">";
				cin >> cmd;
				if (cin.eof()) {
					// Save to backup.sv and end game
					break;
				}
				else if (!cin)
				{
					break;
				}
				else if (cmd == "board") {
					cout << board;
				}
				else if (cmd == "status") {
					board.playerStatus();
				}
				else if (cmd == "residences") {
					for (auto &prop : players[currTurn]->properties) {
						cout << prop->ID << " ";
					}
				}
				else if (cmd == "build-road") {
					cout << "Place enter address." << endl;
					int criteriaNum;
					cout << ">";
					cin >> criteriaNum;
					if (criteriaNum > 71) {
						cout << "Invalid input." << endl;
						continue;
					}
					Status s = board.buildRoad(player, criteriaNum);
					//output depends on s: cant build/ no enough resources
					if (s != Status::OK) cout << s << endl;
				}
				else if (cmd == "build-res") {
					cout << "Place enter address." << endl;
					int criteriaNum;
					cout << ">";
					cin >> criteriaNum;
					if (criteriaNum > 53) {
						cout << "Invalid input." << endl;
						continue;
					}
					Status s = board.buildRes(player, criteriaNum);
					//output depends on s: cant build/ no enough resources
					if (s != Status::OK) cout << s << endl;
					if (player->winCheck()) {
						won = true;
						break;
					}
				}
				else if (cmd == "improve") {
					cout << "Place enter address." << endl;
					int criteriaNum;
					cout << ">";
					cin >> criteriaNum;
					if (criteriaNum > 53) {
						cout << "Invalid input." << endl;
						continue;
					}
					Status s = board.improve(player, criteriaNum);
					//output depends on s: no enough resources
					if (s != Status::OK) cout << s << endl;
					if (player->winCheck()) {
						won = true;
						break;
					}
				}
				else if (cmd == "trade") {
					string colour, responce;
					resourceType give, take;
					cout << ">";
					cin >> colour >> give >> take;
					cout << "Does " << colour << "accept this offer?" << endl;
					cout << ">";
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
					cout << "status" << endl << "residences" << endl;
					cout << "build-road <path#>" << endl;
					cout << "build-res <housing#>" << endl;
					cout << "improve <housing#>" << endl;
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
#if _DEBUG
				else if (cmd == "quit")
				{
					return 0;
				}
#endif
				else {
					cout << "Invalid command" << endl;
				}
			}

			if (won) break;
		}

		if (won) {
			cout << Player::to_string(player->colour) << " won!" << endl;
			cout << "Would you like to play again? (yes/no)" << endl;
			while (true) {
				cin >> cmd;
				if (cmd == "yes") break;
				else if (cmd == "no") break;
				else cout << "Invalid input." << endl;
			}
		}
		// destruct

		if (cmd != "yes") break;
	}

	return 0;

}
