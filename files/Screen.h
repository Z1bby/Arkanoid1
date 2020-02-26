#include <vector> // musi rysowac wektor pilek i powerupow
class Paddle; // bedzie rysowac wszystkie elementy
class Ball;
class Construction;
class SimpleBonus;
class Powerups;
class LaserPowerup;

class Screen
{
    int _width,_height;
public:
    Screen(int w=19,int h=19)
    {
        _width = w;
        _height = h;
    }
    ~Screen(){}

    int getWidth()const{return _width;}
    int getHeight()const{return _height;}
    void display(Paddle &paddle,std::vector <Ball> &ball,Construction &construction,std::vector <Powerups> &powerups,LaserPowerup &laser_powerup,std::vector <SimpleBonus> &simple_bonus,int lifes);
    // ^ rysowanie wszystkich elementow
};
