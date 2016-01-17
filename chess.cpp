#include <iostream>
#include "rules.h"
#include "board.h"

using namespace std;

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

char kbhit()
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

enum KeyPress : int
{
	Key_Up = 65,
	Key_Down = 66,
	Key_Right = 67,
	Key_Left = 68,
	Key_Enter = 10,
	Key_Space = 32
};

KeyPress getInput()
{
	while (true) {
		if (kbhit()) {
			char c = getchar();
			if (c >= Key_Up && c <= Key_Left)
				return (KeyPress)c;
			else if (c == Key_Enter || c == Key_Space)
				return (KeyPress)c;
		}
	}
}

int main(int argc, char** argv)
{
	Piece board[] = {
		BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
		BPawn, BPawn,   BPawn,   BPawn,  BPawn, BPawn,   BPawn,   BPawn,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   WBishop,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		WPawn, WPawn,   WPawn,   WPawn,  WPawn, WPawn,   WPawn,   WPawn,
		WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook
	};
	BoardState boardState[8*8] = { };

	vector<Point> validMoves;
	Point sourcePos = {};
	bool sourceSelected = false;
	Point highlightPos = Point(4, 4);
	boardState[highlightPos.ind] = BoardState_Highlighted;

	while (true) {
		printBoard(board, boardState);

		KeyPress input = getInput();
		switch (input) {
			case Key_Up:
				highlightPos = Point(highlightPos.x, max(highlightPos.y - 1, 0));
				break;
			case Key_Down:
				highlightPos = Point(highlightPos.x, min(highlightPos.y + 1, 7));
				break;
			case Key_Left:
				highlightPos = Point(max(highlightPos.x - 1, 0), highlightPos.y);
				break;
			case Key_Right:
				highlightPos = Point(min(highlightPos.x + 1, 7), highlightPos.y);
				break;
			default:
				break;
		}
		if (input == Key_Enter || input == Key_Space) {
			if (!sourceSelected) {
				if (board[highlightPos.ind] != Empty) {
					sourceSelected = true;
					sourcePos = highlightPos;
				}
			} else {
				sourceSelected = false;
				for (Point move : validMoves) {
					if (highlightPos.ind == move.ind) {
						applyMove(board, sourcePos, highlightPos);
						break;
					}
				}
			}
		}

		for (int i = 0; i<boardLen; ++i)
			boardState[i] = BoardState_Empty;
		if (!sourceSelected) {
			validMoves.clear();
			addPossibleMoves(board, highlightPos, validMoves);
		}

		for (Point pos : validMoves) {
			boardState[pos.ind] = sourceSelected ? BoardState_PossibleMove : BoardState_Selected;
		}
		if (sourceSelected)
			boardState[sourcePos.ind] = BoardState_Selected;
		boardState[highlightPos.ind] = BoardState_Highlighted;

/*		bool hasValidMove = false;
		while (!hasValidMove) {
			cout << "Enter Move: ";
			getline(cin, input);
			hasValidMove = applyMove(board, input, errorMessage);
			if (!hasValidMove) {
				cout << errorMessage << endl;
			}
		}*/
	}
	return 0;
}
