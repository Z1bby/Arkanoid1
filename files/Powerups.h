#include <iostream>
#include <vector>
class Paddle; // laser powerup musi znac polozenie paletki
class Construction; // i nieszczy cegly
class Powerups;

class SimpleBonus // zwykly spadajacy bonus, zlapanie daje bezterminowy efekt (dodanie pilki, zycia, zabranie zycia)
{
protected:
    std::vector <int> x,y;
public:
    void setBonus(int xx,int yy); // ustawianie nowego bonusa (spadajacego) w punkcie gdzie zostala zniszczona cegla
    void fall_down(); // przesuwanie bonusow o kratke w dol
    void popBonus(int a); // usuwanie bonusu z planszy (gdy paletka dotkie bonus, lub bonus spadnie na dol
    void popAll();
    int getX(int a){return x[a];} // bierze wspolrzedne n'tego bonusa
    int getY(int a){return y[a];}
    int getBonusAmount(){return y.size();} // ilosc spadajacych bonusow na planszy
};

class Powerups: public SimpleBonus // spadajacy bonus, ktory po zlapaniu daje powerup trwajacy 10 - 15 sekund
{
protected:
    std::string name; // zeby w screen.cpp wyswietlac je w petli for. Uzywane w linijce 103
    std::vector <int> x,y; // wspolzedne punktu.
    int power; // moc powerupa, speedup dziala mocniej gdy sie zbierze 2.
    int powerup_record; // odliczanie timera
    int timer; // timer
    int durance; // jak dlugo ma trwac powerup
public:
    Powerups(int d = 100)
    {
        name = "";
        timer = 0;
        powerup_record = 0;
        durance = d;
        power = 0;
    }
    ~Powerups(){}
    void turn_on(int record); // wlaczanie powerupa
    void turn_off(); // wylaczanie powerupa
    void uppdate(int record); // zwraca false jesli powerup nie jest wlaczony, a true jesli jest
    std::string getTime(); // zmiana int timer, na string timer
    int getTimer(){return timer;} // jesli timer = 0, to powerup jest wylaczony.
    int getRecord(){return powerup_record;}
    int getPower(){return power;} // moc bonusa, przy wylaczonym moc = 0
    void setName(std::string a){name = a;}
    std::string getName(){return name;}
};

class LaserPowerup: public Powerups // zmodyfikowany konkretny powerup, strzelanie z platformy
{
    std::vector <int> laser_x, laser_y; // wspolrzedne pociskow lecacych w gore
    int height_limit; // mowi gdzie jest sufit, i kiedy pociski maja sie usuwac
    int record_move,record_shot; // odliczanie czasu do wystrzelania pocisku, i do poruszania sie pocisku
    void pop_laser(int a); // usuwanie n'tego pocisku (gdy dotknie sufitu lub cegly)
public:
    LaserPowerup(int d = 100, int h = 19):Powerups()
    {
        height_limit = h;
        record_move = 0;
        record_shot = 0;
    }
    ~LaserPowerup(){}
    void laser_move(int record, Construction &construction); // poruszanie pociskiem (i niszenie cegiel)
    void laser_shot(int record,Paddle &paddle); // wystrzeliwanie pociskow
    void turn_on(int record) // zmieniony turn_on, bo dodano 2 zmienne record_*
    {
        record_move = record;
        record_shot = record;
        powerup_record = record;
        timer = durance;
        power ++;
    }
    int getBulletsAmmount(){return laser_x.size();}
    int getLaserX(int a){return laser_x[a];} // zwraca polozenie n'tego pocisku
    int getLaserY(int a){return laser_y[a];}
    int getRecordShot(){return record_shot;}
    int getRecordMove(){return record_move;}
};
