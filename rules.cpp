#include "rules.h"
#include "board.h"
#include <iostream>
#include <vector>
#include <algorithm>

bool isWhite(Piece p) {
	return p >= WKing && p <= WPawn;
}

void getRookMoves(const Piece* board, const Point& pos, vector<Point>& moves)
{
	Point target;
	for (int x = pos.x+1; x<8; ++x) {
		target = Point(x, pos.y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int x = pos.x-1; x>=0; --x) {
		target = Point(x, pos.y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int y = pos.y+1; y<8; ++y) {
		target = Point(pos.x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int y = pos.y-1; y>=0; --y) {
		target = Point(pos.x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
}

void getBishopMoves(const Piece* board, const Point& pos, vector<Point>& moves)
{
	for (int x = pos.x+1, y = pos.y+1; x<8 && y<8; ++x, ++y) {
		Point target(x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int x = pos.x+1, y = pos.y-1; x<8 && y>0; ++x, --y) {
		Point target(x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int x = pos.x-1, y = pos.y-1; x>=0 && y>=0; --x, --y) {
		Point target(x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
	for (int x = pos.x-1, y = pos.y+1; x>=0 && y<8; --x, ++y) {
		Point target(x, y);
		moves.push_back(target);
		if (board[target.ind] != Empty)
			break;
	}
}

void addPossibleMoves(const Piece* board, const Point& pos, vector<Point>& moves)
{
	Piece type = board[pos.ind];
	bool pieceIsWhite = isWhite(type);
	switch (type)
	{
		case BKnight:
		case WKnight:
		{
			moves.push_back(Point(pos.x + 1, pos.y + 2));
			moves.push_back(Point(pos.x - 1, pos.y + 2));
			moves.push_back(Point(pos.x + 2, pos.y + 1));
			moves.push_back(Point(pos.x - 2, pos.y + 1));
			moves.push_back(Point(pos.x + 1, pos.y - 2));
			moves.push_back(Point(pos.x - 1, pos.y - 2));
			moves.push_back(Point(pos.x + 2, pos.y - 1));
			moves.push_back(Point(pos.x - 2, pos.y - 1));
		}
			break;
		case BRook:
		case WRook:
		{
			getRookMoves(board, pos, moves);
		}
			break;
		case BBishop:
		case WBishop:
		{
			getBishopMoves(board, pos, moves);
		}
			break;
		case WQueen:
		case BQueen:
		{
			getRookMoves(board, pos, moves);
			getBishopMoves(board, pos, moves);
		}
			break;
		case WPawn:
		case BPawn:
		{
			int dir = type == BPawn ? 1 : -1;
			Point next(pos.x, pos.y + dir);
			if (next.y >= 0 && next.y < 8 && board[next.ind] == Empty) {
				moves.push_back(next);
				Point fwd = Point(pos.x, pos.y+(dir*2));
				if (((pos.y == 1 && type == BPawn) || (pos.y == 6 && type == WPawn)) && 
					board[fwd.ind] == Empty)
					moves.push_back(fwd);
			}
			Point right = Point(pos.x+1, pos.y+dir);
			if (right.y >= 0 && right.y < 8 && right.x >= 0 && right.x < 8 && board[right.ind] != Empty && isWhite(board[right.ind]) != pieceIsWhite)
				moves.push_back(right);
			Point left = Point(pos.x-1, pos.y+dir);
			if (left.y >= 0 && left.y < 8 && left.x >= 0 && left.x < 8 && board[left.ind] != Empty && isWhite(board[left.ind]) != pieceIsWhite)
				moves.push_back(left);
		}
			break;
		case WKing:
		case BKing:
		{
			moves.push_back(Point(pos.x + 1, pos.y));
			moves.push_back(Point(pos.x - 1, pos.y));
			moves.push_back(Point(pos.x + 1, pos.y - 1));
			moves.push_back(Point(pos.x,     pos.y - 1));
			moves.push_back(Point(pos.x - 1, pos.y - 1));
			moves.push_back(Point(pos.x + 1, pos.y + 1));
			moves.push_back(Point(pos.x,     pos.y + 1));
			moves.push_back(Point(pos.x - 1, pos.y + 1));
		}
			break;
		case Empty:
			break;
	}

	moves.erase(
		remove_if(moves.begin(), moves.end(), 
			[board,pieceIsWhite](const Point& p) { return !(p.x >= 0 && p.x < 8 && p.y >= 0 && p.y < 8 && (isWhite(board[p.ind]) != pieceIsWhite || board[p.ind] == Empty)); }
		),
		moves.end()
	);
}

bool applyMove(Piece* board, const Point& start, const Point& end)
{
	vector<Point> moves;
	addPossibleMoves(board, start, moves);
	for (Point& p : moves)
	{
		if (p.ind == end.ind) {
			board[end.ind] = board[start.ind];
			board[start.ind] = Empty;
			return true;
		}
	}
	return false;
}
