#define _WIN32_WINNT 0x0500 // do funkcji GetAsyncKeyState(); = ruszanie paletka

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <conio.h>

#include "Paddle.h"
#include "Ball.h"
#include "Screen.h"
#include "Construction.h"
#include "Powerups.h"
#include "Arkanoid.h"

void turn_off_powerups(std::vector <Powerups> &powerups,LaserPowerup &laser_powerup,std::vector <SimpleBonus> &simple_bonus) // zeruje timery, usuwa spadajace bonusy z planszy. (funkcja uruchamiana przy tracie zycia)
{
    for(int i=0 ; i<simple_bonus.size() ; i++)
        simple_bonus[i].popAll();
    for(int i=0 ; i<powerups.size() ; i++)
        powerups[i].turn_off();
    laser_powerup.turn_off();
}

void Arkanoid::powerup_collision(int width,int &record,std::vector <Ball> &ball,Paddle &paddle,std::vector <Powerups> &powerups, LaserPowerup &laser_powerup, std::vector <SimpleBonus> &simple_bonus)
{ // paletka vs spadajace bonusy
    for(int i=0 ; i<powerups.size() ; i++) // powerupy standardowe, zmiana szybkosci, moc pilki. wektor klasy powerups
        for(int j=0 ; j<powerups[i].getBonusAmount() ; j++) // kazdy powerup moze miec 'j' spadajacych bonusow.
            if(paddle.touch(powerups[i].getX(j),powerups[i].getY(j)) && powerups[i].getPower()<3) // gdy paletka dotknie spadajacy bonus
            {
                powerups[i].turn_on(record); // powerup sie wlacza
                powerups[i].popBonus(j); // usuwam bonus, ktory dotknela paletka
                j--; // getBonusAmount() sie zmniejszylo, wiec zmniejsza sie tez j
            }

    for(int j=0 ; j<laser_powerup.getBonusAmount() ; j++) // powerup niestandardowy
        if(paddle.touch(laser_powerup.getX(j),laser_powerup.getY(j))) // tak samo ma 'j' spadajacych bonusow
        {
            laser_powerup.turn_on(record);
            laser_powerup.popBonus(j);
            j--;
        }
    for(int i=0 ; i<simple_bonus.size() ; i++) // powerupy standardowe, zmiana szybkosci, moc pilki. wektor klasy powerups
        for(int j=0 ; j<simple_bonus[i].getBonusAmount() ; j++) // kazdy powerup moze miec 'j' spadajacych bonusow.
            if(paddle.touch(simple_bonus[i].getX(j),simple_bonus[i].getY(j))) // gdy paletka dotknie spadajacy bonus
            {
                switch(i)
                {
                case 0:
                    {
                        for(int n=0 ; n<ball.size() ; n++)
                            ball.pop_back();
                        changeLifes();
                        Sleep(1000);
                        Ball tmp_ball(width/2);
                        ball.push_back(tmp_ball);
                        paddle.setPosition(width/2);
                        turn_off_powerups(powerups,laser_powerup,simple_bonus);
                    }
                    break;
                case 1:
                    if(getLifes() < 9)
                        changeLifes(1);
                    break;
                case 2:
                    Ball tmp_ball(simple_bonus[2].getX(j)); // nie mozna uzywac konstruktora podczas inicjacji wektora, wiec dodalem zmienna tymczasowa
                    ball.push_back(tmp_ball);
                    break;
                }
                simple_bonus[i].popBonus(j); j--;
            }
}

