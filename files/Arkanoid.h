#include <vector>
class Paddle;
class Ball;
class Screen;
class Construction;
class SimpleBonus;
class Powerups;
class LaserPowerup;

class Arkanoid
{
    int speed; // szybkosc pilki
    int lifes;
public:
    Arkanoid(int s = 100,int l = 3)
    {
        speed = s;
        lifes = l;
    }
    ~Arkanoid(){}
    int getSpeed(){return speed;}
    void setSpeed(int a = 100){speed = a;}
    int getLifes(){return lifes;}
    void changeLifes(int a=-1){lifes += a;}
    void run();
    void powerup_collision(int width,int &record,std::vector <Ball> &ball,Paddle &paddle,std::vector <Powerups> &powerups, LaserPowerup &laser_powerup, std::vector <SimpleBonus> &simple_bonus); // paletka vs spadajace bonusy
    void gameover_animation(Paddle &paddle, Screen &screen, Construction &construction);
};
