#include <SFML/Graphics.hpp>
#include "map.h"
#include "mission.h"
#include <iostream> 
#include <sstream>
#include "view.h"
#include "menu.h"
#include <vector>
#include <list>

using namespace sf;
using namespace std;

class Entity {
public:
    float dx, dy, x, y, speed, moveTimer, timeMove;
    int w, h;
    float health;
    bool life, isShoot;
    float CurrentFrame = 0;
    Texture texture;
    Sprite sprite;
    String name;
    Entity(Image& image, float X, float Y, int W, int H, String Name) {
        x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
        speed = 0; health = 100; dx = 0; dy = 0;
        life = true;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
    }
    virtual void update(float time) = 0;
    FloatRect getRect() {
        return FloatRect(x, y, w, h);
    }
};

class Player: public Entity {
public:
    int dir = 0; 
    int components = 0;
    Player(Image& image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
        if (name == "Player") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
    }
    void interactionWithMap()
    {
        for (int i = y / 32; i < (y + 27) / 32; i++)
            for (int j = x / 32; j < (x + 20) / 32; j++)
            {
                if (i >= 0 && i < 25 && j >= 0 && j < 46) {
                    if (TileMap[i][j] == '-' || TileMap[i][j] == '_')
                    {
                        if (dy > 0) { y = i * 32 - 27; }
                        if (dy < 0) { y = i * 32 + 32; }
                    }
                    if (TileMap[i][j] == '|' || TileMap[i][j] == '_' || TileMap[i][j] == 'd')
                    {
                        if (dx > 0) { x = j * 32 - 20; }
                        if (dx < 0) { x = j * 32 + 32; }
                    }
                    if (TileMap[i][j] == 'p') { health = health - 0.01; sprite.setColor(Color::Green); }
                    if (TileMap[i][j] == '+') {
                        health = health + 40;
                        if (health >= 100)
                            health = 100;
                        sprite.setColor(Color::White);
                        TileMap[i][j] = ' ';
                    }
                    if (TileMap[i][j] == 's') {
                        components++;
                        TileMap[i][j] = ' ';
                    }
                    if (components == 15) {
                        TileMap[11][45] = ' ';
                        TileMap[12][45] = ' ';
                        TileMap[13][45] = ' ';
                    }
                }
            }
    }
    float getPlayerCoordinateX() {	
        return x;
    }
    float getPlayerCoordinateY() {	
        return y;
    }
    void control(float time) {
        if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
            dir = 1; speed = 0.1;
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(40 * int(CurrentFrame), 60, 40, 60));
        }

        if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
            dir = 0; speed = 0.1;
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(40 * int(CurrentFrame), 120, 40, 60));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
            dir = 3; speed = 0.1;
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(40 * int(CurrentFrame), 180, 40, 60));
        }

        if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
            dir = 2; speed = 0.1;
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(40 * int(CurrentFrame), 0, 40, 60));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Space)))
            isShoot = true;
        getPlayerCoordinateForView(getPlayerCoordinateX(), getPlayerCoordinateY());
    }
    void update(float time)
    {   
        if (sprite.getColor() == Color::Green) {
            moveTimer += time;
            if (moveTimer > 1000) { health -= 2; moveTimer = 0; }
        }
        if (life) control(time);
        switch (dir)
        {
        case 0: dx = speed; dy = 0; break;
        case 1: dx = -speed; dy = 0; break;
        case 2: dx = 0; dy = speed; break;
        case 3: dx = 0; dy = -speed; break;
        }

        x = x + dx*time;
        y = y + dy*time;
        speed = 0;
        sprite.setPosition(x, y);
        interactionWithMap();
        if (health <= 0) { life = false; speed = 0; }
        
            
    }
};

