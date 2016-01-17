#pragma once

#include <string>
#include <vector>
#include "board.h"

using namespace std;

void addPossibleMoves(const Piece* board, const Point& pos, vector<Point>& moves);

bool applyMove(Piece* board, const Point& start, const Point& end);
