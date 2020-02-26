class Paddle
{
    int position, _width; // pozycja na planszy (x, y=0), szerokosc od srodka paletki do boku (min 1)
    int speed;
    int health; // bedzie uzywane pozniej
public:
    Paddle(int p=9,int w=3)
    {
        health = 10;
        speed = 120;
        position = p;
        _width = w;
    }
    ~Paddle(){}

    int getPosition(){return position;}
    int getWidth(){return _width;}
    int getSpeed(){return speed;}
    void setSpeed(int a){speed = a;}
    void move_left(){position--;}
    void move_right(){position++;}
    void setPosition(int a=9){position = a;} // uzywane przy utracie zycia
    void setWidth(int a=3){if(a>0) _width = a;} // uzywane przy utracie zycia
    bool touch(int x,int y = 0) // zwraca true jesli punkt (x, y) znajduje sie w paletce (paletka odbija pilke, lapie bonus)
    {
        if(x >= getPosition() - getWidth() && x <= getPosition() + getWidth() && y == 0)
            return true;
        return false;
    }
    void change_health(int a = -1){health += a;}
};
