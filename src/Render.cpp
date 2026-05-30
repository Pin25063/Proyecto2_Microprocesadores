#include "Render.hpp"

extern bool tiene_llave1;
extern bool tiene_llave2;
extern bool tiene_llave3;
#include "Juego.hpp"

void dibujar_entidad(WINDOW* win, int y, int x, char simbolo, int par_color) {
    wattron(win, COLOR_PAIR(par_color));
    mvwaddch(win, y, x, simbolo);
    wattroff(win, COLOR_PAIR(par_color));
}

void borrar_entidad(WINDOW* win, int y, int x) {
    const std::string* mapa_actual = mapa_ptr();
    char fondo = mapa_actual[y][x];
    mvwaddch(win, y, x, fondo);
}

void dibujar_ataque(WINDOW* win, int linkY, int linkX, char orientacion) {
    wattron(win, COLOR_PAIR(2)); 
    int ataqueX = linkX;
    int ataqueY = linkY;

    if (orientacion == '^') {
        mvwaddch(win, linkY - 1, linkX, '|');
        ataqueY--;
    } else if (orientacion == 'v') {
        mvwaddch(win, linkY + 1, linkX, '|'); 
        ataqueY++;
    } else if (orientacion == '<') {
        mvwaddch(win, linkY, linkX - 1, '-'); 
        ataqueX--;
    } else if (orientacion == '>') {
        mvwaddch(win, linkY, linkX + 1, '-');
        ataqueX++; 
    }

    pthread_mutex_lock(&mutex_enemigos);

    for (int i = 0; i < NUM_ENEMIGOS; i++) {
        if (!enemigos[i].vivo) {
            continue;
        }

        if (enemigos[i].salon_pertenece != salon_actual) {
            continue;
        }

        if (enemigos[i].x == ataqueX && enemigos[i].y == ataqueY) {
            enemigos[i].vida--;

            if (enemigos[i].vida > 0) {
                puntaje += 5;
            } else {
                enemigos[i].vivo = false;
                puntaje += 10;
            }
        }
    }

    pthread_mutex_unlock(&mutex_enemigos);
    wattroff(win, COLOR_PAIR(2));
    
    wrefresh(win); 
    napms(150);   
}

//  Dibujar mapa 
void dibujar_mapa(WINDOW* win) {
    int alto, ancho;
    dim_salon(alto, ancho);
    const std::string* mapa = mapa_ptr();

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            char c = mapa[y][x];

            if (c == '#') {
                wattron(win, COLOR_PAIR(2));
                mvwaddch(win, y, x, c);
                wattroff(win, COLOR_PAIR(2));
            }
            else if (c == '|' || c == '/') {
                int par;

                if (salon_actual == 0) {
                    if (x == 0) {
                        par = 5;
                        if (tiene_llave1)
                            c = '/';
                        else
                            c = '|';
                    }
                    else if (x == ANCHO0 - 1) {
                        par = 6;

                        if (tiene_llave2)
                            c = '/';
                        else
                            c = '|';
                    }
                    else {
                        par = 7;

                        if (tiene_llave3)
                            c = '/';
                        else
                            c = '|';
                    }
                }
                else if (salon_actual == 1) {
                    par = 5;
                    c = '/';
                }
                else if (salon_actual == 2) {
                    par = 6;
                    c = '/';
                }
                else {
                    par = 7;
                    c = '/';
                }

                wattron(win, COLOR_PAIR(par));
                mvwaddch(win, y, x, c);
                wattroff(win, COLOR_PAIR(par));
            } else if (c == 'E' || c == 'X') {
                wattron(win, COLOR_PAIR(4));
                mvwaddch(win, y, x, c);
                wattroff(win, COLOR_PAIR(4));
            } else {
                mvwaddch(win, y, x, c);
            }
        }
    }
}