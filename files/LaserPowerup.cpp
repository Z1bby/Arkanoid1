#include <vector>
#include "Powerups.h"
#include "Paddle.h"
#include "Construction.h"

void LaserPowerup::pop_laser(int a) // to samo co popBonus(int a)
{
    for(int j=a+1 ; j<laser_y.size() ; j++)
    {
        laser_y[j-1] = laser_y[j];
        laser_x[j-1] = laser_x[j];
    }
    laser_y.pop_back();
    laser_x.pop_back();
}

void LaserPowerup::laser_shot(int record,Paddle &paddle)
{
    if(record - record_shot > 500) // strzela 2 razy na sekunde
    {
        laser_x.push_back(paddle.getPosition() - paddle.getWidth()); laser_y.push_back(1); // lewa strona paletki
        laser_x.push_back(paddle.getPosition() + paddle.getWidth()); laser_y.push_back(1); // prawa strona paletki
        record_shot = record;
    }
}

void LaserPowerup::laser_move(int record,Construction &construction)
{
    for(int i=0 ; i<laser_y.size() ; i++)
    {
        laser_y[i] ++; // pocisk leci w gore
        if(laser_y[i] > height_limit) // przechodzi przez granice pudla
        {
            pop_laser(i);
            i--; // gdy pocisk jest usuwany, laser_y.size() maleje, wiec 'i' tez musi zmalec
        }
        else if(construction.getBrick(laser_x[i],laser_y[i])) // trafia w cegle
        {
            construction.break_brick(laser_x[i],laser_y[i]);
            pop_laser(i);
            i--;
        }
    }
    record_move = record;
}
