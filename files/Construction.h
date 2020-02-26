#include <vector>
class Construction
{
    int _width, _height;
    std::vector<std::vector<int> > bricks; // duzy wektor bricks[x][y] = cegla/klocek do zniszczenia
public:
    Construction(int w=19,int h=19)
    {
        _width = w;
        _height = h;
        bricks.resize(w+1); // gdy plansza ma szerokosc 20 to liczy sie od 0 do _width = 19. dlatego tu jest + 1.
        for (int x=0 ; x<=w ; x++)
        {
            bricks[x].resize(h+1);
            for(int y=h/2 ; y<=h ; y++)
                if(x>1 && x<_width-1 && y>=_height/2 && y<_height-1) // wypelnianie planszy ceglami, uwzgledniajac marginesy
                    bricks[x][y] = 2; // wytrzymalosc 1 cegly
                else
                    bricks[x][y] = 0;
        }
    }
    ~Construction(){}
    int getBrick(int x,int y) // zwraca wytrzymalosc cegly, lub 0 gdy wspolrzedna wychodzi poza obszar
    {
        if(x>=0 && x<=_width && y>=_height/2 && y<=_height)
            return bricks[x][y];
        return 0;
    }
    void break_brick(int x,int y) // zmniejsza wytrzymalosc
    {
        if(x>1 && x<_width - 1 && y>=_height/2 && y<_height-1) // koniecznie sprawdza czy punkt znajduje sie w wektorze
            if(bricks[x][y])
                bricks[x][y]--;
    }
    bool bricks_destroyed()
    {
        for(int h = _height/2 ; h<_height-1 ; h++)
            for(int w = 2 ; w<_width-1 ; w++)
                if(bricks[w][h]) return false;
        return true;
    }
};
