#include "Juego.hpp"
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include "Mapa.hpp"
#include "Render.hpp"
#include "Enemigos.hpp"
#include "Proyectiles.hpp"

int salon_actual = 0;

int linkX = 5;
int linkY = 2;
bool link_recibe_dano = false;
DatosProyectil proyectiles_enemigos[MAX_PROYECTILES];

bool tiene_llave1 = false, tiene_llave2 = false, tiene_llave3 = false;
bool llave1_recogida = false, llave2_recogida = false, llave3_recogida = false;

// Ejecutar partida 
void ejecutar_partida() {
    clear();
    linkX = 5;
    linkY = 2;
    link_recibe_dano = false;
    salon_actual = 0;
    tiene_llave1 = tiene_llave2 = tiene_llave3 = false;
    llave1_recogida = llave2_recogida = llave3_recogida = false;

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int alto, ancho;
    dim_salon(alto, ancho);

    WINDOW* juego_win = newwin(alto, ancho, (yMax - alto) / 2, (xMax - ancho) / 2);
    keypad(juego_win, TRUE);

    char link_char = 'v';

    const int NUM_ENEMIGOS = 16;
    DatosEnemigo enemigos[NUM_ENEMIGOS] = {
        {20, 5, 'E', 0, true, rand() % 4},
        {35, 12, 'X', 0, true, rand() % 4},
        {10, 15, 'E', 0, true, rand() % 4},
        {40, 3, 'X', 0, true, rand() % 4},

        {10, 5, 'E', 1, true, rand() % 4},
        {20, 8, 'X', 1, true, rand() % 4},
        {5, 10, 'E', 1, true, rand() % 4},
        {25, 3, 'X', 1, true, rand() % 4},

        {15, 6, 'E', 2, true, rand() % 4},
        {25, 10, 'X', 2, true, rand() % 4},
        {5, 4, 'E', 2, true, rand() % 4},
        {45, 8, 'X', 2, true, rand() % 4},

        {10, 5, 'E', 3, true, rand() % 4},
        {20, 10, 'X', 3, true, rand() % 4},
        {8, 12, 'E', 3, true, rand() % 4},
        {25, 4, 'X', 3, true, rand() % 4}
    };

    pthread_t hilos_enemigos[NUM_ENEMIGOS];

    for (int i = 0; i < NUM_ENEMIGOS; i++) {
        pthread_create(&hilos_enemigos[i], NULL, mover_enemigo, (void*)&enemigos[i]);
    }

    DatosProyectil flecha = {0, 0, '^', 0, false};
    pthread_t hilo_flecha;

    bool en_partida = true;
    while (en_partida) {
        werase(juego_win);

        dibujar_mapa(juego_win);

        if (salon_actual == 0 && !llave1_recogida) {
            wattron(juego_win, COLOR_PAIR(5));
            mvwaddch(juego_win, 9, 24, 'K');
            wattroff(juego_win, COLOR_PAIR(5));
        }
        if (salon_actual == 1 && !llave2_recogida) {
            wattron(juego_win, COLOR_PAIR(6));
            mvwaddch(juego_win, 2, 16, 'K');
            wattroff(juego_win, COLOR_PAIR(6));
        }
        if (salon_actual == 2 && !llave3_recogida) {
            wattron(juego_win, COLOR_PAIR(7));
            mvwaddch(juego_win, 2, 16, 'K');
            wattroff(juego_win, COLOR_PAIR(7));
        }

        if (link_recibe_dano) {
            wattron(juego_win, COLOR_PAIR(6)); 
            mvwaddch(juego_win, linkY, linkX, link_char);
            wattroff(juego_win, COLOR_PAIR(6));
            link_recibe_dano = false; 
        } else {
            wattron(juego_win, COLOR_PAIR(1)); 
            mvwaddch(juego_win, linkY, linkX, link_char);
            wattroff(juego_win, COLOR_PAIR(1));
        }

        for (int i = 0; i < NUM_ENEMIGOS; i++) {
            if (salon_actual == enemigos[i].salon_pertenece && enemigos[i].vivo) {
                int color_enemigo = (enemigos[i].simbolo == 'E') ? 4 : 5;
                dibujar_entidad(juego_win, enemigos[i].y, enemigos[i].x, enemigos[i].simbolo, color_enemigo);
            }
        }

        if (flecha.activo && salon_actual == flecha.salon_pertenece) {
            dibujar_entidad(juego_win, flecha.y, flecha.x, '*', 3);
        }

        for (int i = 0; i < MAX_PROYECTILES; i++) {
            if (proyectiles_enemigos[i].activo && salon_actual == proyectiles_enemigos[i].salon_pertenece) {
                dibujar_entidad(juego_win, proyectiles_enemigos[i].y, proyectiles_enemigos[i].x, '*', 6);
            }
        }

        mvprintw(yMax - 2, (xMax - 50) / 2,
                 "Utiliza W, A, S, D para moverte y presiona Q para salir");
        refresh();
        wrefresh(juego_win);
        
        int tecla = wgetch(juego_win);
        int nuevaX = linkX, nuevaY = linkY;

        switch (tecla) {
            case 'w': case 'W': nuevaY--; link_char = '^'; break;
            case 's': case 'S': nuevaY++; link_char = 'v'; break;
            case 'a': case 'A': nuevaX--; link_char = '<'; break;
            case 'd': case 'D': nuevaX++; link_char = '>'; break;
            case 'j': case 'J': dibujar_ataque(juego_win, linkY, linkX, link_char); break; 
            case 'k':
            case 'K':
                if (!flecha.activo) {
                    flecha.x = linkX;
                    flecha.y = linkY;
                    flecha.orientacion = link_char;
                    flecha.salon_pertenece = salon_actual;
                    flecha.activo = true;
                    pthread_create(&hilo_flecha, NULL, mover_proyectil, (void*)&flecha);
                }
                break;
            case 'q': case 'Q': en_partida = false;        break;
        }

        if (!en_partida) break;

        char prox = mapa_ptr()[nuevaY][nuevaX];

        if (prox == '|' || prox == '/') {
            // Determinar destino según salón actual
            bool ok = false;
            int ns = salon_actual, sx = linkX, sy = linkY;

            switch (salon_actual) {
                case 0:
                    if (nuevaX == 0 && tiene_llave1) {
                        ns = 1;
                        sx = ANCHO1 - 2;
                        sy = 19;
                        ok = true;
                    }
                    else if (nuevaX == ANCHO0 - 1 && tiene_llave2) {
                        ns = 2;
                        sx = 1;
                        sy = 8;
                        ok = true;
                    }
                    else if (nuevaY == ALTO0 - 1 && tiene_llave3) {
                        ns = 3;
                        sx = 30;
                        sy = 1;
                        ok = true;
                    }
                    break;
                case 1:
                    if (nuevaX == ANCHO1 - 1) {
                        ns = 0;
                        sx = 1;
                        sy = 19;
                        ok = true;
                    }
                    break;
                case 2:
                    if (nuevaX == 0) {
                        ns = 0;
                        sx = ANCHO0 - 2;
                        sy = 19;
                        ok = true;
                    }
                    break;
                case 3:
                    if (nuevaY == 0) {
                        ns = 0;
                        sx = 40;
                        sy = ALTO0 - 2;
                        ok = true;
                    }
                    break;
            }

            if (ok) {
                salon_actual = ns;
                linkX = sx;
                linkY = sy;
                dim_salon(alto, ancho);
                delwin(juego_win);
                clear(); refresh();
                juego_win = newwin(alto, ancho, (yMax - alto) / 2, (xMax - ancho) / 2);
                keypad(juego_win, TRUE);
            }
        } else if (prox != '#' && prox != 'E' && prox != 'X' && prox != '|' && prox != '/') {
            linkX = nuevaX;
            linkY = nuevaY;
            if (salon_actual == 0 && linkX == 24 && linkY == 9 && !llave1_recogida)
                { llave1_recogida = true; tiene_llave1 = true; }
            if (salon_actual == 1 && linkX == 16 && linkY == 2 && !llave2_recogida)
                { llave2_recogida = true; tiene_llave2 = true; }
            if (salon_actual == 2 && linkX == 16 && linkY == 2 && !llave3_recogida)
                { llave3_recogida = true; tiene_llave3 = true; }
        }
    }

    for (int i = 0; i < NUM_ENEMIGOS; i++) {
        enemigos[i].vivo = false;
        pthread_join(hilos_enemigos[i], NULL);
    }

    if (flecha.activo) {
        flecha.activo = false;
        pthread_join(hilo_flecha, NULL);
    }

    werase(juego_win);
    wrefresh(juego_win);
    delwin(juego_win);

    clear();
    refresh();
}