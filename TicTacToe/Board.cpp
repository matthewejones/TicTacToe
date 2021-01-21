#include "pch.h"
#include "Board.h"
#include <iostream>

int* Board::CoordFromPos(uint8_t pos) const
{
	int *ret = new int[2];
	ret[0] = (pos - 1) % 3;
	ret[1] = (pos - 1 - ret[0]) / 3;
	return ret;
}

void Board::RotateCoord(int * coord)
{
	int y;
	y = 2 - coord[0];
	coord[0] = coord[1];
	coord[1] = y;
}

void Board::RotateCoord(int * coord, int repeat)
{
	for (int i = 0; i < repeat; i++)
	{
		RotateCoord(coord);
	}
}

void Board::UnRotateCoord(int * coord)
{
	int x;
	x = 2 - coord[1];
	coord[1] = coord[0];
	coord[0] = x;
}

void Board::UnRotateCoord(int * coord, int repeat)
{
	for (int i = 0; i < repeat; i++)
	{
		UnRotateCoord(coord);
	}
}

bool Board::TryCoords(char board[3][3], int x, int y, int i)
{
	int coords[2] = { x, y };
	RotateCoord(coords, i);
	return board[coords[1]][coords[0]] == ' ';
}

uint16_t Board::GameState(char SearchCharacter, char board[3][3])
{
	uint16_t ret = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == SearchCharacter)
			{
				ret |= (1 << (j + i * 3));
			}
		}
	}
	return ret;
}

uint16_t Board::GameState(char SearchCharacter)
{
	return GameState(SearchCharacter, chars);
}

uint16_t Board::GameState(void)
{
	return GameState(piece);
}

void Board::Rotate(char in[3][3])
{
	char ret[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ret[i][j] = in[i][j];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			in[i][j] = ret[2 - j][i];
		}
	}
}

