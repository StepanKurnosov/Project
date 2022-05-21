#include <SFML\Graphics.hpp>
#include <iostream>

const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 46;//размер карты ширина 


sf::String TileMap[HEIGHT_MAP] = {
	"[--------------------------------------------]",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|__    ___    ___    ____    ___    ___    __|",
	"|                                            d",
	"|                                            d",
	"|                                            d",
	"|__    ___    ___    ____    ___    ___    __|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"|_      _      _      __      _      _      _|",
	"<-------------------------------------------->"
};

void randomMapGenerate() {

	int randomX = 0;//случайный элемент по горизонтали
	int randomY = 0;//случ эл-т по вертикали
	srand(time(0));//рандом	
	int countSphere = 15;
	int countPoison = 20;
	int countHeal = 3;

	while (countSphere > 0) {
		randomX = 1 + rand() % (WIDTH_MAP - 1);
		randomY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomY][randomX] == ' ' &&
			randomX != 3 && randomX != 4 && randomX != 5 && randomX != 6 &&
			randomY != 3 && randomY != 4 && randomY != 5 && randomY != 6 && randomY != 7) {
			TileMap[randomY][randomX] = 's'; 
			countSphere--;
		}
	}

	while (countPoison > 0) {
		randomX = 1 + rand() % (WIDTH_MAP - 1);
		randomY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomY][randomX] == ' ' &&
			randomX != 3 && randomX != 4 && randomX != 5 && randomX != 6 &&
			randomY != 3 && randomY != 4 && randomY != 5 && randomY != 6 && randomY != 7) {
			TileMap[randomY][randomX] = 'p'; 
			countPoison--;
		}
	}
	while (countHeal > 0) {
		randomX = 1 + rand() % (WIDTH_MAP - 1);
		randomY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomY][randomX] == ' ' &&
			randomX != 3 && randomX != 4 && randomX != 5 && randomX != 6 &&
			randomY != 3 && randomY != 4 && randomY != 5 && randomY != 6 && randomY != 7) {
			TileMap[randomY][randomX] = '+';
			countHeal--;
		}
	}
}
