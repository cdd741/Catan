#pragma once
#ifndef RESOURCES_H
#define RESOURCES_H

#include "tile.hpp"

class BRICK : public Tile
{
	using Tile::Tile;
	void produce_res(Builder* to) override
	{
		to->addResources(nResources);
	}
};

class ENERGY : public Tile
{
	using Tile::Tile;
	void produce_res(Builder * to) override
	{
		to->addResources(0, nResources);
	}
};

class GLASS : public Tile
{
	using Tile::Tile;
	void produce_res(Builder * to) override
	{
		to->addResources(0, 0, nResources);
	}
};

class HEAT : public Tile
{
	using Tile::Tile;
	void produce_res(Builder * to) override
	{
		to->addResources(0, 0, 0, nResources);
	}
};

class WIFI : public Tile
{
	using Tile::Tile;
	void produce_res(Builder * to) override
	{
		to->addResources(0, 0, 0, 0, nResources);
	}
};

class PARK : public Tile
{
	using Tile::Tile;
	void produce_res(Builder * to) override
	{
		to->addResources();
	}
};

#endif