void Arkanoid::gameover_animation(Paddle &paddle, Screen &screen, Construction &construction)
{
    std::string frame = "";

    frame += char(9); // gora planszy
    frame += char(201);
    for(int w=0 ; w<=screen.getWidth() ; w++)
        frame += char(205);
    frame += char(187);
    frame += '\n';

    for(int h=screen.getHeight() ; h>=0 ; h--)
    {
        frame += char(9); // gora planszy
        frame += char(186);
        for(int w=0 ; w<=screen.getWidth() ; w++)
        {
            bool can_put = true;
            switch(construction.getBrick(w,h))
            {
            case 1:
                frame += char(176); can_put = false;
                break;
            case 2:
                frame += char(177); can_put = false;
                break;
            case 3:
                frame += char(178); can_put = false;
            }
            if(can_put)
            {
                if(paddle.touch(w,h)) frame += '=';
                else frame += ' ';
            }
        }
        frame += char(186);
        frame += '\n';
    }
    system("cls");
    int meter = 0;
    int calculator = 0;
    int direction = 2; //  1 = ^    2 = >     3 = V     4 = <
    int color_it = 1;
    int steps_w = screen.getWidth() + 2,steps_h = screen.getHeight() + 2;
    bool displayed = false;
    int record=0;
    while(calculator < (screen.getWidth()+3)*(screen.getHeight()+2) || displayed == false)
    {
        if(clock() - record > 30)
        {
            for(int i=0 ; i<4 ; i++)
            {
                if(calculator != (screen.getWidth()+3)*(screen.getHeight()+2) )
                {
                    displayed = false;
                    calculator ++;
                    switch(direction)
                    {
                    case 1:
                        if(steps_h == screen.getHeight() + 2 - meter)
                            frame[color_it] = char(200);
                        else
                            frame[color_it] = char(186);
                        color_it -= (screen.getWidth()+5);
                        steps_h--;
                        break;
                    case 2:
                        if(steps_w == screen.getWidth() + 3 - meter)
                            frame[color_it] = char(201);
                        else
                            frame[color_it] = char(205);
                        color_it ++;
                        steps_w--;
                        break;
                    case 3:
                        if(steps_h == screen.getHeight() + 2 - meter)
                            frame[color_it] = char(187);
                        else
                            frame[color_it] = char(186);
                        color_it += (screen.getWidth()+5);
                        steps_h--;
                        break;
                    case 4:
                        if(steps_w == screen.getWidth() + 3 - meter)
                            frame[color_it] = char(188);
                        else
                            frame[color_it] = char(205);
                        color_it --;
                        steps_w--;
                    }

                    if(!steps_h || !steps_w)
                    {
                        if(!steps_w)
                            meter++;
                        direction = (direction % 4) + 1;
                        steps_w = screen.getWidth() + 3 - meter;
                        steps_h = screen.getHeight() + 2 - meter;
                    }
                }
            }
            displayed = true;
            system("cls");
            std::cout<<"\n\n"<<frame;
            record = clock();
        }
    }
    Sleep(1000);
}

