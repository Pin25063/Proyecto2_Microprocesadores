#ifndef MENU_HPP
#define MENU_HPP

#include <ncurses.h>

void mostrar_diseno(WINDOW *ventana);
void mostrar_instrucciones();
void mostrar_puntajes_destacados();
void iniciar_juego();
void menu_principal();
void mostrar_game_over(bool victoria);
void guardar_puntaje(int puntaje_final);

#endif