class Enemy :public Entity {
public:
    Enemy(Image& image, float X, float Y, int W, int H, float timeMove, String Name, String typeName) :Entity(image, X, Y, W, H, Name) {
        this->timeMove = timeMove;
        if (typeName == "Horizontal-") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
            dx = -0.1;
            dy = 0;
        }
        if (typeName == "Horizontal+") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
            dx = 0.1;
            dy = 0;
        }
        if (typeName == "Vertical-") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
            dx = 0;
            dy = -0.1;
        }
        if (typeName == "Vertical+") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
            dx = 0;
            dy = 0.1;
        }
    }
    void interactionWithMap()
    {
        for (int i = y / 32; i < (y + 40) / 32; i++)
            for (int j = x / 32; j < (x + 40) / 32; j++)
            {
                if (i >= 0 && i < 25 && j >= 0 && j < 46) {
                    if (TileMap[i][j] == '-' || TileMap[i][j] == '_')
                    {
                        if (dy > 0) { y = i * 32 - 40; }
                        if (dy < 0) { y = i * 32 + 32; }
                    }
                    if (TileMap[i][j] == '|' || TileMap[i][j] == '_' || TileMap[i][j] == 'd')
                    {
                        if (dx > 0) { x = j * 32 - 40; }
                        if (dx < 0) { x = j * 32 + 32; }
                    }
                }
            }
        if (health < 50) {
            sprite.setColor(Color::Red);
        }
    }
    void animation(float time, float DX, float DY) {
        if (DX > 0) {
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 160, 80, 80));
        }
        if (DX < 0) {
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 80, 80, 80));
        }
        if (DY > 0) {
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 80, 80));
        }
        if (DY < 0) {
            CurrentFrame += 0.004 * time;
            if (CurrentFrame > 4) CurrentFrame -= 4;
            sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 240, 80, 80));
        }
    }
    void update(float time){
        moveTimer += time;
        if (moveTimer > timeMove) { dx *= -1; moveTimer = 0; dy *= -1; }
        animation(time, dx, dy);
        x = x + dx * time;
        y = y + dy * time;
        speed = 0;
        sprite.setPosition(x + w / 2, y + h / 2);
        interactionWithMap();
        if (health <= 0) { life = false; speed = 0; }
    }
};

class Bullet :public Entity {
public:
    int direction;
    Bullet(Image& image, float X, float Y, int W, int H, int dir, String Name) :Entity(image, X, Y, W, H, Name) {
        x = X;
        y = Y;
        direction = dir;
        speed = 0.8;
        w = W; h = H;
        life = true;
    }
    void interactionWithMap()
    {
        for (int i = y / 32; i < (y + 8) / 32; i++)
            for (int j = x / 32; j < (x + 8) / 32; j++)
            {
                if (i >= 0 && i < 25 && j >= 0 && j < 46) {
                    if (TileMap[i][j] == '-' || TileMap[i][j] == '_')
                    {
                        life = false;
                    }
                    if (TileMap[i][j] == '|' || TileMap[i][j] == '_' || TileMap[i][j] == 'd')
                    {
                        life = false;
                    }
                }
            }
    }
    void update(float time)
    {
        switch (direction)
        {
        case 0: dx = speed; dy = 0; break;
        case 1: dx = -speed; dy = 0; break;
        case 2: dx = 0; dy = speed; break;
        case 3: dx = 0; dy = -speed; break;
        }
               
        x += dx * time;
        y += dy * time;

        if (x <= 0) x = 1;
        if (y <= 0) y = 1;
        interactionWithMap();
        sprite.setPosition(x + w / 2, y + h / 2);
    }
};


