#include <cstdlib>
#include <ctime>
#include <windows.h>

#include "Paddle.h"
#include "Ball.h"
#include "Screen.h"
#include "Construction.h"
#include "Powerups.h"

void rand_powerup(int x,int y,std::vector <Powerups> &powerups,LaserPowerup &laser_powerup, std::vector <SimpleBonus> &simple_bonus)
{
    int a = (rand()%20) - 1; // losuje ktory powerup wypadnie ze zniszczonego klocka - punkt (x,y)

    switch(a)
    {
    case -1:
        laser_powerup.setBonus(x,y); break; // wlacza laser
    case 0:
        powerups[0].setBonus(x,y); break; // przyspieszenie pilki
    case 1:
        powerups[1].setBonus(x,y); break; // spowolnienie pilki
    case 2:
        powerups[2].setBonus(x,y); break; // zmniejszenie paletki
    case 3:
        powerups[3].setBonus(x,y); break; // zwiekszenie paletki
    case 4:
        powerups[4].setBonus(x,y); break; // wybuch przy uderzaniu pilki w cegle
    case 5:
        powerups[5].setBonus(x,y); break; // pilka przechodzaca przez cegly
    case 6:
        powerups[6].setBonus(x,y); break; // pilka nie niszczaca cegiel
    case 7:
        powerups[7].setBonus(x,y); break; // odwrocenie sterowania
    case 8:
        powerups[8].setBonus(x,y); break; // przyspieszenie paletki
    case 9:
        powerups[9].setBonus(x,y); break; // spowolnienie paletki
    case 10:
        powerups[10].setBonus(x,y); break; // tarcza
    //case 11:
    //    simple_bonus[0].setBonus(x,y); break; // zabiera zycie
    case 12:
        simple_bonus[1].setBonus(x,y); break; // dodaje zycie
    case 13:
        simple_bonus[2].setBonus(x,y); break; // dodaje pilke
    }
}

int symbol(int a) // potrzebne do take_step(), przesuniecie pilki osi x o 1 gdy gy direction_x jest > 0, a o -1 gdy direction_x < 0. to samo z direction_y
{
    if (a<0) return -1;
    else if (a>0) return 1;
    return 0;
}

int abs(int a)
{
    if(a<0)
        return -a;
    return a;
}
void Ball::take_step() // porusza pilke w danym kierunku
{
    if(abs(direction_x)>abs(direction_y))
    {
        if(motion_counter<=0)
        {
            motion_counter += abs(direction_x);
            y += symbol(direction_y);
        }
        x += symbol(direction_x);
        motion_counter -= abs(direction_y);
    }
    else
    {
        if(motion_counter<=0)
        {
            motion_counter += abs(direction_y);
            x += symbol(direction_x);
        }
        y += symbol(direction_y);
        motion_counter -= abs(direction_x);
    }
}

