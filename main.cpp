#include "files/Arkanoid.h"

int main()
{
    // ksiazka pod tytulem "komentarze do pong_2.cbp"

    /// TODO - Simple bonus - naladowanie pilki na mocniejsze uderzenie
    ///        Nowy Powerup - sterowanie paletka u gory (dodanie 1/2 wysokosci i nowa pilke. bonusy odbite od 2 pilki leca w gore)
    ///        Animacje przy zbieraniu bonusow.
    ///        Dodanie hp paletki
    ///        Simple bonus - zmiana zabierajacego zycie na zabierajacy hp
    ///        Simple bonus - dodajacy hp
    ///        Powerups - pilka zabierajaca hp
    ///        Powerups - pilka dodajaca hp
    ///        Powerups - co sekunde odwraca kolory

    Arkanoid game(65,2); // gra(szybkosc pilki, zycia)
    game.run();
    return 0;
}
