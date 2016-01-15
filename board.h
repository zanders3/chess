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

const int boardLen = 8*8;

void printBoard(const Piece* board);
