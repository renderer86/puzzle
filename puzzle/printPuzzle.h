#pragma once

const int PUZZLE_WIDTH = 9;
const int PUZZLE_HEIGHT = 9;

class PrintPuzzle
{
public:
	PrintPuzzle();

private:
	char tile[PUZZLE_WIDTH][PUZZLE_HEIGHT];
};