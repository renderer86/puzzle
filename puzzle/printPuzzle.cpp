#include "printPuzzle.h"

PrintPuzzle::PrintPuzzle()
{
	for (int x = 0; x < PUZZLE_WIDTH; ++x)
	{
		for (int y = 0; y < PUZZLE_HEIGHT; ++y)
		{
			tile[x][y] = 'бр';
		}
	}
}