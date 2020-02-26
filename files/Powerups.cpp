#include <vector>
#include <windows.h>
#include "Powerups.h"

void SimpleBonus::setBonus(int xx,int yy) // dodanie bonusu
{
    x.push_back(xx);
    y.push_back(yy);
}

void SimpleBonus::fall_down()
{
    for(int i=0 ; i<y.size() ; i++)
    {
        y[i]--;
        if(y[i] < 0)
            popBonus(i);
    }
}

void SimpleBonus::popBonus(int a)
{
    for(int i=a+1 ; i<y.size() ; i++) // przesuwa wszystkie bonusy o szufladke w lewo, nadpisujac bonus 'a'
    {
        y[i-1] = y[i];
        x[i-1] = x[i];
    }
    x.pop_back(); // ostatni bonus jest na ostatniej i przedostatniej pozycji, wiec moze byc usuniety
    y.pop_back();
}

void SimpleBonus::popAll()
{
    for(int i=0 ; i<y.size() ; i++) // przesuwa wszystkie bonusy o szufladke w lewo, nadpisujac bonus 'a'
    {
        x.pop_back();
        y.pop_back();
    }
}

void Powerups::turn_on(int record)
{
    powerup_record = record;
    timer = durance;
    power ++; // jesli powerup jeszcze dziala, to przy zebraniu kolejnego, stoper wraca do 10s (albo 15s), a power rosnie z 1 do 2
}

void Powerups::turn_off() // zeruje timery i usuwa bonusy z planszy (funkcja uruchamiana przy utracie zycia)
{
    powerup_record = 0;
    timer = 0;
    power = 0;
    popAll();
}

void Powerups::uppdate(int record) // odswierzanie timera
{
    timer = durance - ((record - powerup_record)/100);
    if(timer<1)
    {
        powerup_record = 0;
        timer = 0;
        power = 0;
    }
}

std::string Powerups::getTime()
{
    if(timer<=0) return "---";
    std::string rev_time = "";
    rev_time += char((timer%10)+48);
    rev_time += '.';
    int tmp = timer/10;
    while(tmp)
    {
        rev_time += char((tmp%10)+48);
        tmp /= 10;
    }
    std::string time = "";
    if(rev_time.size() < 3) time += "0";
    for(int i=rev_time.size() - 1 ; i>=0 ; i--)
    {
        time += rev_time[i];
    }
    return time;
}
