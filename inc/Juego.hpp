#ifndef JUEGO_HPP
#define JUEGO_HPP

#include <ncurses.h>
#include <vector>

struct Entidad {
    int x;
    int y;
    char simbolo;
};

struct DatosEnemigo {
    int x;
    int y;
    char simbolo;
    int salon_pertenece;
    bool vivo;
    int direccion;
};

struct DatosProyectil {
    int x;
    int y;
    char orientacion;
    int salon_pertenece;
    bool activo;
};

const int MAX_PROYECTILES = 16;

extern int salon_actual;
extern bool tiene_llave1, tiene_llave2, tiene_llave3;
extern bool llave1_recogida, llave2_recogida, llave3_recogida;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern DatosProyectil proyectiles_enemigos[MAX_PROYECTILES];

void ejecutar_partida();
void dibujar_mapa(WINDOW* win);

void dibujar_entidad(WINDOW* win, int y, int x, char simbolo, int par_color);
void borrar_entidad(WINDOW* win, int y, int x);
void dibujar_ataque(WINDOW* win, int linkY, int linkX, char orientacion);

void* mover_enemigo(void* arg);
void* mover_proyectil(void* arg);

#endif