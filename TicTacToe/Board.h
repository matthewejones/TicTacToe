#include<cstdint>

#pragma once
class Board
{
	char chars[3][3];
	char piece;
	int turncounter;
	int* CoordFromPos(uint8_t pos) const;
	void RotateCoord(int * coord);
	void RotateCoord(int * coord, int repeat);
	void UnRotateCoord(int * coord);
	void UnRotateCoord(int * coord, int repeat);
	bool TryCoords(char board[3][3], int x, int y, int i);
	uint16_t GameState(char SearchCharacter, char board[3][3]);
	uint16_t GameState(char SearchCharacter);
	uint16_t GameState(void);
	void Rotate(char in[3][3]);
	int * CalculateMove(char board[3][3]);
	int * CalculateMove();


public:
	Board();
	Board(const Board& board);
	Board& operator=(Board board);
	friend void swap(Board& first, Board& second);
	bool AddPiece(char c, uint8_t x, uint8_t y);
	bool AddPiece(uint8_t x, uint8_t y);
	char GetPiece(uint8_t x, uint8_t y) const;
	bool HasWon(uint16_t GameState) ;
	bool HasWon(void);
	bool TakeComputerMove();
	bool TakeMove();
	void Display(void) const;
	~Board();
};