void Arkanoid::run()
{
    HWND Hwnd = GetConsoleWindow(); // do funkcji GetAsyncKeyState();

    const int width=19,height=19,paddle_width = 2; // potrzebne wymiary

    Paddle paddle(width/2,paddle_width); // tworzenie paletki
    std::vector <Ball> ball; // moze byc wiecej pilek, dlatego vector

    {
        Ball tmp_ball(width/2); // nie mozna uzywac konstruktora podczas inicjacji wektora, wiec dodalem zmienna tymczasowa
        ball.push_back(tmp_ball);
    }

    Screen screen(width,height); // plansza
    Construction construction(width,height); // cegly
    LaserPowerup laser_powerup(100,height); // specjalny powerup. pierwsza wartosc jest do timera, a druga to granica siegania pociskow
    std::vector <SimpleBonus> simple_bonus(3); // 0 - dodanie zycia, 1 - odjecie zycia, 2 - dodanie pilki
    std::vector <Powerups> powerups;

    {
        Powerups tmp1(150),tmp2(200); // tmp1 jest na 10s, tmp2 na 15s
        tmp1.setName("Speedup:         "); // nazwy sa tylko po to zeby usprawnic wyswietlanie w screen.display(...) i wyswietlic info o powerupach w petli
        powerups.push_back(tmp1);

        tmp1.setName("Slowdown:        "); // 1
        powerups.push_back(tmp1);

        tmp2.setName("PaddleDecrease:  "); // 2
        powerups.push_back(tmp2);

        tmp2.setName("PaddleExtend:    "); // 3
        powerups.push_back(tmp2);

        tmp1.setName("ExplosiveTouch:  "); // 4
        powerups.push_back(tmp1);

        tmp1.setName("GoThrough:       "); // 5
        powerups.push_back(tmp1);

        tmp1.setName("Harmless:        "); // 6
        powerups.push_back(tmp1);

        tmp1.setName("InvertControls:  "); // 7
        powerups.push_back(tmp1);

        tmp2.setName("SpeedPaddle:     "); // 8
        powerups.push_back(tmp2);

        tmp2.setName("SlowPaddle:      "); // 9
        powerups.push_back(tmp2);

        tmp2.setName("Shield:          "); // 10
        powerups.push_back(tmp2);
    }

    srand(time(NULL)); // losowanie powerupow

    int record, record_display = 0, record_press_key = 0, record_ball_move = 0, record_powerup_fall_down = 0, record_powerup_refresh = 0;
    bool win = false;
    while(!GetAsyncKeyState(VK_ESCAPE) && getLifes() && win == false) // koniec gry gdy nacisnie sie escape albo straci sie zycia
    {
        record = clock();
        if((GetAsyncKeyState(VK_LEFT) && !powerups[7].getTimer() || (GetAsyncKeyState(VK_RIGHT) && powerups[7].getTimer())) && !paddle.touch(0,0) && record - record_press_key > paddle.getSpeed()) // strzalka w lewo
        {
            paddle.move_left();
            record_press_key = record;
        }
        if((GetAsyncKeyState(VK_LEFT) && powerups[7].getTimer() || (GetAsyncKeyState(VK_RIGHT) && !powerups[7].getTimer())) && !paddle.touch(screen.getWidth(),0) && record - record_press_key > paddle.getSpeed()) // strzalka w prawo
        {
            paddle.move_right();
            record_press_key = record;
        }
        if(record - record_ball_move > getSpeed())
        {
            for(int i=0 ; i<ball.size() ; i++) // petla for dla 'i' pilek
            {
                ball[i].take_step();
                ball[i].check_collision(paddle,screen,construction,powerups,laser_powerup,simple_bonus);
                if(ball[i].getY() < 0) // tej kolizji nie moglem dac w funkcji klasy ball, bo uzywam tu pop_back, a vector jest zainicjowany tutaj
                {
                    for(int j=i+1 ; j<ball.size()  ; j++)
                        ball[j-1] = ball[j]; //              usuwanie ball[i] ktora wypadla pod plansze
                    ball.pop_back();
                    if(ball.size() == 0) // gdy niema pilek
                    {
                        changeLifes(); // traci zycie
                        Ball tmp_ball(width/2);
                        ball.push_back(tmp_ball);
                        paddle.setPosition(width/2);
                        turn_off_powerups(powerups,laser_powerup,simple_bonus);
                        Sleep(500);
                    }
                }
            }
            if(construction.bricks_destroyed())
                win = true;
            record_ball_move = record;
        }
        if(record - record_powerup_fall_down > 400) // bonusy spadaja wolno
        {
            for(int i=0 ; i<simple_bonus.size() ; i++)
                simple_bonus[i].fall_down();
            for(int i=0 ; i<powerups.size() ; i++)
                powerups[i].fall_down();
            laser_powerup.fall_down();
            record_powerup_fall_down = record;
        }
        if(record - record_powerup_refresh > 100) // odswierza stopery, sprawdza czy paletka zlapala bonus
        {
            for(int i=0 ; i<powerups.size() ; i++)
                if(powerups[i].getTimer())
                    powerups[i].uppdate(record);

            if(laser_powerup.getTimer())
                laser_powerup.uppdate(record);

            powerup_collision(width,record,ball,paddle,powerups,laser_powerup,simple_bonus);
            setSpeed(100 - (powerups[0].getPower()*20) + (powerups[1].getPower()*25)); // ustawia szybkosc pilki
            paddle.setWidth(paddle_width - powerups[2].getPower() + powerups[3].getPower()); // ustawia szerokosc paleti
            paddle.setSpeed(120 - (powerups[8].getPower()*35) + (powerups[9].getPower()*60));
            record_powerup_refresh = record;
        }
        if(record - laser_powerup.getRecordMove()> 100)
            laser_powerup.laser_move(record,construction); // record_move = record jest w funkcji
        if(laser_powerup.getTimer() && record - laser_powerup.getRecordShot()> 500)
            laser_powerup.laser_shot(record,paddle); // record_shot = record jest w funkcji

        if(record - record_display > 100)
        {
            system("cls");
            screen.display(paddle,ball,construction,powerups,laser_powerup,simple_bonus,getLifes());
            //std::cout<<"\n\n"<<speed; // po to zebym widzial czy speedup i slowdown dobrze dzialaja
            record_display = record;
        }
    }
    gameover_animation(paddle,screen,construction);

    system("cls");
    for(int h=0 ; h<=screen.getHeight()/2 ; h++)
        std::cout<<"\n";
    std::cout<<char(9)<<"You ";
    if(win)
        std::cout<<"Win!";
    else
        std::cout<<"Lose :(";
    Sleep(2000);

}
