#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cassert>

#include "address.hpp"
#include "builder.hpp"

#include "status.hpp"
#include "resourse.hpp"
#include "player.hpp"

class Builder;
class Tile;

struct Coordinate2DHash
{
public:
	template <typename T, typename U>
	std::size_t operator()(const std::pair<T, U> &x) const
	{
		// possible collisions when x coord > 10000, however, rare in this case
		return std::hash<T>()(x.first) * 10000 + std::hash<U>()(x.second);
	}
};

typedef std::pair<int, int> Coordinate2D;
#define make_coord(x, y) std::make_pair(x, y)
typedef std::unordered_map<Coordinate2D, Tile*, Coordinate2DHash> Graph;

class Layout
{
public:
	Layout() {}
	Layout(std::istream& in) : Layout() { load(in); }

public:
	virtual void load(std::istream& in);	// -> graph

	/*
	Graph Format Specification
	
	Suppose S is a N-sided regular shape
	Let s be a shape in S at coordinate (s1, s2)

	since S is even-sided, there are exactly N/2 (flat division) sides in the bottom half of the shape
	counting from left to right, the left most side-collapsing shape would have a coord of (s1+1, s2)
	and symmetrically, the right most side-collapsing shape would have a coord of (s1+1, s2+1)
	the second left most shape : (s1+2, s2)
	and symmetrically: (s1+2, s2+1)
	and similar construction follows

	*/
	Graph graph;
};

class Board
{
public:
	Board(Layout * layout) : layout{ layout }
	{
		construct();
	}

	// construction always starts at (0, 0)
	void construct()
	{
		// (0, 0)
		Building* l = new Building();
		Building* r = new Building();
		Building* lu = new Building();
		Building* ru = new Building();
		Building* ll = new Building();
		Building* rl = new Building();

		l->connect(lu);
		l->connect(ll);
		lu->connect(ru);
		r->connect(ru);
		r->connect(rl);
		ll->connect(rl);

		buildings.insert(l);
		buildings.insert(r);
		buildings.insert(lu);
		buildings.insert(ll);
		buildings.insert(ru);
		buildings.insert(rl);

		std::unordered_map<Coordinate2D, info_cons, Coordinate2DHash> info_stor;

		if (layout->graph[make_coord(1, 0)])
		{
			// if left-lower has a tile
			for (auto& ele : do_construct(make_coord(1, 0), nullptr, nullptr, l, ll))
			{
				if (info_stor.count(ele.first))
					info_stor[ele.first].merge(ele.second);
				else
					info_stor[ele.first] = ele.second;
			}
		}

		if (layout->graph[make_coord(1, 1)])
		{
			for (auto& ele : do_construct(make_coord(1, 1), rl, r, nullptr, nullptr))
			{
				if (info_stor.count(ele.first))
					info_stor[ele.first].merge(ele.second);
				else
					info_stor[ele.first] = ele.second;
			}
		}

		for (auto preferred = 1; preferred <= 8; preferred ++)
		{
			for (auto& p : info_stor)
			{
				if (p.first.first == preferred)
					for (auto& ele : do_construct(p.first, p.second.l, p.second.lu, p.second.ru, p.second.r))
					{
						if (info_stor.count(ele.first))
							info_stor[ele.first].merge(ele.second);
						else
							info_stor[ele.first] = ele.second;
					}
			}
		}
	}

	struct info_cons
	{
		Building *l = nullptr, *lu = nullptr, *lr = nullptr, *r = nullptr, *ru = nullptr, *rl = nullptr;
		void merge(const info_cons& other)	// in place
		{
			// assertions needed, cant have conflict without one side being nullptr
			l = l ? l : other.l;
			lu = lu ? lu : other.lu;
			lr = lr ? lr : other.lr;
			r = r ? r : other.r;
			ru = ru ? ru : other.ru;
			rl = rl ? rl : other.rl;
		}
	};

	std::vector<std::pair<Coordinate2D, info_cons>> do_construct(const Coordinate2D& coord, Building * l, Building * lu, Building * ru, Building * r)
	{
		// cannot have all zeros (becomes a free standing tile)
		assert(!(l == nullptr && r == nullptr && lu == nullptr && ru == nullptr));

		if (!l) l = new Building();
		if (!lu) lu = new Building();
		if (!ru) ru = new Building();
		if (!r) r = new Building();
		Building* ll = new Building();
		Building* rl = new Building();

		// make sure they are connected
		l->connect(lu);
		lu->connect(ru);
		ru->connect(r);
		r->connect(rl);
		rl->connect(ll);
		ll->connect(l);

		buildings.insert(l);
		buildings.insert(lu);
		buildings.insert(r);
		buildings.insert(ru);
		buildings.insert(rl);
		buildings.insert(ll);

		layout->graph[coord]->buildings.insert(l);
		layout->graph[coord]->buildings.insert(r);
		layout->graph[coord]->buildings.insert(lu);
		layout->graph[coord]->buildings.insert(ll);
		layout->graph[coord]->buildings.insert(ru);
		layout->graph[coord]->buildings.insert(rl);

		std::vector<std::pair<Coordinate2D, info_cons>> v;
		if (layout->graph[make_coord(coord.first + 1, coord.second)])
		{
			info_cons info;
			info.ru = l;
			info.r = ll;
			v.push_back(make_pair(make_coord(coord.first + 1, coord.second), info));
		}
		if (layout->graph[make_coord(coord.first + 1, coord.second + 1)])
		{
			info_cons info;
			info.lu = r;
			info.l = rl;
			v.push_back(make_pair(make_coord(coord.first + 1, coord.second + 1), info));
		}
		if (layout->graph[make_coord(coord.first + 2, coord.second + 1)])
		{
			info_cons info;
			info.lu = ll;
			info.ru = rl;
			v.push_back(make_pair(make_coord(coord.first + 2, coord.second + 1), info));
		}
		return v;
	}


	Status buildRoad(Builder* player, int address);
	Status buildRes(Builder* player, int address);
	Status improve(Builder* player, int address);
	Status trade(Builder* player1, Builder* player2, resouseType item1, resouseType item2);
	void playerStatus();
	Status diceRoll(int dice);
	void geeseOccur();
	friend std::ostream &operator<<(std::ostream &out, const Board &b);

	virtual	~Board() {}

protected:
	Layout* layout = nullptr;
	std::unordered_set<Building*> buildings;
	std::unordered_set<Builder*> builders;
	std::unordered_set<Tile*> tiles;
	std::unordered_set<Road*> roads;
};


	/*
	Construction Sequence
	
	Let's look at the following simple system
		A
	B		C
		D

	in order to construct D, we need to know all details of intersections in the upper portion of D
	the order is therefore, A->B->C->D

	A asks B to self-construct, then B returns the information about specific elements in the graph
	(which is D in this case)
	similary, C also prepares its information for D
	and then, we finally construct D with information known in the system

	what if the system is larger?
	Consider
			A
		B		C
	D		E		F
		G		H
			I

	the order would be, 
	A->B
	A->C
	B->D
	B->E
	C->F
	D->G
	E->H
	G->I
	at each step, the targeted element already has all information it needs

	the order of construction would be strickly row by row, which ensures all lower-row elements has
	all information they need to self-construct

	the correct data structure would be a queue
	with an unordered_map storing information prepared for each tile
	*/

#endif
