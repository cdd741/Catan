#include "player.hpp"
#include <string>

std::istream &operator>>(std::istream &in, Player::col &c) {
	std::string s;
	in >> s;
	if (s == "Red") c = Player::Red;
	else if (s == "Blue") c = Player::Blue;
	else if (s == "Yellow") c = Player::Yellow;
	else if (s == "Orange") c = Player::Orange;
	else throw;
	return in;
}
