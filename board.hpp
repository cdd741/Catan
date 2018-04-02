#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "address.hpp"
#include "builder.hpp"

#include "status.hpp"
#include "resource.hpp"
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
	Layout(int nRows) : nRows{ nRows } { row_ct = new int[nRows] { 1, 2, 3, 2, 3, 2, 3, 2, 1 }; }
	Layout(std::istream& in, int nRows) : Layout(nRows) { load(in); }
	virtual ~Layout() { delete row_ct; }
public:
	int* row_ct;
	int nRows;
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

class TerminalGrid
{
	std::map<size_t, char*> grid;
public:
	std::unordered_map<void*, Coordinate2D> desired;

	const size_t terminalWidth;

	friend TerminalGrid& operator<<(TerminalGrid& out, const Tile& tile)
	{
		auto desired = out.desired[(void*)&tile];
		assert(desired == make_coord(-1, -1));
		std::stringstream ss;
		ss << *tile.info.lu << "--" << tile.info.lu->isConnected(tile.info.ru) << "--" << *tile.info.ru;
		out.setLocation(desired);
		out << ss.str();

		out.setLocation(make_coord(desired.first + 1, desired.second));
		out << " /          \\ ";

		out.setLocation(make_coord(desired.first + 2, desired.second - 1));
		out << tile.info.lu->isConnected(tile.info.l) << "           " << tile.info.ru->isConnected(tile.info.r);

		out.setLocation(make_coord(desired.first + 3, desired.second - 1));
		out << "/            \\";
        
        	out.setLocation(make_coord(desired.first + 4, desired.second - 2));
        	ss << *tile.info.l << "           " << *tile.info.r;
        
        	out.setLocation(make_coord(desired.first + 5, desired.second - 1));
        	out << "\\            /";
        
        	out.setLocation(make_coord(desired.first + 6, desired.second - 1));
        	out << tile.info.ll->isConnected(tile.info.l) << "           " << tile.info.rl->isConnected(tile.info.r);
        
        	out.setLocation(make_coord(desired.first + 7, desired.second));
        	out << " \\          / ";
     
        	out.setLocation(make_coord(desired.first + 8, desired.second));
        	ss << *tile.info.ll << "--" << tile.info.ll->isConnected(tile.info.rl) << "--" << *tile.info.rl;


		// since this is a grid, overwritting at the same coordinate does not affect final output
		// doing so prevents possibilities of missing a road/node in the relative directions
		// TODO: add the next a few lines

		if (tile.info.ll)
		{
			out.desired[tile.info.ll] = make_coord(desired.first + 4, desired.second - 12);
			out << *tile.info.ll;
		}

		if (tile.info.rl)
		{
			out.desired[tile.info.rl] = make_coord(desired.first + 4, desired.second + 13);
			out << *tile.info.rl;
		}
		return out;
	}

	TerminalGrid(size_t width) : terminalWidth{ width }
	{

	}

	virtual ~TerminalGrid()
	{
		for (auto & p : grid)
			delete p.second;
	}

	Coordinate2D location(void* object) const
	{
		if (desired.count(object))
			return make_coord(-1, -1);	// no desired location proposed, unexpected
		return desired.at(object);
	}

	char* &operator[](const size_t& key)
	{
		if (grid[key] == nullptr)
			return grid[key] = new char[terminalWidth];
	}

	Coordinate2D _location;
	void setLocation(const Coordinate2D& loc)
	{
		_location = loc;
	}

	friend TerminalGrid& operator<<(TerminalGrid& out, const std::string& s)
	{
		assert(out._location != make_coord(-1, -1));
		for (auto i = 0; i < s.size(); i++)
		{
			assert(out._location.second + i < out.terminalWidth);
			out[out._location.first][out._location.second + i] = s[i];
		}
		out._location.second += s.size();
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, const TerminalGrid& grid)
	{
		size_t line = 0;
		for (auto & p : grid.grid)
		{
			for (; line < p.first; line++)
				out << "\n";
			if (p.second)
				out << p.second;
			out << "\n";
			line++;
		}
		return out;
	}
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

		layout->graph[make_coord(0, 0)]->info = info_cons{ l, lu, ll, r, ru, rl };

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

		layout->graph[coord]->info = info_cons{ l, lu, ll, r, ru, rl  };

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


