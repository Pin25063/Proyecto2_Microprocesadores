#ifndef RENDER_HPP
#define RENDER_HPP

#include <ncurses.h>
#include <string>

#include "Mapa.hpp"

extern bool tiene_llave1;
extern bool tiene_llave2;
extern bool tiene_llave3;

void dibujar_entidad(WINDOW* win, int y, int x, char simbolo, int par_color);
void borrar_entidad(WINDOW* win, int y, int x);
void dibujar_ataque(WINDOW* win, int linkY, int linkX, char orientacion);
void dibujar_mapa(WINDOW* win);

#endif