#ifndef JUEGO_HPP
#define JUEGO_HPP

#include <ncurses.h>
#include <vector>

struct Entidad {
    int x;
    int y;
    char simbolo;
};

extern int salon_actual;

void ejecutar_partida();
void dibujar_mapa(WINDOW* win);

#endif