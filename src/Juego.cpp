#include "Juego.hpp"
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include "Mapa.hpp"
#include "Render.hpp"
#include "Enemigos.hpp"
#include "Proyectiles.hpp"
#include <ctime>
#include "Menu.hpp"

pthread_mutex_t mutex_jugador = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_proyectiles = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_enemigos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_salon = PTHREAD_MUTEX_INITIALIZER;

int salon_actual = 0;
int linkX = 5;
int linkY = 2;
bool link_recibe_dano = false;
DatosProyectil proyectiles_enemigos[MAX_PROYECTILES];
DatosEnemigo enemigos[NUM_ENEMIGOS];

int vida_link = 5;
int puntaje = 0;

bool invulnerable = false;
int frames_invulnerable = 0;

bool tiene_llave1 = false, tiene_llave2 = false, tiene_llave3 = false;
bool llave1_recogida = false, llave2_recogida = false, llave3_recogida = false;

void ejecutar_partida(bool modo_guiado) {
    srand(time(NULL));
    clear();
    linkX = 5;
    linkY = 2;
    link_recibe_dano = false;
    salon_actual = 0;
    tiene_llave1 = tiene_llave2 = tiene_llave3 = false;
    llave1_recogida = llave2_recogida = llave3_recogida = false;
    vida_link = 5;
    puntaje = 0;
    invulnerable = false;
    frames_invulnerable = 0;
    puntaje = 0;

    for (int i = 0; i < MAX_PROYECTILES; i++) {
        proyectiles_enemigos[i].activo = false;
        proyectiles_enemigos[i].es_enemigo = true;
    }

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int alto, ancho;
    dim_salon(alto, ancho);

    WINDOW* juego_win = newwin(alto, ancho, (yMax - alto) / 2, (xMax - ancho) / 2);
    keypad(juego_win, TRUE);
    nodelay(juego_win, TRUE);

    char link_char = 'v';

    DatosEnemigo enemigos_temp[NUM_ENEMIGOS] = {
        {20, 5, 'E', 0, true, rand() % 4, 0, 3},
        {35, 12, 'X', 0, true, rand() % 4, 1, 3},
        {10, 15, 'E', 0, true, rand() % 4, 2, 3},
        {40, 3, 'X', 0, true, rand() % 4, 3, 3},

        {10, 5, 'E', 1, true, rand() % 4, 4, 3},
        {20, 8, 'X', 1, true, rand() % 4, 5, 3},
        {5, 10, 'E', 1, true, rand() % 4, 6, 3},
        {25, 3, 'X', 1, true, rand() % 4, 7, 3},

        {15, 6, 'E', 2, true, rand() % 4, 8, 3},
        {25, 10, 'X', 2, true, rand() % 4, 9, 3},
        {5, 4, 'E', 2, true, rand() % 4, 10, 3},
        {45, 8, 'X', 2, true, rand() % 4, 11, 3},

        {10, 5, 'E', 3, true, rand() % 4, 12, 3},
        {20, 10, 'X', 3, true, rand() % 4, 13, 3},
        {8, 12, 'E', 3, true, rand() % 4, 14, 3},
        {25, 4, 'X', 3, true, rand() % 4, 15, 3},
        
        {15, 10, 'X', 4, true, rand() % 4, 16, 10}, 
        {35, 10, 'X', 4, true, rand() % 4, 17, 3}
    };
    for (int i = 0; i < NUM_ENEMIGOS; i++) {
        enemigos[i] = enemigos_temp[i];
    }

    pthread_t hilos_enemigos[NUM_ENEMIGOS];

    for (int i = 0; i < NUM_ENEMIGOS; i++) {
        pthread_create(&hilos_enemigos[i], NULL, mover_enemigo, (void*)&enemigos[i]);
    }

    DatosProyectil flecha = {0, 0, '^', 0, false};
    pthread_t hilo_flecha;

    char comandos_tutorial[] = {
        'd','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d', 
        's','s','s','s','s','s','s', 
        
        
        's','s','s','s','s','s','s','s','s','s', 
        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
        
        'w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w', 
        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a', 
        
        'd','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d', 
        's','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s', 
        'd', 

        'd','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d', 

        'w','w','w','w','w','w', 
        'd','d','d','d','d','d','d','d','d','d','d','d','d','d','d', 

        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a', 
        's','s','s','s','s','s', 
        'a', 

        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a', 
        's','s','s','s','s','s','s','s', 
        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a', 
        's','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s', 
        'd','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d','d', 
        's' 
    };
    int idx_tutorial = 0;
    bool victoria = false;
    bool en_partida = true;
    
    int frame = 0;
    while (en_partida) {
        frame++;
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

        pthread_mutex_lock(&mutex_jugador);
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
        pthread_mutex_unlock(&mutex_jugador);

        pthread_mutex_lock(&mutex_enemigos);
        for (int i = 0; i < NUM_ENEMIGOS; i++) {
            if (salon_actual == enemigos[i].salon_pertenece && enemigos[i].vivo) {
                int color_enemigo = (enemigos[i].simbolo == 'E') ? 4 : 5;
                dibujar_entidad(juego_win, enemigos[i].y, enemigos[i].x, enemigos[i].simbolo, color_enemigo);
            }
        }
        pthread_mutex_unlock(&mutex_enemigos);

        if (flecha.activo && salon_actual == flecha.salon_pertenece) {
            dibujar_entidad(juego_win, flecha.y, flecha.x, '*', 3);
        }

        pthread_mutex_lock(&mutex_proyectiles);
        for (int i = 0; i < MAX_PROYECTILES; i++) {
            if (proyectiles_enemigos[i].activo && salon_actual == proyectiles_enemigos[i].salon_pertenece) {
                dibujar_entidad(juego_win, proyectiles_enemigos[i].y, proyectiles_enemigos[i].x, '*', 6);
            }
        }
        pthread_mutex_unlock(&mutex_proyectiles);

        mvprintw(yMax - 2, (xMax - 50) / 2,
                 "Utiliza W, A, S, D para moverte y presiona Q para salir");

        pthread_mutex_lock(&mutex_jugador);
        int vida_actual = vida_link;
        pthread_mutex_unlock(&mutex_jugador);

        move(yMax - 4, xMax / 2 - 40);
        clrtoeol();

        wattron(stdscr, COLOR_PAIR(6));
        std::string corazones = "";
        for (int i = 0; i < vida_actual; i++) {
            corazones += "<3 ";
        }
        mvprintw(yMax - 4, xMax / 2 - 40, "Vida: %s", corazones.c_str());
        wattroff(stdscr, COLOR_PAIR(6));

        wattron(stdscr, COLOR_PAIR(4));
        mvprintw(yMax - 4, xMax / 2 + 26, "Puntaje: %d", puntaje);
        wattroff(stdscr, COLOR_PAIR(4));
        refresh();
        wrefresh(juego_win);

        int tecla_real = wgetch(juego_win);
        if (tecla_real == 'q' || tecla_real == 'Q') {
            en_partida = false;
            break;
        }

        int tecla;
        if (modo_guiado) {
            usleep(200000);
            tecla = comandos_tutorial[idx_tutorial];
            idx_tutorial = (idx_tutorial + 1) % (int)sizeof(comandos_tutorial);
        } else {
            tecla = wgetch(juego_win);
        }
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
                    flecha.es_enemigo = false;
                    pthread_create(&hilo_flecha, NULL, mover_proyectil, (void*)&flecha);
                }
                break;
            case 'q': case 'Q': en_partida = false;        break;
        }

        if (!en_partida) break;

        if (vida_link <= 0) {
            en_partida = false;
        }

        if (nuevaY >= 0 && nuevaX >= 0 && nuevaY < alto && nuevaX < ancho) {

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
                    else if (nuevaY == ALTO3 - 1) {
                        ns = 4;
                        sx = 25; 
                        sy = 1;
                        ok = true;
                    }
                    break;
            }

            if (ok) {
                pthread_mutex_lock(&mutex_salon);
                salon_actual = ns;
                puntaje += 25;
                pthread_mutex_unlock(&mutex_salon);
                linkX = sx;
                linkY = sy;
                dim_salon(alto, ancho);
                delwin(juego_win);
                clear(); refresh();
                juego_win = newwin(alto, ancho, (yMax - alto) / 2, (xMax - ancho) / 2);
                keypad(juego_win, TRUE);
                nodelay(juego_win, TRUE);
            }
        } else if (prox != '#' && prox != 'E' && prox != 'X' && prox != '|' && prox != '/') {
            pthread_mutex_lock(&mutex_jugador);
            linkX = nuevaX;
            linkY = nuevaY;
            pthread_mutex_unlock(&mutex_jugador);
            
            if (salon_actual == 4 && prox == 'V') {
                puntaje += 100; // Bonus por pasarse el juego
                victoria = true;
                en_partida = false;
            }
            
            if (salon_actual == 0 && linkX == 24 && linkY == 9 && !llave1_recogida) {
                llave1_recogida = true;
                tiene_llave1 = true;
                puntaje += 20;
            }
            if (salon_actual == 1 && linkX == 16 && linkY == 2 && !llave2_recogida) {
                llave2_recogida = true;
                tiene_llave2 = true;
                puntaje += 20;
            }
            if (salon_actual == 2 && linkX == 16 && linkY == 2 && !llave3_recogida) {
                llave3_recogida = true;
                tiene_llave3 = true;
                puntaje += 20;
            }
        }

        }

        if (invulnerable) {
            frames_invulnerable++;

            if (frames_invulnerable > 30) {
                invulnerable = false;
                frames_invulnerable = 0;
            }
        }
        usleep(33000);
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
    
    guardar_puntaje(puntaje); 

    if (victoria) {
        mostrar_game_over(true);
        getch();
    } else {
        mostrar_game_over(false); 
        getch();
    }
}