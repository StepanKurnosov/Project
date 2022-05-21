#include <SFML\Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

void menu(RenderWindow& window) {
    Texture menuTexture1, menuTexture2, menuTexture3, menuBackground, menuPudge;
    menuTexture1.loadFromFile("Images/PlayCamp.png");
    menuTexture2.loadFromFile("Images/ExitGame.png");
    menuBackground.loadFromFile("Images/Fon312.jpg");
    Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
    bool isMenu = 1;
    int menuNum = 0;
    menu1.setPosition(688, 200);
    menu2.setPosition(793, 500);

    //////////////////////////////лемч///////////////////
    while (isMenu)
    {
        menu1.setColor(Color::White);
        menu2.setColor(Color::White);
        menuNum = 0;

        if (IntRect(688, 200, 544, 97).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); menuNum = 1; }
        if (IntRect(793, 500, 334, 97).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 2; }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuNum == 1) isMenu = false;
            if (menuNum == 2) { window.close(); isMenu = false; }

        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);

        window.display();
    }
}