int * Board::CalculateMove(char boardin[3][3])
{
	//copy board
	char board[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = boardin[i][j];
		}
	}
	//Four rotations of state of game
	uint16_t gamestates[4];
	
	for (int i = 0; i < 4; i++)
	{
		gamestates[i] = GameState(piece, board);
		Rotate(board);
	}

	char notpiece = piece == 'o' ? 'x' : 'o';

	//Gamestates of other piece
	uint16_t notgamestates[4];

	for (int i = 0; i < 4; i++)
	{
		notgamestates[i] = GameState(notpiece, board);
		Rotate(board);
	}


	//return value
	int * coords = new int[2];
	coords[0] = 0;
	coords[1] = 0;


	//win
	for (int i = 0; i < 4; i++)
		{
		if ((((gamestates[i] & 6) == 6) || ((gamestates[i] & 72) == 72) || 
			((gamestates[i] & 272) == 272)) && TryCoords(board, 0, 0, i))
		{
			coords[0] = 0;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if ((((gamestates[i] & 5) == 5) || ((gamestates[i] & 144) == 144)) && TryCoords(board, 1, 0, i))
		{
			coords[0] = 1;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if ((((gamestates[i] & 257) == 257) || ((gamestates[i] & 40) == 40)) && TryCoords(board, 1, 1, i))
		{
			coords[0] = 1;
			coords[1] = 1;
			RotateCoord(coords, i);
			return coords;
		}

	}

	//block
	for (int i = 0; i < 4; i++)
	{
		if ((((notgamestates[i] & 6) == 6) || ((notgamestates[i] & 72) == 72) ||
			((notgamestates[i] & 272) == 272)) && TryCoords(board, 0, 0, i))
		{
			coords[0] = 0;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if ((((notgamestates[i] & 5) == 5) || ((notgamestates[i] & 144) == 144)) && TryCoords(board, 1, 0, i))
		{
			coords[0] = 1;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if ((((notgamestates[i] & 257) == 257) || ((notgamestates[i] & 40) == 40)) && TryCoords(board, 1, 1, i))
		{
			std::cout << TryCoords(board, 1, 1, i);

			coords[0] = 1;
			coords[1] = 1;
			RotateCoord(coords, i);
			return coords;
		}

	}
	//fork?
	//blockfork?
	for (int i = 0; i < 4; i++)
	{
		if ( ((notgamestates[i] & 17) == 17) && 
			TryCoords(board, 2, 0, i) && TryCoords(board, 1, 0, i)  && (TryCoords(board, 2, 2, i) || TryCoords(board, 0, 2, i)) )
		{
			coords[0] = 2;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if (((notgamestates[i] & 3) == 3) && TryCoords(board, 2, 0, i) && TryCoords(board, 0, 2, i) && TryCoords(board, 0, 1, i))
		{
			coords[0] = 0;
			coords[1] = 1;
			RotateCoord(coords, i);
			return coords;
		}
		else if (((notgamestates[i] & 9) == 9) && TryCoords(board, 2, 0, i) && TryCoords(board, 0, 2, i) && TryCoords(board, 1, 0, i))
		{
			coords[0] = 1;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if (((notgamestates[i] & 10) == 10) && TryCoords(board, 2, 0, i) && TryCoords(board, 0, 2, i) && TryCoords(board, 0, 0, i))
		{
			coords[0] = 0;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
		else if (((notgamestates[i] & 257) == 257) && TryCoords(board, 1, 0, i) && TryCoords(board, 2, 1, i) && TryCoords(board, 2, 0, i))
		{
			coords[0] = 1;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
	}
	//center
	if (TryCoords(board, 1, 1, 0))
	{
		coords[0] = 1;
		coords[1] = 1;
		return coords;
	}
	//oppcorner
	for (int i = 0; i < 4; i++)
	{
		if (((notgamestates[i] & 1) == 1) && TryCoords(board, 0, 0, i))
		{
			coords[0] = 0;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
	}
	//corner
	for (int i = 0; i < 4; i++)
	{
		if (TryCoords(board, 0, 0, i))
		{
			coords[0] = 0;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
	}
	//side
	for (int i = 0; i < 4; i++)
	{
		if (TryCoords(board, 1, 0, i))
		{
			coords[0] = 1;
			coords[1] = 0;
			RotateCoord(coords, i);
			return coords;
		}
	}




	//next available
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (chars[i][j] == ' ')
			{
				coords[0] = j;
				coords[1] = i;
				return coords;
			}
		}
	}

	return coords;

}

int * Board::CalculateMove()
{
	return CalculateMove(chars);
}

bool Board::TakeComputerMove()
{
	int * coords;
	coords = CalculateMove();
	AddPiece(coords[0], coords[1]);
	delete[] coords;
	Display();

	if (HasWon())
	{
		std::cout << '\n' << piece << " Has Won\n";
		return false;
	}
	else if (turncounter >= 9)
	{
		std::cout << "\nThis game has resulted in a draw";
		return false;
	}

	if (piece == 'o')
	{
		piece = 'x';
	}
	else
	{
		piece = 'o';
	}

	return true;
}

Board::Board()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			chars[i][j] = ' ';
			std::cout << j + i * 3 + 1;
			if (j < 2)
			{
				std::cout << '|';
			}
		}
		std::cout << '\n';
		if (i < 2)
		{
			std::cout << "-+-+-\n";
		}
	}
	piece = 'x';
	turncounter = 0;
}

Board::Board(const Board & board)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			chars[i][j] = board.chars[i][j];
		}
	}
	piece = board.piece;

}

Board & Board::operator=(Board board)
{
	swap(*this, board);
	return *this;
}

bool Board::AddPiece(char c, uint8_t x, uint8_t y)
{
	if (chars[y][x] == ' ')
	{
		chars[y][x] = c;
		turncounter++;
		return true;
	}
	return false;
}

bool Board::AddPiece(uint8_t x, uint8_t y)
{
	return AddPiece(piece, x, y);
}

char Board::GetPiece(uint8_t x, uint8_t y) const
{
	return chars[y][x];
}

bool Board::HasWon(uint16_t GameState)
{
	return (GameState&292) == 292 || (GameState&146) == 146 ||
		(GameState&73) == 73 || (GameState&448) == 448 ||
		(GameState&56) == 56 || (GameState&7) == 7 ||
		(GameState&273) == 273 || (GameState&84) == 84;
}

bool Board::HasWon(void)
{
	return HasWon(GameState());
}

bool Board::TakeMove()
{
	bool CanPlace;
	do
	{
		std::cout << "Please choose your move: ";
		uint8_t pos;
		std::cin >> pos;
		pos -= 48;
		int * coords;
		coords = CoordFromPos(pos);
		CanPlace = AddPiece(coords[0], coords[1]);
		delete[] coords;
	} while (!CanPlace);
	Display();
	if (HasWon())
	{
		std::cout << '\n' << piece << " Has Won\n";
		return false;
	}
	else if (turncounter >= 9)
	{
		std::cout << "\nThis game has resulted in a draw";
		return false;
	}

	piece = piece == 'o' ? 'x' : 'o';

	if (!TakeComputerMove()) return false;

	return true;
}

void Board::Display() const
{
	std::cout << '\n';
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << chars[i][j];
			if (j < 2)
			{
				std::cout << "|";
			}
		}
		std::cout << '\n';
		if (i < 2)
		{
			std::cout << "-+-+-\n";
		}
	}
}

Board::~Board()
{
}

void swap(Board & first, Board & second)
{
	using std::swap;

	swap(first.chars, second.chars);
	swap(first.piece, second.piece);
}
