#include "board.h"
#include <iostream>
using namespace std;

char const *pieceChars[] = {
	"\u2654", "\u2655", "\u2656", "\u2657", 
	"\u2658", "\u2659", "\u265A", "\u265B",
	"\u265C", "\u265D", "\u265E", "\u265F",
	" "
};

void printBoard(const Piece* board, const BoardState* boardState)
{
	cout << "\r";
	for (int i = 0; i<boardLen; ++i) {
		int x = i%8, y = i/8;

		if (x == 0)
			cout << (char)(y+'A') << " ";

		bool isBlackSq = x % 2 == 0;
		if (y % 2 == 0)
			isBlackSq = !isBlackSq;

		switch (boardState[i])
		{
			case BoardState_Highlighted:
				cout << "\x1b[0;44m";
				break;
			case BoardState_PossibleMove:
				cout << "\x1b[0;42m";
				break;
			case BoardState_Selected:
				cout << "\x1b[0;43m";
				break;
			case BoardState_Empty:
				cout << (isBlackSq ? "\x1b[0;47m" : "\x1b[0m");
				break;
		}

		cout << pieceChars[board[i]] << " ";
		if (x == 7)
			cout << "\x1b[0m" << endl;
	}
	cout << "\x1b[0m  1 2 3 4 5 6 7 8 " << endl;
}
