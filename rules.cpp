#include "rules.h"
#include "board.h"
#include <iostream>

bool isWhite(Piece p) {
	return p >= WKing && p <= WPawn;
}

bool isBlack(Piece p) {
	return p >= BKing && p <= BPawn;
}

bool bishopCanMove(const Piece* board, const Point& start, const Point& end, const Point& delta, string& errorMessage) 
{
	if (delta.x != delta.y) {
		errorMessage = "Move not diagonal";
		return false;
	}
	int dirx = end.x > start.x ? 1 : -1, diry = end.y > start.y ? 1 : -1;
	for (int i = 1; i<delta.x; i++) {
		int x = start.x + dirx * i, y = start.y + diry * i;
		if (board[ix(x, y)] != Empty) {
			errorMessage = "Piece in the way";
			return false;
		}
	}
	return true;
}

bool rookCanMove(const Piece* board, const Point& start, const Point& end, const Point& delta, string& errorMessage)
{
	if (!((delta.x == 0 && delta.y != 0) || (delta.y == 0 && delta.x != 0))) {
		errorMessage = "Move not horizontal";
		return false;
	}
	int dirx = delta.x != 0 ? (end.x > start.x ? 1 : -1) : 0;
	int diry = delta.y != 0 ? (end.y > start.y ? 1 : -1) : 0;
	int steps = max(abs(delta.x), abs(delta.y));
	for (int i = 1; i<steps; ++i) {
		int x = start.x + dirx * i, y = start.y + diry * i;
		if (board[ix(x, y)] != Empty) {
			errorMessage = "Piece in the way";
			return false;
		}
	}
	return true;
}

bool canMove(const Piece* board, const string& move, Point& start, Point& end, string& errorMessage)
{
	if (move.size() != 5 || move[2] != ' ') {
		errorMessage = "Invalid move command expected B2 C2\n";
		return false;
	}
	if (!Point::Parse(move[0], move[1], start)) {
		errorMessage = "Invalid start position " + move.substr(0, 2);
		return false;
	}
	if (!Point::Parse(move[3], move[4], end)) {
		errorMessage = "Invalid end position " + move.substr(3, 2);
		return false;
	}

	Point delta = {
		abs(end.x - start.x),
		abs(end.y - start.y),
		0
	};
	Piece type = board[start.ind];
	Piece target = board[end.ind];
	if ((isBlack(type) && isBlack(target)) || (isWhite(type) && isWhite(target))) {
		errorMessage = "Cannot take own piece";
		return false;
	}

	switch (type) {
		case Empty:
			errorMessage = "Starting position empty";
			return false;
		case WKnight:
		case BKnight:
			if ((delta.x == 1 && delta.y == 2) || (delta.x == 2 && delta.y == 1))
				return true;

			errorMessage = "Invalid knight move";
			return false;
		case WBishop:
		case BBishop:
			return bishopCanMove(board, start, end, delta, errorMessage);
		case WRook:
		case BRook:
			return rookCanMove(board, start, end, delta, errorMessage);
		case WQueen:
		case BQueen:
			if (bishopCanMove(board, start, end, delta, errorMessage) || rookCanMove(board, start, end, delta, errorMessage))
				return true;
			errorMessage = "Invalid queen move";
			return false;
		case WPawn:
		case BPawn:
			if ((type == WPawn && end.y < start.y) || (type == BPawn && end.y > start.y)) {
				if (delta.x == 0 && delta.y == 1 && target == Empty)
					return true;
				if (delta.x == 1 && delta.y == 1 && target != Empty)
					return true;
				if (delta.x == 0 && delta.y == 2 && start.y == (isWhite(type) ? 6 : 1) && target == Empty)
					return true;
				//'en-passant' is not supported >:)
			}
			errorMessage = "Invalid Pawn move 2";
			return false;
		case WKing:
		case BKing:
			if (delta.x != 1 && delta.y != 1) {
				errorMessage = "King can only move 1 square";
				return false;
			}
			if (!bishopCanMove(board, start, end, delta, errorMessage) && !rookCanMove(board, start, end, delta, errorMessage)) {
				errorMessage = "Invalid king move";
				return false;
			}
			//TODO: king cannot move into check checks
			return true;
	}
}

bool applyMove(Piece* board, const string& move, string& errorMessage)
{
	Point start, end;
	if (canMove(board, move, start, end, errorMessage))
	{
		board[end.ind] = board[start.ind];
		board[start.ind] = Empty;

		//Promote pawns that reach the map edge
		if (board[end.ind] == BPawn && end.y == 7) {
			board[end.ind] = BQueen;
		}
		if (board[end.ind] == WPawn && end.y == 0) {
			board[end.ind] = WQueen;
		}
		return true;
	}
	else
		return false;
}
