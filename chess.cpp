#include <iostream>
#include "rules.h"
#include "board.h"

using namespace std;

int main(int argc, char** argv)
{
	Piece board[] = {
		BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
		BPawn, BPawn,   BPawn,   BPawn,  BPawn, BPawn,   BPawn,   BPawn,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   WKing,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		WPawn, WPawn,   WPawn,   WPawn,  WPawn, WPawn,   WPawn,   WPawn,
		WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook
	};
	string input;
	string errorMessage;
	while (true) {
		printBoard(board);
		bool hasValidMove = false;
		while (!hasValidMove) {
			cout << "Enter Move: ";
			getline(cin, input);
			hasValidMove = applyMove(board, input, errorMessage);
			if (!hasValidMove) {
				cout << errorMessage << endl;
			}
		}
	}
	return 0;
}
