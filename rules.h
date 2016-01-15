#pragma once

#include <string>
#include "board.h"

using namespace std;

static int ix(int x, int y) {
	return y * 8 + x;
}

struct Point
{
	int x, y, ind;

	static bool Parse(char yMove, char xMove, Point& p) {
		if (xMove < '1' || xMove > '8')
			return false;
		p.x = (int)(xMove - '1');
		if (yMove < 'A' || yMove > 'I')
			return false;
		p.y = (int)(yMove - 'A');
		p.ind = ix(p.x, p.y);
		return true;
	}
};

bool canMove(const Piece* board, const string& move, Point& start, Point& end, string& errorMessage);
bool applyMove(Piece* board, const string& move, string& errorMessage);
