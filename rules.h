#pragma once

#include <string>
#include <vector>
#include "board.h"

using namespace std;

inline static bool isWhite(Piece p) {
	return p >= WKing && p <= WPawn;
}

void addPossibleMoves(const Piece* board, const Point& pos, vector<Point>& moves);
void applyMove(Piece* board, const Point& start, const Point& end);
int getBoardValue(const Piece* board);
