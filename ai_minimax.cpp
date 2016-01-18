#include "ai_minimax.h"
#include "rules.h"
#include <limits.h>
#include <iostream>

using namespace std;

struct Board
{
	Piece board[boardLen];
};

struct ChessNode
{
	Board board;
	vector<Point> validMoves;
};

int findBestMove(ChessNode* nodes, int depth, int maxDepth, Point* bestStartPos, Point* bestEndPos, bool isWhiteMove);

int findBestScore(ChessNode* nodes, const Point& start, const Point& end, int depth, int maxDepth, bool isWhiteMove)
{
	nodes[depth].board = nodes[depth-1].board;
	Piece* boardState = nodes[depth].board.board;
	applyMove(boardState, start, end);

	if (depth >= maxDepth)
		return getBoardValue(boardState);
	else
		return findBestMove(nodes, depth, maxDepth, nullptr, nullptr, isWhiteMove);
}

int findBestMove(ChessNode* nodes, int depth, int maxDepth, Point* bestStartPos, Point* bestEndPos, bool isWhiteMove)
{
	Piece* boardState = nodes[depth].board.board;
	int bestValue = INT_MIN;
	for (int i = 0; i<boardLen; ++i) {
		if (boardState[i] == Empty)
			continue;
		if (isWhiteMove != isWhite(boardState[i]))
			continue;
		Point startPos(i%8, i/8);
		nodes[depth].validMoves.clear();

		addPossibleMoves(boardState, startPos, nodes[depth].validMoves);
		for (Point& endPos : nodes[depth].validMoves) {
			int score = findBestScore(nodes, startPos, endPos, depth+1, maxDepth, !isWhiteMove);
			if (score > bestValue) {
				bestValue = score;
				if (bestStartPos != nullptr)
					*bestStartPos = startPos;
				if (bestEndPos != nullptr)
					*bestEndPos = endPos;
			}
		}
	}
	return bestValue;
}

void doMove(Piece* board, BoardState* boardState, bool isWhiteMove)
{
	const int maxDepth = 6;
	ChessNode aiNodes[maxDepth];
	memcpy(&aiNodes[0].board, board, sizeof(Board));

	Point startPos, endPos;
	cout << "Thinking...";
	int score = findBestMove(aiNodes, 0, maxDepth - 1, &startPos, &endPos, false);
	cout << "\rMove score " << score << endl;

	for (int i = 0; i<boardLen; i++)
		boardState[i] = BoardState_Empty;

	if (score > INT_MIN) {
		boardState[startPos.ind] = BoardState_Highlighted;
		boardState[endPos.ind] = BoardState_PossibleMove;
		applyMove(board, startPos, endPos);
	}
}
