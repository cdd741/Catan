#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>

class Player
{
public:
	virtual ~Player() = 0;

	enum col{ Blue, Red, Orange, Yellow };
	static std::string to_string(col col)
	{
		switch (col) {
		case Player::Blue:
			return "Blue";
			break;
		case Player::Red:
			return "Red";
			break;
		case Player::Yellow:
			return "Yellow";
			break;
		case Player::Orange:
			return "Orange";
			break;
		}
		return "err";
	}
};


#endif // !PLAYER_H