	void assignUIIndexes(std::ostream* pOut = nullptr)
	{
		assert(layout);

		// the graph is always set to start from (0, 0)

		auto index = 0;

		// scanner line method
		// let's assign indexes as they are drawn in place
		

		// we start from the 0th line, (0, 0),
		// the top most line is the 1st scanner line

#define exists(x, y) (bool)(layout->graph[make_coord(x, y)])

		int r_ct[]{ 1,2,3,4,5,6,7,8 };

		std::vector<std::vector<Building*>> line_layout;
		line_layout.resize(layout->nRows + 2);

		for (auto row_idx = 0; row_idx < layout->nRows; row_idx++)
		{
			size_t max_ct = row_idx + 1;
			size_t index_cp = index;
			for (auto i = 0; i < layout->row_ct[row_idx]; i++)
			{
				auto base = (max_ct - layout->row_ct[row_idx]) / 2;

				auto x = row_idx;
				auto y = base + i;

				if (!exists(x, y - 1) && exists(x - 2, y - 2))
					addr_map[index++] = layout->graph[{x - 2, y - 2}]->info.ll;
				if (!exists(x, y - 1) && exists(x - 1, y - 1))
					addr_map[index++] = layout->graph[make_coord(x - 1, y - 1)]->info.l;
				
				addr_map[index++] = layout->graph[make_coord(x, y)]->info.lu;
				addr_map[index++] = layout->graph[make_coord(x, y)]->info.ru;

				if (!exists(x, y + 1) && exists(x - 1, y))
					addr_map[index++] = layout->graph[make_coord(x - 1, y)]->info.r;
				if (!exists(x, y + 1) && exists(x - 2, y))
					addr_map[index++] = layout->graph[make_coord(x - 2, y)]->info.rl;
			}

			for (auto i = index_cp; i < index; i++)
				line_layout[row_idx].push_back(addr_map[i]);
		}

		for (auto x = layout->nRows - 2; x < layout->nRows; x++)
		{
			for (auto i = 0; i < layout->row_ct[x]; i++)
			{
				auto base = (x + 1 - layout->row_ct[x]) / 2;
				auto y = base + i;
				addr_map[index++] = layout->graph[make_coord(x, y)]->info.ll;
				line_layout[x+2].push_back(addr_map[index - 1]);
				addr_map[index++] = layout->graph[make_coord(x, y)]->info.rl;
				line_layout[x+2].push_back(addr_map[index - 1]);
			}
		}

		index = 0;
		auto k = layout->graph[make_coord(6, 2)];
		for (auto i = 0; i < line_layout.size() - 1; i++)
		{
			for (auto j = 0; j < line_layout[i].size()-1; j++)
			{
				if (line_layout[i][j]->isConnected(line_layout[i][j + 1]))
					road_map[index++] = line_layout[i][j]->isConnected(line_layout[i][j + 1]);
			}
			
			for (auto j = 0; j < line_layout[i].size(); j++)
			{
				for (auto k = 0; k < line_layout[i + 1].size(); k++)
				{
					if (line_layout[i][j]->isConnected(line_layout[i+1][k]))
					{
						line_layout[i][j]->isConnected(line_layout[i + 1][k])->ID = index;
						road_map[index++] = line_layout[i][j]->isConnected(line_layout[i + 1][k]);
						
						// in the given case, only connected once
						break;
					}
				}
			}
		}


		auto i = line_layout.size() - 1;
		for (auto j = 0; j < line_layout[i].size() - 1; j++)
		{
			if (line_layout[i][j]->isConnected(line_layout[i][j + 1]))
				road_map[index++] = line_layout[i][j]->isConnected(line_layout[i][j + 1]);
		}
		

		if (pOut)
		{
			std::ostream & out = *pOut;

			TerminalGrid terminal(80);
			terminal.desired[layout->graph[make_coord(0, 0)]] = make_coord(0, 33);
			terminal << *layout->graph[make_coord(0, 0)];
		}
	}
	
	friend std::ostream &operator<<(std::ostream &out, const Board &b)
	{
		return out;
	}
	
	void movingGeese(int tileidx);
	Status buildRoad(Builder* player, int address);
	Status buildRes(Builder* player, int address);
	Status improve(Builder* player, int address);
	Status trade(Builder* player1, Builder* player2, resourceType item1, resourceType item2);
	void playerStatus();
	Status diceRoll(int dice);
	void geeseOccur();
	friend std::ostream &operator<<(std::ostream &out, const Board &b);


	virtual	~Board() {}

protected:
	Layout* layout = nullptr;

	Tile* Geese = nullptr;
	std::unordered_set<Building*> buildings;	// for releasing

	std::unordered_map<unsigned int, Building*> addr_map;
	std::unordered_map<unsigned int, Road*> road_map;

	std::unordered_map<unsigned int, Builder*> builders;
	std::unordered_map<unsigned int, Tile*> tiles;
	std::unordered_map<unsigned int, Road*> roads;


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
