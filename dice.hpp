#pragma once
#ifndef DICE_H
#define DICE_H

#include <random>

class Dice
{
public:
	virtual unsigned int roll() const = 0;

protected:
	// lower and upper are both INCLUSIVE
	// random number generation utility
	static unsigned int ranged_rand(unsigned int lower, unsigned int upper)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(lower, upper);
		return distr(eng);
	}
};

// a basic dice produces a random unsigned integer within the range every time
class FairDice : public Dice
{
public:
	unsigned int roll() const override
	{
		// rows twice
		return ranged_rand(1, 6) + ranged_rand(1, 6);
	}
};

// a loaded dice produces the same value every time

// note: the dice itself doesn't care whether or not a value is valid
// since such criterias change very regularly
// (suppose you changed to a wider range)
class LoadedDice : public Dice
{
	unsigned int value;
public:
	LoadedDice(unsigned int value) : value{ value } {}

public:
	unsigned int roll() const override
	{
		return value;
	}
};

#endif
