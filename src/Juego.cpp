#include "Juego.hpp"
#include <string>

const int MAPA_ALTO = 15;
const int MAP_ANCHO = 40;

std::string mapa[MAPA_ALTO] = {
        "########################################",
        "#                                      #",
        "#  ######   ########   ######   #####  #",
        "#  #    #   #   +  #   #    #   # K #  #",
        "#  #    #   #      #   #    #   #   #  #",
        "#  #    #####      #####    #####   #  #",
        "#  #                                D  #", 
        "#  ########                  ########  #",
        "#  #                                #  #",
        "#  #    #####      #####    #####   #  #",
        "#  #    #   #      #   #    #   #   #  #",
        "#  #    #   #   /  #   #    #   #   #  #",
        "#  ######   ########   ######   #####  #",
        "#                                      #",
        "########################################"
};

void dibujar_mapa(WINDOW* win) {
    for (int y = 0; y < MAPA_ALTO; y++) {
        for (int x = 0; x < MAP_ANCHO; x++) {
            char caracter = mapa[y][x];

            if (caracter == '#') {
                wattron(win, COLOR_PAIR(2));
                mvwaddch(win, y, x, caracter);
                wattroff(win, COLOR_PAIR(2));
            } else if (caracter == '+') {
                wattron(win, COLOR_PAIR(1));
                mvwaddch(win, y, x, caracter);
                wattroff(win, COLOR_PAIR(1));
            } else if (caracter == 'K' || caracter == '/') {
                wattron(win, COLOR_PAIR(2));
                mvwaddch(win, y, x, caracter);
                wattroff(win, COLOR_PAIR(2));
            } else {
                mvwaddch(win, y, x, caracter);
            }
        }
    }
}

void ejecutar_partida() {
    clear();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int inicioY = (yMax - MAPA_ALTO) / 2;
    int inicioX = (xMax - MAP_ANCHO) / 2;

    WINDOW* juego_win = newwin(MAPA_ALTO, MAP_ANCHO, inicioY, inicioX);
    keypad(juego_win, TRUE);

    bool en_partida = true;
    while (en_partida) {
        werase(juego_win);

        dibujar_mapa(juego_win);

        mvprintw(yMax -2, (xMax - 40) / 2, "presione cualquier tecla para regresar");
        refresh();
        wrefresh(juego_win);
        

        int tecla = wgetch(juego_win);
        en_partida = false;
    }

    werase(juego_win);
    wrefresh(juego_win);
    delwin(juego_win);

    clear();
    refresh();
}