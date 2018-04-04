#include "dice.hpp"

 std::mt19937 Dice::eng(std::random_device{}());
//std::mt19937 Dice::eng(0);	// use this for valgrind as it does not support rd inst.
