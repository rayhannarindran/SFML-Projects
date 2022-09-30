// Med Me Up Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"

using namespace sf;
using namespace std;

RenderWindow window(VideoMode(1280, 720), "MED ME UP!", Style::Titlebar | Style::Close);
Event ev;

class BulletsP
{
private:
    float xp, yp;
    float timing;
    int stp;
    int outOfBox;
public:
    BulletsP()
    {
        xp = 0;
        yp = 190.f;
        stp = 10;
        timing = rand() % 1000 + 50;
        outOfBox = 0;
    }

    void bulletMovementP()
    {
        if (outOfBox != 1)
        {
            yp += stp;
        }
    }

    void checkVisibility()
    {
        if (yp > 720.f)
        {
            outOfBox = 1;
        }
    }

    void drawBulletsP(float xb)
    {
        xp = xb;
        CircleShape bullet(15, 3);
        bullet.setRotation(180);
        bullet.setFillColor(Color(0, 255, 0));
        bullet.setPosition(xp-70, yp+18);
        window.draw(bullet);
    }
    float getTiming()
    {
        return timing;
    }
    void setY(float y)
    {
        yp = y;
    }
};

class Bullets
{
private:
    float xp, yp, stp;
    int hit;

public:
    Bullets()
    {
        xp = 0;
        yp = 0;
        stp = 5;
        hit = 0;
    }

    void setBulletPosition(float x, float y)
    {
        xp = x; yp = y;
    }

    void bulletMovement()
    {
        yp -= stp;
    }

    void drawBullet()
    {
        if (hit != 1)
        {
            CircleShape bullet(10);
            bullet.setFillColor(Color(255, 0, 0));
            bullet.setPosition(xp + 100, yp + 50);
            window.draw(bullet);
        }
    }

    void checkHit(int x1, int x2, int y)
    {
        cout << "plane x1: " << x1 << endl;
        cout << "plane x2: " << x2 << endl;
        cout << "bullet x: " << xp << endl;
        cout << "bullet y: " << yp << endl;
        if (xp > x1 && xp < x2 && yp < y+100)
        {
            hit = 1;
        }
    }

    int isHit()
    {
        return hit;
    }
};

class Entities
{
private:
    float x, y, hbx1, hbx2;
    float stx;
    int xmax, ymax;
    int hit;

public:
    Entities()
    {
        xmax = 1280;
        ymax = 720;
        x = 0.f;
        y = 30.f;
        stx = rand() % 10 + 5;
        hbx1 = 0;
        hbx2 = 0;
        hit = 0;
    }

    void checkHit(int x)
    {
        hit = x;
    }

    void moveHitboxP()
    {
        hbx1 = x - 400;
        hbx2 = x + 150;
    }

    void drawPesawat()
    {
        if (hit != 1)
        {
            Texture planeBody;
            if (!planeBody.loadFromFile("Plane.png"))
            {
                printf("Plane Texture Not Found!");
            }
            Sprite planeS(planeBody);
            planeS.setScale(Vector2f(-0.5, 0.5));
            planeS.setPosition(x, y);
            window.draw(planeS);
        }
        else {
            CircleShape planeS((0,0,0));
            planeS.setScale(Vector2f(-0.5, 0.5));
            planeS.setPosition(x, y);
            window.draw(planeS);
        }
    }

    void drawTank()
    {
        y = 500;
        Texture tankBody;
        if (!tankBody.loadFromFile("Tank.png"))
        {
            printf("Tank Texture Not Found!");
        }
        Sprite tankS(tankBody);
        tankS.setScale(Vector2f(0.5, 0.5));
        tankS.setPosition(x, y);
        window.draw(tankS);
    }
   
    void Movement()
    {
        int random = rand() % 10 + 6;
        x += random;
    }

    void playerPosition(int xh)
    {
        if (xh < 47)
        {
            x = 47 - 100;
        }
        else if (xh > 1169)
        {
            x = 1169 - 100;
        }
        else
        {
            x = xh - 100;
        } 
    }

    float getPlayerX()
    {
        return x;
    }
    
    float getPlayerY()
    {
        return y;
    }

    float getPlayerHBX1()
    {
        return hbx1;
    }

    float getPlayerHBX2()
    {
        return hbx2;
    }

};

int main()
{
    class Entities t;
    class Entities p[100];
    class BulletsP bulletsP[100];
    vector<Bullets> bullets;
    int pInc = 0;

    while (window.isOpen())
    {
        //RENDERING
        window.clear(Color::White);

        //QUITTING
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case Event::Closed:
                window.close();
                break;

            case Event::KeyPressed:
                if (ev.key.code == Keyboard::Escape)
                {
                    window.close();
                    break;
                }

            case Event::MouseMoved:
                t.playerPosition(ev.mouseMove.x);
                break;

            case Event::MouseButtonReleased:
                if (ev.mouseButton.button == Mouse::Left)
                {
                    class Bullets bullet;
                    bullet.setBulletPosition(t.getPlayerX(), t.getPlayerY());
                    bullets.push_back(bullet);
                }
            
            }

        }
        //GAMEPLAY
        t.drawTank();
        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].drawBullet();
            bullets[i].bulletMovement();
            bullets[i].checkHit(p[pInc].getPlayerHBX1(), p[pInc].getPlayerHBX2(), p[pInc].getPlayerY());
            p[pInc].checkHit(bullets[i].isHit());
        }
        p[pInc].drawPesawat();
        p[pInc].moveHitboxP();
        p[pInc].Movement();

        float timing = bulletsP[pInc].getTiming();
        const float planeX = p[pInc].getPlayerX();

        if (planeX >= timing)
        {
            float xb = timing;
            bulletsP[pInc].drawBulletsP(timing);
            bulletsP[pInc].checkVisibility();
            bulletsP[pInc].bulletMovementP();
        }

        if (p[pInc].getPlayerX() > 1600){pInc++;}

        //RENDERING
        window.display();
    }
}
