all:
	g++ -O3 -Wall -Werror chess.cpp board.cpp rules.cpp ai_minimax.cpp -std=c++11 -o chess
