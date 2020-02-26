#include <vector> // musi losowac sposrod wektora powerupow
class Paddle; // musi widziec klasy, zeby dzialala funkcja check collision
class Screen;
class Construction;
class SimpleBonus;
class Powerups;
class LaserPowerup;

class Ball
{
    int x,y;
    int direction_x,direction_y,motion_counter; // potrzebne w funkcji take_step()
public:
    Ball(int xx=9,int yy=1)
    {
        x = xx;
        y = yy;
        direction_x = 0; direction_y = -1; motion_counter = 0; // gdy direction_x > 0 to leci w prawo, gdy < 0 leci w lewo, a gdy = 0 to nie zmienia pozycji wzgledem osi x;
    } //                                                          gdy direction_y > 0 to leci w gore , gdy < 0 leci w dol, a gdy = 0 to nie zmienia pozycji wzgledem osi y;
    ~Ball(){}

    int getX(){return x;}
    int getY(){return y;}
    int getDirectionX(){return direction_x;}
    int getDirectionY(){return direction_y;}
    void bounce_vertical(){direction_y = -direction_y;} // przy odbiciu zmienia kierunek
    void bounce_horisontal(){direction_x = -direction_x;}
    void take_step(); // algorytm na zmienianie pozycji pilki w linii prostej
    void check_collision(Paddle &paddle,Screen &screen,Construction &construction,std::vector <Powerups> &powerups,LaserPowerup &laser_powerup,std::vector <SimpleBonus> &simple_bonus);
    // odbijanie pilki od scian, paletki i cegiel.
};
