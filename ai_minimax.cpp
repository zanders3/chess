#include "ai_minimax.h"
#include "rules.h"
#include <limits.h>
#include <iostream>

using namespace std;

struct ChessNode
{
	static ChessNode* Create(const Piece* board, const Point& start, const Point& end, bool isWhiteMove, int maxDepth)
	{
		ChessNode* node = new ChessNode();
		memcpy(node->boardState, board, sizeof(Piece) * boardLen);
		applyMove(node->boardState, start, end);
		node->isWhiteMove = isWhiteMove;
		node->startPos = start;
		node->endPos = end;

		if (maxDepth > 0)
			node->expandChildren(maxDepth);

		return node;
	}

	void expandChildren(int maxDepth) {
		vector<Point> validMoves;
		for (int i = 0; i<boardLen; ++i) {
			if (boardState[i] == Empty)
				continue;
			if (isWhiteMove != isWhite(boardState[i]))
				continue;
			Point startPos(i%8, i/8);
			validMoves.clear();
			addPossibleMoves(boardState, startPos, validMoves);
			for (Point& endPos : validMoves) {
				children.push_back(ChessNode::Create(boardState, startPos, endPos, !isWhiteMove, maxDepth - 1));
			}
		}
	}

	int getMaxScore() {
		if (children.size() == 0)
			return getBoardValue(boardState);
		
		int bestValue = INT_MIN;
		for (ChessNode* child : children)
			bestValue = max(bestValue, child->getMaxScore());
		return bestValue;
	}

	vector<ChessNode*> children;
	Piece boardState[boardLen];
	Point startPos, endPos;
	bool isWhiteMove;
};

void doMove(Piece* board, bool isWhiteMove)
{
	cout << "Thinking..." << endl;
	ChessNode chessNode;
	memcpy(chessNode.boardState, board, sizeof(Piece) * boardLen);
	chessNode.isWhiteMove = isWhiteMove;
	chessNode.expandChildren(5);

	cout << "Finding best move..." << endl;
	ChessNode* bestMove = nullptr;
	int bestScore = INT_MIN;
	for (ChessNode* child : chessNode.children) {
		int score = child->getMaxScore();
		if (score > bestScore) {
			bestMove = child;
			bestScore = score;
		}
	}
	cout << "Best move " << bestScore << endl;
	if (bestMove != nullptr)
		applyMove(board, bestMove->startPos, bestMove->endPos);
}
