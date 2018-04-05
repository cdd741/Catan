#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

#include "board.hpp"
#include "builder.hpp"
#include "resource.hpp"
#include "dice.hpp"
#include "status.hpp"
#include <thread>

#ifdef USING_XWINDOW
#include "window.h"
#endif

#ifdef USING_SDL2
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "graphics.hpp"
#endif

using namespace std;

int main(int argc, char* argv[])
{
#ifdef USING_SDL2
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);


	GRAPHICS::SharedContext shared;
	thread gui_thread(GRAPHICS::start_threaded_sdl2, &shared);
#endif

#ifdef USING_XWINDOW
	Xwindow window(800, 600);
#endif

	Layout * layout = nullptr;
	string layout_fname{ "layout.txt" };
	vector<string> players_data;
	int loaded_geese = -1;

	try {
		while (true) {

			vector<Builder*> players;
			int currTurn = 0;

			for (int i = 1; i < argc; ++i) {
				if (argv[i] == string("-seed")) {
					stringstream ss{ argv[++i] };
					int n;
					ss >> n;
					Dice::seed(n);
				}
				else if (argv[i] == string("-load"))
				{
					ifstream in(argv[++i]);
					in >> currTurn;
					in.ignore();

					for (auto i = 0; i < 4; i++)
					{
						string s;
						getline(in, s);
						players_data.push_back(s);
					}
					layout = new Layout(in, 9);
					in >> loaded_geese;
				}
				else if (argv[i] == string("-board")) {
					layout_fname = argv[++i];
				}
				else if (argv[i] == string("-random-board")) { layout = new RandomLayout(9); }
			}

			if (!layout)
			{
				ifstream layout_in(layout_fname);
				layout = new Layout(layout_in, 9);
			}
			Board board(layout);
			board.assignUIIndexes();

			if (players_data.size())	// loading
			{
				for (int i = 0; i < 4; i++)
				{
					stringstream ss(players_data[i]);
					
					switch (i)
					{
					case 0:
						players.push_back(new Builder(ss, Player::Blue));
						break;
					case 1:
						players.push_back(new Builder(ss, Player::Red));
						break;
					case 2:
						players.push_back(new Builder(ss, Player::Orange));
						break;
					case 3:
						players.push_back(new Builder(ss, Player::Yellow));
						break;
					}
					
					std::string s;
					ss >> s;	// 'r'
					assert(s == "r");

					// roads
					while (ss >> s)
					{
						if (s == "h") break;
						std::stringstream sss(s);
						int addr;
						sss >> addr;
						board.buildRoad(players.back(), addr, true);	// initial road, simply assign as succ.
					}

					// houses
					while (true)
					{
						int addr;
						if (!(ss >> addr)) break;
						char typ;
						ss >> typ;
						switch (typ)
						{
						case 'B':
							board.load_residence(players.back(), addr, Building::Basement);
							break;
						case 'H':
							board.load_residence(players.back(), addr, Building::House);
							break;
						case 'T':
							board.load_residence(players.back(), addr, Building::Tower);
							break;
						default:
							throw "err";
						};

					}
				}
				if (loaded_geese != -1)
				{
					board.setGeese(loaded_geese);
				}
			}

			cout << board;
#ifdef USING_XWINDOW
			board.render(&window);
#endif
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
					cout << "where do you want to build a basement?" << endl;
					int place;
					while (true) {
						cout << ">";
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
					#ifdef USING_XWINDOW
					board.render();
					#endif
					++i;
				}
			}

			board.getPlayers(players);

			// Add Code Here
			string cmd;
			Builder* player;
			bool won = false;
			

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

					if (cin.eof()) throw "end";
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

								if (cin.eof()) throw "end";
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
								cout << "Choose where to pleace the GEESE." << endl;
								cout << ">";
								string tok;
								cin >> tok;
								if (cin.eof()) throw "end";

								stringstream ss(tok);
								ss >> place;

								if (ss.fail()) 
									cout << "Invalid input." << endl;
								if (place > 53)
									cout << "Invalid input." << endl;
								//BUG!!! Throw has occured
							} while ((stat = board.movingGeese(place)) != Status::OK);
							#ifdef USING_XWINDOW
							board.render();
							#endif
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
								cout << "Builder " << Player::to_string(player->colour) << " can choose to steal from";
								stringstream ss;
								for (auto & n : nei)
								{
									ss << ' ' << Player::to_string(n->colour) << ',';
								}
								cout << ss.str().substr(0, ss.str().length() - 1) << '.' << endl;
								cout << "Choose a builder to steal from." << endl;
								string tok;
								cout << ">";
								cin >> tok;

								if (cin.eof()) throw "end";
								bool bHandled = false;
								while (!bHandled)
								{
									for (auto& n : nei)
									{
										if (Player::to_string(n->colour) == tok)
										{
											bHandled = true;
											string p = n->loseRandom();
											cout << "Builder " << Player::to_string(player->colour)
												<< " steals " << p
												<< " from builder " << Player::to_string(n->colour) 
												<< '.' << endl;

										}
									}
									if (!bHandled) cout << "Invalid." << endl;
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
#ifdef USING_XWINDOW
						board.render(&window);
#endif
					cout << "Please enter your command, enter <help> for more information." << endl;
					cout << ">";
					cin >> cmd;

					if (cin.eof()) throw "end";

					else if (!cin)
					{
						break;
					}
					else if (cmd == "current-player") cout << Player::to_string(player->colour) << endl;
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
						cout << endl;
					}
					else if (cmd == "build-road") {
						cout << "Place enter address." << endl;
						int criteriaNum;
						cout << ">";
						cin >> criteriaNum;

						if (cin.eof()) throw "end";
						if (cin.fail()) {
							cin.clear();
							cin.ignore();
							cout << "Invalid input." << endl;
						}
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

						if (cin.eof()) throw "end";
						else if (cin.fail()) {
							cin.clear();
							cin.ignore();
							cout << "Invalid input." << endl;
							continue;
						}
						else if (criteriaNum > 53) {
							cout << "Invalid input." << endl;
							continue;
						}
						else {
							Status s = board.buildRes(player, criteriaNum);
							//output depends on s: cant build/ no enough resources
							if (s != Status::OK) cout << s << endl;
							if (player->winCheck()) {
								won = true;
								break;
							}
						}
					}
					else if (cmd == "improve") {
						cout << "Place enter address." << endl;
						int criteriaNum;
						cout << ">";
						cin >> criteriaNum;

						if (cin.eof()) throw "end";
						else if (cin.fail()) {
							cin.clear();
							cin.ignore();
							cout << "Invalid input." << endl;
							continue;
						}
						else if (criteriaNum > 53) {
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
						string responce, colour;
						resourceType give, take;
						cout << "Please enter who do you want to trade with." << endl << ">";
						while (true) {
							cin >> colour;
							if (colour == "Blue" || colour == "Red" || colour == "Orange" || colour == "Yellow") break;
							cout << "Wrong input, please check the first letter is capitalized." << endl << '>';
						}
						if (cin.eof()) throw "end";
						cout << "Please enter what do you want to give." << endl;
						while (true) {
							cout << ">";
							try {
								cin >> give;
							} catch(string &s) {
								if (s == "eof") throw "end";
								else cout << "Invalid input." << endl;
								continue;
							}
							break;
						}
						cout << "Please enter what do you want to take." << endl;
						while (true) {
							cout << ">";
							try {
								cin >> take;
							}
							catch (string &s) {
								if (s == "eof") throw "end";
								else cout << "Invalid input." << endl;
							}
							break;
						}
						cout << "Does " << colour << " accept this offer? (yes/no)" << endl;
						while (true) {
							cout << ">";
							cin >> responce;
							if (cin.eof()) throw "end";
							if (responce == "yes") {
								Builder * player2;
								if (colour == "Blue") player2 = players[0];
								else if (colour == "Red") player2 = players[1];
								else if (colour == "Orange") player2 = players[2];
								else if (colour == "Yellow") player2 = players[3];
								/* self-assign check?*/
								Status s = player->trade(player2, give, take);
								break;
							}
							else if (responce == "no") break;
							else {
								cout << "Invalid command" << endl;
								continue;
							}
						}

					}
					else if (cmd == "help") {
						cout << "current-player" << endl;;
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
						ofstream save(file);
						save << currTurn << endl;
						board.saveTo(save);
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
					cout << '<';
					cin >> cmd;
					if (cmd == "yes") break;
					else if (cmd == "no") break;
					else cout << "Invalid input." << endl;
				}
			}
			// destruct

			if (cmd != "yes") break;
		}
	}
	catch(string &str) {
		// when eof
		// Save as backup.sv
	}

#ifdef USING_SDL2
	shared.push(GRAPHICS::Event(GRAPHICS::Event::QUIT));
	gui_thread.join();
	SDL_Quit();
#endif

	return 0;

}
