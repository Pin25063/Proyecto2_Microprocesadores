#include "Juego.hpp"
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>

//  Dimensiones de cada salón 
static const int ALTO0 = 28, ANCHO0 = 80;
static const int ALTO1 = 22, ANCHO1 = 60;
static const int ALTO2 = 22, ANCHO2 = 60;
static const int ALTO3 = 22, ANCHO3 = 60;

// Mapas estáticos 
// Salon 0: Principal
static const std::string mapa0[ALTO0] = {
    "################################################################################",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "#         ##########                              ##########                   #",
    "#         ##########                              ##########                   #",
    "#                                                                              #",
    "#                                                                              #",
    "#                             ###########                                      #",
    "#                             ###########                                      #",
    "#                                                                              #",
    "#                                                                              #",
    "#      ###########                                           ###########       #",
    "#      ###########                                           ###########       #",
    "#                                                                              #",
    "#                                                                              #",
    "#                           #####################                              #",
    "#                           #####################                              #",
    "#                                                                              #",
    "|                                                                              |",
    "#                                                                              #",
    "#                                                                              #",
    "#         ##########                              ##########                   #",
    "#         ##########                              ##########                   #",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "########################################|#######################################"
};

// Salon 1: Izquierda 
static const std::string mapa1[ALTO1] = {
    "############################################################",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          #",
    "#                                                          #",
    "#                 #########################                #",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          #",
    "#                                                          #",
    "#                 #########################                #",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          |",
    "#                                                          #",
    "############################################################"
};

// Salon 2: Derecha 
static const std::string mapa2[ALTO2] = {
    "############################################################",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          #",
    "#                                                          #",
    "#             ###############################              #",
    "|             ###############################              #",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          #",
    "#                                                          #",
    "#             ###############################              #",
    "#                                                          #",
    "#                                                          #",
    "#        ###########                 ###########           #",
    "#        ###########                 ###########           #",
    "#                                                          #",
    "############################################################"
};

// Salon 3: Abajo
static const std::string mapa3[ALTO3] = {
    "##############################|#############################",
    "#                                                          #",
    "#                                                          #",
    "#          #####################################           #",
    "#          #####################################           #",
    "#                                                          #",
    "#                                                          #",
    "#               ###########################                #",
    "#                                                          #",
    "#                                                          #",
    "#          #####################################           #",
    "#          #####################################           #",
    "#                                                          #",
    "#                                                          #",
    "#               ###########################                #",
    "#                                                          #",
    "#                                                          #",
    "#          #####################################           #",
    "#          #####################################           #",
    "#                                                          #",
    "#                                                          #",
    "############################################################"
};

int salon_actual = 0;

int linkX = 5;
int linkY = 2;
bool link_recibe_dano = false;
const int MAX_PROYECTILES = 16;
DatosProyectil proyectiles_enemigos[MAX_PROYECTILES];

bool tiene_llave1 = false, tiene_llave2 = false, tiene_llave3 = false;
bool llave1_recogida = false, llave2_recogida = false, llave3_recogida = false;

static const std::string* mapa_ptr() {
    switch (salon_actual) {
        case 1:  return mapa1;
        case 2:  return mapa2;
        case 3:  return mapa3;
        default: return mapa0;
    }
}

static void dim_salon(int& alto, int& ancho) {
    switch (salon_actual) {
        case 1:  alto = ALTO1; ancho = ANCHO1; return;
        case 2:  alto = ALTO2; ancho = ANCHO2; return;
        case 3:  alto = ALTO3; ancho = ANCHO3; return;
        default: alto = ALTO0; ancho = ANCHO0;
    }
}

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
    if (orientacion == '^') {
        mvwaddch(win, linkY - 1, linkX, '|'); 
    } else if (orientacion == 'v') {
        mvwaddch(win, linkY + 1, linkX, '|'); 
    } else if (orientacion == '<') {
        mvwaddch(win, linkY, linkX - 1, '-'); 
    } else if (orientacion == '>') {
        mvwaddch(win, linkY, linkX + 1, '-'); 
    }
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

