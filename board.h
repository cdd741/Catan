#pragma once
#ifndef BOARAD_H
#define BOARAD_H

#include <vector>;
#include "tile.hpp"


class Board {
	std::vector<Tile> tiles;
	std::vector<Builder> builders;
};

#endif // !BOARAD_H
