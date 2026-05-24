#include "Juego.hpp"
#include <string>

//  Dimensiones de cada salón 
static const int ALTO0 = 19, ANCHO0 = 48;
static const int ALTO1 = 14, ANCHO1 = 33;
static const int ALTO2 = 14, ANCHO2 = 33;
static const int ALTO3 = 14, ANCHO3 = 33;

// Mapas estáticos 
// Salon 0: Principal (48x19). Puertas: izq (9,0), der (9,47), abajo (18,24)
static const std::string mapa0[ALTO0] = {
    "################################################",
    "#                                              #",
    "#                                              #",
    "#         ###                   ####           #",
    "#        ######               ########         #",
    "#                                              #",
    "#                              ##              #",
    "#                            #####             #",
    "#                                              #",
    "D         ######                               D",
    "#       #########                   #####      #",
    "#                                              #",
    "#                                              #",
    "#                            ######            #",
    "#                          ##########          #",
    "#        #######                               #",
    "#      ###########                             #",
    "#                                              #",
    "########################D#######################"
};

// Salon 1: Izquierda (35x14). Puerta: der (7,34) → vuelve al principal
static const std::string mapa1[ALTO1] = {
    "#################################",
    "#                               #",
    "#                               #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#      ####           ####      D",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#                               #",
    "#                               #",
    "#################################"
};

// Salon 2: Derecha (35x14). Puerta: izq (7,0) → vuelve al principal
static const std::string mapa2[ALTO2] = {
    "#################################",
    "#                               #",
    "#                               #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#                               #",
    "#      ###################      #",
    "D      ###################      #",
    "#                               #",
    "#      ####           ####      #",
    "#      ####           ####      #",
    "#                               #",
    "#                               #",
    "#################################"
};

// Salon 3: Abajo (35x14). Puerta: arriba (0,17) → vuelve al principal
static const std::string mapa3[ALTO3] = {
    "################D################",
    "#                               #",
    "#                               #",
    "#      ###################      #",
    "#      ###################      #",
    "#                               #",
    "#      ###################      #",
    "#      ###################      #",
    "#                               #",
    "#      ###################      #",
    "#      ###################      #",
    "#                               #",
    "#                               #",
    "#################################"
};

int salon_actual = 0;

//  Helpers internos 
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
            } else if (c == 'D') {
                wattron(win, COLOR_PAIR(3));
                mvwaddch(win, y, x, c);
                wattroff(win, COLOR_PAIR(3));
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

// Ejecutar partida 
void ejecutar_partida() {
    clear();
    salon_actual = 0;

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int alto, ancho;
    dim_salon(alto, ancho);

    WINDOW* juego_win = newwin(alto, ancho, (yMax - alto) / 2, (xMax - ancho) / 2);
    keypad(juego_win, TRUE);

    int linkX = 5, linkY = 2;
    char link_char = 'v';

    bool en_partida = true;
    while (en_partida) {
        werase(juego_win);

        dibujar_mapa(juego_win);

        wattron(juego_win, COLOR_PAIR(1));
        mvwaddch(juego_win, linkY, linkX, link_char);
        wattroff(juego_win, COLOR_PAIR(1));

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
            case 'q': case 'Q': en_partida = false;        break;
        }

        if (!en_partida) break;

        char prox = mapa_ptr()[nuevaY][nuevaX];

        if (prox == 'D') {
            // Determinar destino según salón actual y puerta tocada
            bool ok = false;
            int ns = salon_actual, sx = linkX, sy = linkY;

            switch (salon_actual) {
                case 0: // Principal
                    if      (nuevaX == 0)  { ns = 1; sx = 31; sy =  7; ok = true; } // puerta izq -> salon izquierda
                    else if (nuevaX == 47) { ns = 2; sx =  1; sy =  7; ok = true; } // puerta der -> salon derecha
                    else if (nuevaY == 18) { ns = 3; sx = 16; sy =  1; ok = true; } // puerta abajo -> salon abajo
                    break;
                case 1: // Salon izquierda -> volver al principal por puerta izq
                    if (nuevaX == 32)      { ns = 0; sx =  1; sy =  9; ok = true; }
                    break;
                case 2: // Salon derecha -> volver al principal por puerta der
                    if (nuevaX == 0)       { ns = 0; sx = 46; sy =  9; ok = true; }
                    break;
                case 3: // Salon abajo -> volver al principal por puerta abajo
                    if (nuevaY == 0)       { ns = 0; sx = 24; sy = 17; ok = true; }
                    break;
            }

            if (ok) {
                salon_actual = ns;
                linkX = sx;
                linkY = sy;
                dim_salon(alto, ancho);
                delwin(juego_win);
                clear(); refresh();
                juego_win = newwin(alto, ancho,
                                   (yMax - alto) / 2, (xMax - ancho) / 2);
                keypad(juego_win, TRUE);
            }
        } else if (prox != '#' && prox != 'E' && prox != 'X') {
            linkX = nuevaX;
            linkY = nuevaY;
        }
    }

    werase(juego_win);
    wrefresh(juego_win);
    delwin(juego_win);

    clear();
    refresh();
}