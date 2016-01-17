#pragma once

enum Piece
{
	WKing,
	WQueen,
	WRook,
	WBishop,
	WKnight,
	WPawn,
	BKing,
	BQueen,
	BRook,
	BBishop,
	BKnight,
	BPawn,
	Empty,
};

enum BoardState
{
	BoardState_Empty,
	BoardState_Highlighted,
	BoardState_PossibleMove,
	BoardState_Selected
};

static int ix(int x, int y) {
	return y * 8 + x;
}

struct Point
{
	int x, y, ind;

	Point(int x, int y) : x(x), y(y), ind(ix(x, y)) {}
	Point() {}

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

const int boardLen = 8*8;

void printBoard(const Piece* board, const BoardState* boardState);
