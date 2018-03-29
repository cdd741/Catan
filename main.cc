#include "board.hpp"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream in("layout.txt");
	Board board(new Layout(in, 9));

	return 0;
}
