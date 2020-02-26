#include <vector>
#include <iostream>

#include "Paddle.h"
#include "Ball.h"
#include "Screen.h"
#include "Construction.h"
#include "Powerups.h"

void Screen::display(Paddle &paddle,std::vector <Ball> &ball,Construction &construction,std::vector <Powerups> &powerups,LaserPowerup &laser_powerup, std::vector <SimpleBonus> &simple_bonus, int lifes)
{
    std::string board = "\n"; // duzy string, ktory bedzie tylko raz wyswietlony (konsola az tak nie miga w przeciwienstwie do wypisywania cout<<"X"; 400 razy...
    board += char(9); // tabulator = margines
    board += "Lifes: "; board += char(lifes + 48); board += '\n';
    board += char(9);
    board += char(201); // czesc pudelka, lewy gorny rog z asci
    for(int w=0 ; w<=getWidth() ; w++)
        board += char(205); // sufit
    board += char(187); // prawy gorny rog
    board += '\n';
    bool gap = true;
    int i = 0;
    for(int h=getHeight() ; h>=-10 ; h--) // punkt (x,y) to punk (w,h). h od height
    {
        board += char(9); // tabulator
        if(h>=0)
        {
            board += char(186); // lewa sciana
            for(int w=0 ; w<=getWidth() ; w++) // punkt (x,y) to punk (w,h). w od width
            {
                bool can_put = true; // dzieki temu nie wypisze pilki i bonusu, gdy sa na tej samej pozycji, tylko sama pilke. pudelko sie nie psuje
                switch(construction.getBrick(w,h)) // cegla jest na 3 uderzenia i ma 3 etapy zniszczenia.
                {
                case 1:
                    board += char(176); // rysowanie cegly
                    can_put = false; // juz nic wiecej nie moze byc narysowane
                    break;
                case 2:
                    board += char(177);
                    can_put = false;
                    break;
                case 3:
                    board += char(178);
                    can_put = false;
                }
                for(int i=0 ; i<ball.size() ; i++) // rysowanie 'i' pilek
                {
                    if(can_put && w == ball[i].getX() && h == ball[i].getY()) // w = x, h = y.
                    {
                        board += 'O';
                        can_put = false; // juz nic wiecej nie moze byc narysowane
                    }
                }
                for(int i=0 ; i<simple_bonus.size() ; i++) // rysowanie bonusow SimpleBonus
                {
                    for(int j=0 ; j<simple_bonus[i].getBonusAmount() ; j++) // kazdy powerup moze miec kilka spadajacych bonusow
                    {
                        if(can_put && w == simple_bonus[i].getX(j) && h == simple_bonus[i].getY(j)) // getX(j) pozycja wzgledem osi X dla spadajacego bonusu numer 'j'
                        {
                            board += '?'; // coz to moze byc... moze zmniejszenie paletki?
                            can_put = false;
                        }
                    }
                }

                for(int i=0 ; i<powerups.size() ; i++) // rysowanie bonusow Powerups
                {
                    for(int j=0 ; j<powerups[i].getBonusAmount() ; j++) // kazdy powerup moze miec kilka spadajacych bonusow
                    {
                        if(can_put && w == powerups[i].getX(j) && h == powerups[i].getY(j)) // getX(j) pozycja wzgledem osi X dla spadajacego bonusu numer 'j'
                        {
                            board += '?'; // coz to moze byc... moze zmniejszenie paletki?
                            can_put = false;
                        }
                    }
                }

                for(int j=0 ; j<laser_powerup.getBonusAmount() ; j++) // rysowanie bonusow LaserPowerup
                {
                    if(can_put && w == laser_powerup.getX(j) && h == laser_powerup.getY(j))
                    {
                        board += '?';
                        can_put = false;
                    }
                }
                for(int j=0 ; j<laser_powerup.getBulletsAmmount() ; j++) // rysowanie pociskow z laser_powerup
                {
                    if(can_put && w == laser_powerup.getLaserX(j) && h == laser_powerup.getLaserY(j))
                    {
                        board += char(24); // strzalka w gore w asci
                        can_put = false;
                    }
                }

                if(can_put)
                {
                    if(paddle.touch(w,h)) // paletka (funkcja sprawdza czy paletka dotyka dany punkt)
                        board += '=';
                    else if(h == 0 && powerups[10].getTimer())
                        board += '_';
                    else
                        board += ' '; // puste pole
                }
            }
            board += char(186); // prawa sciana
        }
        else
        for(int w=0 ; w<=getWidth() ; w++)
            board += ' ';
        // info o powerupach, stopery i moc

        if(gap) gap = false;
        else if(i < powerups.size())
        {
            board += char(9);
            board += powerups[i].getName();
            board += powerups[i].getTime();
            if(powerups[i].getPower() && ( i < 5 || i > 7 ) && i != 10) // 5,6,7 i 10 nie maja mocy
            {
                board += " x ";
                board += char(powerups[i].getPower() + 48);
            }
            gap = true; i++;
        }
        else if(i == powerups.size())
        {
            board += char(9);
            board += "Laser:           " + laser_powerup.getTime(); // wyjatkowy bonus
            gap = true;
            i++;
        }
        board += '\n';
    }

    std::cout<<board<<"\n";
}
