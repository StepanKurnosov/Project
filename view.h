#include <SFML\Graphics.hpp>
using namespace sf;

View view;

void getPlayerCoordinateForView(float x, float y) { 
	float tempX = x; float tempY = y;

	if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;
	if (y > 554) tempY = 554;
	if (x > 1147) tempX = 1147;

	view.setCenter(tempX, tempY);

}

void changeview() {
	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.001f); //масштабируем, уменьшение
		//view.zoom(1.0006f); //тоже самое помедленнее соответственно
	}
	if (Keyboard::isKeyPressed(Keyboard::Y)) {
		view.zoom(0.999f);
	}
	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//устанавливает размер камеры (наш исходный)
	}
}