void* mover_enemigo(void* arg) {
    DatosEnemigo* enemigo = (DatosEnemigo*)arg;

    while (enemigo -> vivo) {
        if (salon_actual == enemigo->salon_pertenece) {
            int direccion = rand() % 4;
            int nuevaX = enemigo-> x;
            int nuevaY = enemigo-> y;

            if (direccion == 0) {
                nuevaY--;
            } else if (direccion == 1) {
                nuevaY++;
            } else if (direccion == 2) {
                nuevaX--;
            } else if (direccion == 3) {
                nuevaX++;
            }

            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            if (nuevaX == linkX && nuevaY == linkY) {
                link_recibe_dano = true;

                if (direccion == 0) {
                    enemigo->y++;
                } else if (direccion == 1) {
                    enemigo->y--;
                } else if (direccion == 2) {
                    enemigo->x++;
                } else if (direccion == 3) {
                    enemigo->x--;
                }
            } else if (sig_posicion != '#' && sig_posicion != '|') {
                enemigo-> x = nuevaX;
                enemigo-> y = nuevaY;
            }

            if (enemigo->simbolo == 'E' && (rand() % 100 < 5)) {
                for (int i = 0; i < MAX_PROYECTILES; i++) {
                    if (!proyectiles_enemigos[i].activo) {
                        proyectiles_enemigos[i].x = enemigo->x;
                        proyectiles_enemigos[i].y = enemigo->y;
                        proyectiles_enemigos[i].salon_pertenece = enemigo->salon_pertenece;
                        
                        if (direccion == 0) proyectiles_enemigos[i].orientacion = '^';
                        else if (direccion == 1) proyectiles_enemigos[i].orientacion = 'v';
                        else if (direccion == 2) proyectiles_enemigos[i].orientacion = '<';
                        else if (direccion == 3) proyectiles_enemigos[i].orientacion = '>';
                        
                        proyectiles_enemigos[i].activo = true;
                        
                        pthread_t hilo_flecha_enemiga;
                        pthread_create(&hilo_flecha_enemiga, NULL, mover_proyectil, (void*)&proyectiles_enemigos[i]);
                        pthread_detach(hilo_flecha_enemiga);
                        break;
                    }
                }
            }
        }
        usleep(500000);
    }
    return nullptr;
}

void* mover_proyectil(void* arg) {
    DatosProyectil* proyectil = (DatosProyectil*) arg;

    while (proyectil -> activo) {
        if (salon_actual == proyectil -> salon_pertenece) {
            int nuevaX = proyectil->x;
            int nuevaY = proyectil->y;

            if (proyectil ->orientacion == '^') {
                nuevaY--;
            } else if (proyectil -> orientacion == 'v') {
                nuevaY++;
            } else if (proyectil -> orientacion == '<') {
                nuevaX--;
            } else if (proyectil -> orientacion == '>') {
                nuevaX++;
            }

            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            if (nuevaX == linkX && nuevaY == linkY) {
                link_recibe_dano = true;
                proyectil ->activo = false;
            } else if (sig_posicion == '#' || sig_posicion == '|') {
                proyectil -> activo = false;
            } else {
                proyectil -> x = nuevaX;
                proyectil ->y = nuevaY;
            }
        }
        usleep(100000);
    }
    return nullptr;
}




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
        {20, 5, 'E', 0, true}, {35, 12, 'X', 0, true}, {10, 15, 'E', 0, true}, {40, 3, 'X', 0, true},
        {10, 5, 'E', 1, true}, {20, 8, 'X', 1, true}, {5, 10, 'E', 1, true}, {25, 3, 'X', 1, true},
        {15, 6, 'E', 2, true}, {25, 10, 'X', 2, true}, {5, 4, 'E', 2, true}, {28, 8, 'X', 2, true},
        {10, 5, 'E', 3, true}, {20, 10, 'X', 3, true}, {8, 12, 'E', 3, true}, {25, 4, 'X', 3, true}
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