void Ball::check_collision(Paddle &paddle,Screen &screen,Construction &construction,std::vector <Powerups> &powerups,LaserPowerup &laser_powerup,std::vector <SimpleBonus> &simple_bonus)
{
    if((x <= 0 && direction_x < 0) || (x >= screen.getWidth() && direction_x > 0)) // pilka odbija sie od sian bocznych
    {
        // czasami gdy pilka odbijala sie od dolnego rogu (sciana + paletka) to znikala na 1 klatke. dzieki temu to jest naprawione
        switch(symbol(direction_x))
        {
        case 1:
            x = screen.getWidth();
            break;
        case -1:
            x = 0;
        }
        bounce_horisontal();
    }
    if(y >= screen.getHeight() && direction_y > 0 ) // sufit
        bounce_vertical();
    else if(paddle.touch(x,y) && direction_y < 0 ) // paletka
    {
        y = 1;
        direction_y = paddle.getWidth()*4; // ustawianie kierunku w zaleznosci od polozenia pilki na paletce
        direction_x = (x - paddle.getPosition())*25 + symbol(-3+rand()%7);
        if(abs(direction_x) > abs(direction_y))
            motion_counter = abs(direction_x);
        else
            motion_counter = abs(direction_y);
    }
    else if(y == 0 && direction_y < 0 && powerups[10].getTimer())
        bounce_vertical();
    if(construction.getBrick(x,y)) // gdy pilka jest na pozycji cegly
    {
        for(int h=y+powerups[4].getPower() ; h>=y-powerups[4].getPower() ; h--) // niszczenie klockow w srednicy zasiegu bonusu bomby.
            for(int w=x-powerups[4].getPower() ; w<=x+powerups[4].getPower() ; w++) // jesli bonus nie jest wlaczony to petle wykonaja sie raz i pilka zbije tylko jeden klocek
                if(!powerups[6].getTimer()) // bonus 6 to sprawia ze pilka nie zbija klockow. timer = 0 czyli bonus nie dziala, wiec pilka zbije klocek
                    construction.break_brick(w,h);

        if(!powerups[5].getTimer()) // powerup 5 pilka przechodzi przez klocki, a nie odbija sie od nich
        {
            bounce_horisontal(); // jesli jest na cegle, to musiala sie kierowac idealnie od strony rogu, wiec odbija sie o 180 stopni
            bounce_vertical();
            rand_powerup(x,y,powerups,laser_powerup,simple_bonus); // losowanie powerupow, jesli pilka przechodzi przez klocki to rozbija ich bardzo duzo, wiec nie ma potrzeby dodawania powerupow
        take_step(); // przez jedna klatke pilka byla na pozycji cegly, co wygladalo nienaturalnie
        }
    }
    else if(construction.getBrick(x+1,y) && direction_x > 0) // gdy pilka leci w prawo, i po prawej jest cegla
    {
        for(int h=y+powerups[4].getPower() ; h>=y-powerups[4].getPower() ; h--)
            for(int w=x+1-powerups[4].getPower() ; w<=x+1+powerups[4].getPower() ; w++)
                if(!powerups[6].getTimer())
                    construction.break_brick(w,h);
        if(!powerups[5].getTimer())
        {
            bounce_horisontal(); // cegla byla po prawej, wiec pilka sie odbija tylko w lewo (fizyka D:)
            rand_powerup(x+1,y,powerups,laser_powerup,simple_bonus);
        }
    }
    else if(construction.getBrick(x-1,y) && direction_x < 0) // pilka leci w lewo i po lewej jest cegla
    {
        for(int h=y+powerups[4].getPower() ; h>=y-powerups[4].getPower() ; h--)
            for(int w=x-1-powerups[4].getPower() ; w<=x-1+powerups[4].getPower() ; w++)
                if(!powerups[6].getTimer())
                    construction.break_brick(w,h);
        if(!powerups[5].getTimer())
        {
            bounce_horisontal();
            rand_powerup(x-1,y,powerups,laser_powerup,simple_bonus);
        }
    }
    if(construction.getBrick(x,y+1) && direction_y > 0) // w gore
    {
        for(int h=y+1+powerups[4].getPower() ; h>=y+1-powerups[4].getPower() ; h--)
            for(int w=x-powerups[4].getPower() ; w<=x+powerups[4].getPower() ; w++)
                if(!powerups[6].getTimer())
                    construction.break_brick(w,h);
        if(!powerups[5].getTimer())
        {
            bounce_vertical();
            rand_powerup(x,y+1,powerups,laser_powerup,simple_bonus);
        }
    }
    else if(construction.getBrick(x,y-1) && direction_y < 0)// w dol
    {
        for(int h=y-1+powerups[4].getPower() ; h>=y-1-powerups[4].getPower() ; h--)
            for(int w=x-powerups[4].getPower() ; w<=x+powerups[4].getPower() ; w++)
                if(!powerups[6].getTimer())
                    construction.break_brick(w,h);
        if(!powerups[5].getTimer())
        {
            bounce_vertical();
            rand_powerup(x,y-1,powerups,laser_powerup,simple_bonus);
        }
    }
} // i to by bylo na tyle z kolizji pilki
