// Tugas Struktur Data.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

int wx = 1280;
int wy = 720;

RenderWindow window(VideoMode(wx, wy), "Power Up!", Style::Titlebar | Style::Close);
Event ev;

class powerUp
{
    int x, y;
    float rad, hbrad;
    int hit;

public:
    powerUp()
    {
        x = rand() % 1000 + 100;
        y = rand() % 600 + 100;
        rad = 20;
        hbrad = 50;
        hit = 0;
    }

    void drawPower()
    {
        Texture PUPT;
        PUPT.loadFromFile("PowerUp.png");
        Sprite powerUp(PUPT);
        powerUp.setScale(Vector2f(0.15, 0.15));

        if (hit == 0)
        {
            powerUp.setPosition(x-20, y-20);
            window.draw(powerUp);
        }
        else
        {
            x = -500;
            powerUp.setPosition(x-20, y-20);
            window.draw(powerUp);
        }
        
    }

    void checkHit(float px, float py, int state)
    {
        if (state == 0)
        {
            if ((px - hbrad) < (x) && (px + hbrad) > (x) && (py - (hbrad + 50)) < (y) && (py + hbrad) > (y))
            {
                hit = 1;
            }
        }
    }

    void checkState(int state)
    {
        if (state == 2)
        {
            hit = -1;        
        }
        else if (state == 0 && hit == -1)
        {
            hit = 0;
        }
    }

    int getHit()
    {
        return hit;
    }

    float getPX()
    {
        return x;
    }

    float getPY()
    {
        return y;
    }
};

class Player
{
    float x, y, sc;
    int state, countBlink, countDur;
    Texture playerTextureNormal;
    Texture playerTextureRed;

public:
    Player()
    {
        x = wx/2;
        y = wy/2;
        sc = 0.25;
        countBlink = 0;
        countDur = 0;

        state = 0; //0 is Normal, 1 is Red, 2 is Big
    }

    void drawPlayer()
    {
        if (!playerTextureNormal.loadFromFile("Player.png") || !playerTextureRed.loadFromFile("PlayerRed.png"))
        {
            cout << "A Player Texture is Not Found!" << endl;
        }

        if (state == 0)
        {
            sc = 0.25;
            Sprite playerChar(playerTextureNormal);
            playerChar.setScale(Vector2f(sc, sc));
            playerChar.setPosition(x-50, y-90);
            window.draw(playerChar);
        }

        else if (state == 1)
        {
            sc = 0.25;
            Sprite playerChar(playerTextureRed);
            playerChar.setScale(Vector2f(sc, sc));
            playerChar.setPosition(x - 50, y - 90);
            window.draw(playerChar);
        }

        else if (state == 2)
        {
            sc = 0.35;
            Sprite playerChar(playerTextureNormal);
            playerChar.setScale(Vector2f(sc, sc));
            playerChar.setPosition(x - 65, y - 117);
            window.draw(playerChar);
        }
    }

    void movePlayer(Event ev)
    {
        if (ev.key.code == Keyboard::W)
        {
            y -= 10;
        }
        else if (ev.key.code == Keyboard::A)
        {
            x -= 10;
        }
        else if (ev.key.code == Keyboard::S)
        {
            y += 10;
        }
        else if (ev.key.code == Keyboard::D)
        {
            x += 10;
        }
    }

    void checkHit(int hit)
    {
        if (hit == 1)
        {
            if (countDur < 50)
            {
                countDur++;
                countBlink++;
                if (countBlink > 6)
                {
                    countBlink = 0;
                }
                else if (countBlink < 3)
                {
                    state = 1;
                }
                else
                {
                    state = 0;
                }
            }
            else
            {
                countDur = 0;
                countBlink = 0;
                state = 2;
            }
        }

        if (hit == 0 && state == 2)
        {
            countDur++;
            if (countDur > 500)
            {
                state = 0;
                countDur = 0;
            }
        }

        if (hit == -1)
        {
            countDur++;
            if (countDur > 500)
            {
                countDur = 0;
                state = 0;
            }
        }
    }
    
    float getX()
    {
        return x;
    }
    
    float getY()
    {
        return y;
    }

    int getState()
    {
        return state;
    }
};

int main()
{
    Texture bgT;
    bgT.loadFromFile("BG.png");
    bgT.setSmooth(1);
    Sprite bg(bgT);
    bg.setScale(Vector2f(0.6, 0.6));

    class Player p;
    vector<powerUp> pow;
    class powerUp firstPow;
    pow.push_back(firstPow);
    int pi = 0;

    Clock clock;
    Time t0 = seconds(1000);
    Time duration = seconds(5);

    int randx, randy;

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
                if (ev.key.code == Keyboard::W || ev.key.code == Keyboard::A || ev.key.code == Keyboard::S || ev.key.code == Keyboard::D)
                {
                    p.movePlayer(ev);
                    break;
                }
            };
        }


        //GAMEPLAY-------------------------------------

        //Drawing//
        window.draw(bg);
        p.drawPlayer();
        pow[pi].drawPower();

        //Checks//
        pow[pi].checkHit(p.getX(), p.getY(), p.getState());
        p.checkHit(pow[pi].getHit());
        pow[pi].checkState(p.getState());

        //Time
        if (p.getState() == 2)
        {
            t0 = clock.getElapsedTime();
        }

        if ((clock.getElapsedTime() - t0) > duration)
        {
            class powerUp nextPow;
            pow.push_back(nextPow);
            pi++;
            t0 = seconds(1000);
            clock.restart();
        }

        //RENDERING--------------------------------------
        window.display();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