int main()
{
    randomMapGenerate();
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game!", Style::Fullscreen);
    menu(window);
    view.reset(sf::FloatRect(0, 0, 640, 480));
    Clock clock;

    //ШРИФТЫ
    Font font;
    font.loadFromFile("Pixelfont.ttf");
    Text mission("", font, 20);
    mission.setFillColor(Color::Black);
    Text text("", font, 30);
    text.setFillColor(Color::White);

    //Карта
    Image map_image;
    map_image.loadFromFile("Images/NewMap.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);
    //Миссия
    Image quest_image;
    quest_image.loadFromFile("Images/mission.jpg");
    quest_image.createMaskFromColor(Color(0, 0, 0));
    Texture quest_texture;
    quest_texture.loadFromImage(quest_image);
    Sprite s_quest;
    s_quest.setTexture(quest_texture);
    s_quest.setTextureRect(IntRect(0, 0, 340, 510));
    s_quest.setScale(0.6f, 0.6f);

    Image heroImage;
    heroImage.loadFromFile("Images/mainchar.png");
    Player p(heroImage, 150, 150, 40, 60, "Player");
    Image enemyImage;
    enemyImage.loadFromFile("Images/monstr.png");
    Image BulletImage;
    BulletImage.loadFromFile("Images/bullet.png");
    BulletImage.createMaskFromColor(Color(0, 0, 0));
    std::list<Entity*>  entities;
    std::list<Entity*>::iterator it;
    std::list<Entity*>::iterator it2;

    //room 1
    entities.push_back(new Enemy(enemyImage, 200, 650, 80, 80, 1600, "Enemy", "Horizontal-"));
    entities.push_back(new Enemy(enemyImage, 100, 500, 80, 80, 1600, "Enemy", "Horizontal+"));
    //room 2
    entities.push_back(new Enemy(enemyImage, 400, 100, 80, 80, 1600, "Enemy", "Horizontal+"));
    entities.push_back(new Enemy(enemyImage, 400, 200, 80, 80, 1600, "Enemy", "Horizontal+"));
    //room 3
    entities.push_back(new Enemy(enemyImage, 400, 650, 80, 80, 2000, "Enemy", "Vertical-"));
    entities.push_back(new Enemy(enemyImage, 300, 500, 80, 80, 2000, "Enemy", "Vertical+"));
    //room 4
    entities.push_back(new Enemy(enemyImage, 650, 120, 80, 80, 2000, "Enemy", "Vertical+"));
    entities.push_back(new Enemy(enemyImage, 520, 120, 80, 80, 2000, "Enemy", "Vertical+"));
    //room 5
    entities.push_back(new Enemy(enemyImage, 570, 480, 80, 80, 2400, "Enemy", "Vertical+"));
    //room 6
    entities.push_back(new Enemy(enemyImage, 770, 100, 80, 80, 1600, "Enemy", "Horizontal-"));
    entities.push_back(new Enemy(enemyImage, 870, 200, 80, 80, 1600, "Enemy", "Horizontal+"));
    //room 7
    entities.push_back(new Enemy(enemyImage, 820, 700, 80, 80, 2400, "Enemy", "Vertical-"));
    //room 8
    entities.push_back(new Enemy(enemyImage, 1000, 100, 80, 80, 2000, "Enemy", "Vertical+"));
    entities.push_back(new Enemy(enemyImage, 1100, 250, 80, 80, 2000, "Enemy", "Vertical-"));
    //room 9
    entities.push_back(new Enemy(enemyImage, 1000, 650, 80, 80, 1600, "Enemy", "Horizontal+"));
    entities.push_back(new Enemy(enemyImage, 1000, 500, 80, 80, 1600, "Enemy", "Horizontal+"));
    //hallway
    entities.push_back(new Enemy(enemyImage, 100, 370, 80, 80, 4000, "Enemy", "Horizontal+"));
    entities.push_back(new Enemy(enemyImage, 500, 370, 80, 80, 4000, "Enemy", "Horizontal+"));
    entities.push_back(new Enemy(enemyImage, 950, 370, 80, 80, 4000, "Enemy", "Horizontal-"));
    //lastroom
    entities.push_back(new Enemy(enemyImage, 1300, 700, 80, 80, 6500, "Enemy", "Vertical-"));

    while (window.isOpen())
    {

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Escape)
                    window.close();
            if (p.isShoot == true) { p.isShoot = false; entities.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, p.dir, "Bullet")); }
        }

        p.update(time);

        for (it = entities.begin(); it != entities.end();)
        {
            Entity* b = *it;
            b->update(time);
            if (b->life == false) { it = entities.erase(it); delete b; }
            else it++;
        }

        for (it = entities.begin(); it != entities.end(); it++)
        {
            if (((*it)->name == "Enemy") && ((*it)->getRect().intersects(p.getRect())))
            {
                p.life = false;
                if ((*it)->dx > 0)
                {
                    (*it)->x = p.x - (*it)->w;
                    (*it)->dx = 0;
                }
                if ((*it)->dx < 0)
                {
                    (*it)->x = p.x + p.w; 
                    (*it)->dx = 0;
                }
                if ((*it)->dy < 0)
                {
                    (*it)->y = p.y + p.w; 
                    (*it)->dy = 0;
                }
                if ((*it)->dy > 0)
                {
                    (*it)->y = p.y - p.w; 
                    (*it)->dy = 0;
                }
            }

            for (it2 = entities.begin(); it2 != entities.end(); it2++)
            {
                if ((*it)->getRect() != (*it2)->getRect()) {
                    if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Enemy") && ((*it2)->name == "Enemy"))
                    {
                        (*it)->dx *= -1;
                        (*it)->dy *= -1;
                    }
                    if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Enemy") && ((*it2)->name == "Bullet"))
                    {
                        if ((*it)->name == "Bullet")
                            (*it)->life = false;
                        if ((*it2)->name == "Bullet")
                            (*it2)->life = false;
                        if ((*it)->name == "Enemy") {
                            (*it)->health -= 18;
                            if ((*it)->dx < 0.2)
                                (*it)->dx *= 1.15;
                            else (*it)->dx = 0.2;
                            if ((*it)->dy < 0.2)
                                (*it)->dy *= 1.15;
                            else (*it)->dy = 0.2;
                        }
                        if ((*it2)->name == "Enemy") {
                            (*it2)->health -= 18;
                            if ((*it2)->dx < 0.2)
                                (*it2)->dx *= 1.15;
                            else (*it2)->dx = 0.2;
                            if ((*it2)->dy < 0.2)
                                (*it2)->dy *= 1.15;
                            else (*it2)->dy = 0.2;
                        }
                    }
                }
            }

        }

        changeview();
        window.setView(view);
        window.clear();

        // СОЗДАНИЕ КАРТЫ
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == '_') s_map.setTextureRect(IntRect(32 * 1, 0, 32, 32));
                if (TileMap[i][j] == '-') s_map.setTextureRect(IntRect(32 * 2, 0, 32, 32));
                if (TileMap[i][j] == '|') s_map.setTextureRect(IntRect(32 * 3, 0, 32, 32));
                if (TileMap[i][j] == '[') s_map.setTextureRect(IntRect(32 * 4, 0, 32, 32));
                if (TileMap[i][j] == ']') s_map.setTextureRect(IntRect(32 * 5, 0, 32, 32));
                if (TileMap[i][j] == '<') s_map.setTextureRect(IntRect(32 * 6, 0, 32, 32));
                if (TileMap[i][j] == '>') s_map.setTextureRect(IntRect(32 * 7, 0, 32, 32));
                if (TileMap[i][j] == 'p') s_map.setTextureRect(IntRect(32 * 1, 32 * 1, 32, 32));
                if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32 * 3, 32 * 1, 32, 32));
                if (TileMap[i][j] == '+') s_map.setTextureRect(IntRect(32 * 4, 32 * 1, 32, 32));
                if (TileMap[i][j] == 'd') s_map.setTextureRect(IntRect(32 * 5, 32 * 1, 32, 32));
                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }
        for (it = entities.begin(); it != entities.end(); it++) {
            window.draw((*it)->sprite);
        }
        ostringstream playerHealthString;
        playerHealthString << (int)p.health;
        ostringstream playerComponentsString;
        playerComponentsString << p.components;
        if (p.life == true) {
            text.setString("Health: " + playerHealthString.str() + "\n" + "Components: " + playerComponentsString.str() + "/15");
            text.setPosition(view.getCenter().x + 70, view.getCenter().y - 225);
            window.draw(p.sprite);
            window.draw(p.sprite);
            window.draw(text);
        }
        if (Keyboard::isKeyPressed(Keyboard::Tab) || (p.getPlayerCoordinateX() < 180 && p.getPlayerCoordinateX() > 120 && p.getPlayerCoordinateY() < 180 && p.getPlayerCoordinateY() > 120)) {
            std::ostringstream task;
            task << getTextMission();
            mission.setString(task.str());
            mission.setPosition(view.getCenter().x - 95, view.getCenter().y - 130);
            s_quest.setPosition(view.getCenter().x - 100, view.getCenter().y - 130);
            window.draw(s_quest);
            window.draw(mission);
        }
        if (p.life == false) {
            std::ostringstream death;
            death << getTextDeath();
            mission.setString(death.str());
            mission.setPosition(view.getCenter().x - 95, view.getCenter().y - 130);
            s_quest.setPosition(view.getCenter().x - 100, view.getCenter().y - 130);
            window.draw(s_quest);
            window.draw(mission);
        }
        if (p.getPlayerCoordinateX() > 1490) {
            std::ostringstream win;
            win << getTextWin();
            mission.setString(win.str());
            mission.setPosition(view.getCenter().x - 95, view.getCenter().y - 130);
            s_quest.setPosition(view.getCenter().x - 100, view.getCenter().y - 130);
            window.draw(s_quest);
            window.draw(mission);
        }

        window.display();
    }
    return 0;
}