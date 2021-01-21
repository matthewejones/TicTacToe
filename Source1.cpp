#include<iostream>

int main()
{
	char *arr = new char[9]{ '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::cout << arr[1][1] << arr[2][2];
	return 0;
}