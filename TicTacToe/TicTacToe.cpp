// TicTacToe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Board.h"

int main()
{
	
	char c;
	bool first = true;
	do
	{
		std::cout << "Would you like to be noughts (o) or crosses (x)?: ";
		std::cin >> c;
		std::cin.ignore(1000, '\n');
	} while (c != 'x' && c != 'o');
	if (c == 'o')
		first = false;
	
	do
	{
		Board board;
		if (!first)
			board.TakeComputerMove();

		for (int i = 0; i < 9; i++)
		{
			if (!board.TakeMove())
				break;
		}
		std::cout << "\nWould you like to pay again (y/n)?: ";
		std::cin >> c;

		std::cin.ignore(1000, '\n');
		first = !first;


	} while (c == 'y');

}


