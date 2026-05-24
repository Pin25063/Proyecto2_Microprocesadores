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

extern bool tiene_llave1, tiene_llave2, tiene_llave3;
extern bool llave1_recogida, llave2_recogida, llave3_recogida;

void ejecutar_partida();
void dibujar_mapa(WINDOW* win);

#endif