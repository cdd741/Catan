#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_map>
#include <queue>

#include "address.hpp"

class Builder;
class Tile;

struct pairhash
{
public:
	template <typename T, typename U>
	std::size_t operator()(const std::pair<T, U> &x) const
	{
		// possible collisions when x coord > 10000, however, rare in this case
		return std::hash<T>()(x.first) + 10000 * std::hash<U>()(x.second);
	}
};

typedef std::pair<int, int> Coordinate2D;
#define make_coord(x, y) std::make_pair(x, y)
typedef std::unordered_map<Coordinate2D, Tile*, pairhash> Graph;

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
	virtual void construct() = 0;

	virtual	~Board() {}

protected:
	Layout* layout = nullptr;
	std::unordered_set<Address*> addresses;

};

class HexBoard : public Board
{
	using Board::Board;
	
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
	void construct() override
	{
		
	}

	void _construct(Address* a, Address * b)
	{

	}

};

#endif
