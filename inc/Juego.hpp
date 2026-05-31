/*
Archivo principal de definiciones compartidas del juego
Contiene las estructuras de datos utilizadas por los enemigos,
proyectiles y demás elementos del mapa, así como las variables globales,
y mutex de sincronización
*/

#ifndef JUEGO_HPP
#define JUEGO_HPP

#include <ncurses.h>
#include <vector>
#include <pthread.h>
#include <string>

struct Entidad { // Estructura base para jugador y enemigos
    int x;
    int y;
    char simbolo;
};

struct DatosEnemigo { // Información asociada a cada enemigo
    int x;
    int y;
    char simbolo;
    int salon_pertenece;
    bool vivo;
    int direccion;
    int id;
    int vida;
}; // Cada enemigo es controlado por un hilo independiente

struct DatosProyectil { // información usada por los proyectiles de enemigos y jugador
    int x;
    int y;
    char orientacion;
    int salon_pertenece;
    bool activo;
    bool es_enemigo;
};

const int MAX_PROYECTILES = 16; //cantidad máxima de proyectiles simultáneos
const int VELOCIDAD_ENEMIGOS = 200000; // tiempo de espera para controlar la velocidad de enemigos
const int NUM_ENEMIGOS = 18;

// Variables locales del estado actual de la partida
extern pthread_mutex_t mutex_jugador;
extern pthread_mutex_t mutex_proyectiles;
extern pthread_mutex_t mutex_enemigos;
extern pthread_mutex_t mutex_salon;

extern int salon_actual;
extern bool tiene_llave1, tiene_llave2, tiene_llave3;
extern bool llave1_recogida, llave2_recogida, llave3_recogida;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern DatosProyectil proyectiles_enemigos[MAX_PROYECTILES];
extern DatosEnemigo enemigos[NUM_ENEMIGOS];
extern int puntaje;
extern std::string nombre_jugador;

bool ejecutar_partida(bool modo_guiado); //ejecuta la partida completa, retorna true si se desea reiniciar
void dibujar_mapa(WINDOW* win); // dibuja el mapa correspondiente al salón actual

// funciones de renderizado
void dibujar_entidad(WINDOW* win, int y, int x, char simbolo, int par_color);
void borrar_entidad(WINDOW* win, int y, int x);
void dibujar_ataque(WINDOW* win, int linkY, int linkX, char orientacion);

void* mover_enemigo(void* arg);
void* mover_proyectil(void* arg);

#endif