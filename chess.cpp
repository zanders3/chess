#include <iostream>
#include "rules.h"
#include "board.h"
#include "ai_minimax.h"

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

struct GameState
{
	Piece* board;
	BoardState* boardState;
	Point sourcePos, highlightPos;
	vector<Point> validMoves;
	bool sourceSelected, playerTurn;
};

void handlePlayerInput(GameState& gameState)
{
	KeyPress input = getInput();
	switch (input) {
		case Key_Up:
			gameState.highlightPos = Point(gameState.highlightPos.x, max(gameState.highlightPos.y - 1, 0));
			break;
		case Key_Down:
			gameState.highlightPos = Point(gameState.highlightPos.x, min(gameState.highlightPos.y + 1, 7));
			break;
		case Key_Left:
			gameState.highlightPos = Point(max(gameState.highlightPos.x - 1, 0), gameState.highlightPos.y);
			break;
		case Key_Right:
			gameState.highlightPos = Point(min(gameState.highlightPos.x + 1, 7), gameState.highlightPos.y);
			break;
		default:
			break;
	}
	if (input == Key_Enter || input == Key_Space) {
		if (!gameState.sourceSelected) {
			if (gameState.board[gameState.highlightPos.ind] != Empty) {
				gameState.sourceSelected = true;
				gameState.sourcePos = gameState.highlightPos;
			}
		} else {
			gameState.sourceSelected = false;
			for (Point move : gameState.validMoves) {
				if (gameState.highlightPos.ind == move.ind) {
					applyMove(gameState.board, gameState.sourcePos, gameState.highlightPos);
					gameState.playerTurn = false;
					break;
				}
			}
		}
	}

	for (int i = 0; i<boardLen; ++i)
		gameState.boardState[i] = BoardState_Empty;
	if (!gameState.sourceSelected) {
		gameState.validMoves.clear();
		addPossibleMoves(gameState.board, gameState.highlightPos, gameState.validMoves);
	}

	for (Point pos : gameState.validMoves) {
		gameState.boardState[pos.ind] = gameState.sourceSelected ? BoardState_PossibleMove : BoardState_Selected;
	}
	if (gameState.sourceSelected)
		gameState.boardState[gameState.sourcePos.ind] = BoardState_Selected;
	gameState.boardState[gameState.highlightPos.ind] = BoardState_Highlighted;
}

int main(int argc, char** argv)
{
	Piece board[] = {
		BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
		BPawn, BPawn,   BPawn,   BPawn,  BPawn, BPawn,   BPawn,   BPawn,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		Empty, Empty,   Empty,   Empty,  Empty, Empty,   Empty,   Empty,
		WPawn, WPawn,   WPawn,   WPawn,  WPawn, WPawn,   WPawn,   WPawn,
		WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook
	};
	BoardState boardState[8*8] = { };
	GameState gameState = {};
	gameState.board = board;
	gameState.boardState = boardState;
	gameState.sourcePos = {};
	gameState.highlightPos = Point(4, 4);
	gameState.sourceSelected = false;
	gameState.playerTurn = true;

	gameState.boardState[gameState.highlightPos.ind] = BoardState_Highlighted;

	while (true) {
		printBoard(gameState.board, gameState.boardState);

		if (gameState.playerTurn)
			handlePlayerInput(gameState);
		else
		{
			doMove(board, false);
			gameState.playerTurn = true;
		}
	}
	return 0;
}
