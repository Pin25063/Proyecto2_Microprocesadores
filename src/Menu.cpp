#include "Menu.hpp"
#include "Juego.hpp"
#include <ncurses.h>
#include <pthread.h> 
#include <vector>
#include <string>
#include <fstream>

void mostrar_diseno(WINDOW *ventana) {
    int xMax, yMax;
    getmaxyx(ventana, yMax, xMax);

    wattron(ventana, COLOR_PAIR(3));
    mvwprintw(ventana, yMax - 6, 10, "   / \\");
    mvwprintw(ventana, yMax - 5, 10, "  /__ \\\\");
    mvwprintw(ventana, yMax - 4, 10, " /\\\\  /\\\\");
    mvwprintw(ventana, yMax - 3, 10, "/__\\\\/__\\\\");
    mvwprintw(ventana, yMax - 6, xMax - 20, "   / \\");
    mvwprintw(ventana, yMax - 5, xMax - 20, "  /__ \\\\");
    mvwprintw(ventana, yMax - 4, xMax - 20, " /\\\\  /\\\\");
    mvwprintw(ventana, yMax - 3, xMax - 20, "/__\\\\/__\\\\");
    wattroff(ventana, COLOR_PAIR(3));

    wattron(ventana, COLOR_PAIR(2));
    mvwhline(ventana, yMax - 2, 1, *"#", xMax - 2);
    wattroff(ventana, COLOR_PAIR(2));

    wattron(ventana, COLOR_PAIR(1));
    mvwprintw(ventana, yMax - 6, xMax / 2 - 16,  " /\\");
    mvwprintw(ventana, yMax - 5, xMax / 2 - 16, "(..)");
    mvwprintw(ventana, yMax - 4, xMax / 2 - 16,  "/||\\\\");
    mvwprintw(ventana, yMax - 3, xMax / 2 - 16,  " /\\\\");
    wattroff(ventana, COLOR_PAIR(1));
    wattron(ventana, COLOR_PAIR(2));
    mvwprintw(ventana, yMax - 4, xMax / 2 - 9, "o==[]::::::::::::::::>");
    wattroff(ventana, COLOR_PAIR(2));
}

void mostrar_instrucciones() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); 
    WINDOW *inst_win = newwin(yMax - 5, xMax - 12, 2, 5);
    box(inst_win, 0, 0);
    keypad(inst_win, TRUE);

    int yWin, xWin;
    getmaxyx(inst_win, yWin, xWin);

    mvwprintw(inst_win, 1, xWin / 2 - 7, "INSTRUCCIONES");
    mvwhline(inst_win, 2, 1, ACS_HLINE, xWin - 2);

    mvwprintw(inst_win, 4, 3, "OBJETIVO: Explorar el laberinto y llegar a la zona final.");
    mvwprintw(inst_win, 7, 3, "CONTROLES:");
    mvwprintw(inst_win, 8, 5, "W, A, S, D  - Mover a Link");
    mvwprintw(inst_win, 9, 5, "J           - Atacar con Espada");
    mvwprintw(inst_win, 10, 5, "K           - Usar objeto secundario");
    mvwprintw(inst_win, 12, 5, "ESC o Q     - Salir de la partida");

    mvwprintw(inst_win, yWin - 8, xWin / 2 - 22, "Presiona cualquier tecla para volver al menu");

    mostrar_diseno(inst_win);

    wgetch(inst_win); 
    werase(inst_win);
    wrefresh(inst_win);
    delwin(inst_win);

    clear();
    refresh();
}

void mostrar_puntajes_destacados() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *score_win = newwin(yMax - 5, xMax - 12, 2, 5);
    box(score_win, 0, 0);
    keypad(score_win, TRUE);

    int xWin, yWin;
    getmaxyx(score_win, yWin, xWin);

    mvwprintw(score_win, 1, xWin / 2 - 8, "PUNTAJES DESTACADOS");
    mvwhline(score_win, 2, 1, ACS_HLINE, xWin - 2);

    mvwprintw(score_win, 4, xWin / 2 - 11, "JUGADOR         PUNTAJE");
    mvwhline(score_win, 5, xWin / 2 - 11, '-', 23);

    // Datos simulados agregados a la tabla
    mvwprintw(score_win, 6, xWin / 2 - 15, "1. Jugador 1         9500 pts");
    mvwprintw(score_win, 7, xWin / 2 - 15, "2. Jugador 2         8200 pts");
    mvwprintw(score_win, 8, xWin / 2 - 15, "3. Jugador 3         7100 pts");
    mvwprintw(score_win, 9, xWin / 2 - 15, "4. Jugador 4         5000 pts");
    mvwprintw(score_win, 10, xWin / 2 - 15, "5. Jugador 5         4300 pts");

    mvwprintw(score_win, yWin - 8, xWin / 2 - 22, "Presiona cualquier tecla para volver al menu");

    mostrar_diseno(score_win);

    wgetch(score_win); 
    werase(score_win);
    wrefresh(score_win);
    delwin(score_win);

    clear();
    refresh();
}

void iniciar_juego() {
    clear();
    //attron(COLOR_PAIR(1));
    ejecutar_partida();
    //attroff(COLOR_PAIR(1));
    refresh();
}

void menu_principal() {

    const int TOTAL_OPCIONES = 4;

    std::string opciones[TOTAL_OPCIONES] = {
        "Iniciar Partida",
        "Instrucciones",
        "Puntajes Destacados",
        "Salir"
    };

    int seleccion = 0;
    int tecla;
    bool ejecutando = true;

    while (ejecutando) {
        clear();
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        WINDOW *menu_win = newwin(yMax - 5, xMax - 12, 2, 5);
        box(menu_win, 0, 0);
        keypad(menu_win, TRUE);

        int yWin, xWin;
        getmaxyx(menu_win, yWin, xWin);

        wattron(menu_win, COLOR_PAIR(1));
        mvwprintw(menu_win, 2, xWin / 2 - 6, "THE LEGEND OF");
        mvwprintw(menu_win, 3, xWin / 2 - 22, "  _____  ______   _      _____          ");
        mvwprintw(menu_win, 4, xWin / 2 - 22, " |__  / |  ____| | |     |  __ \\     /\\    ");
        mvwprintw(menu_win, 5, xWin / 2 - 22, "   / /  | |__    | |     | |  | |   /  \\   ");
        mvwprintw(menu_win, 6, xWin / 2 - 22, "  / /   |  __|   | |     | |  | |  / /\\ \\  ");
        mvwprintw(menu_win, 7, xWin / 2 - 22, " / /_   | |____  | |____ | |__| / / ____ \\ ");
        mvwprintw(menu_win, 8, xWin / 2 - 22, "/____|  |______| |______||_____/ /_/    \\_\\");
        wattroff(menu_win, COLOR_PAIR(1));
        wattron(menu_win, COLOR_PAIR(4));
        mvwprintw(menu_win, yWin - 9, xWin / 2 - 10, "Usa W/S para navegar");
        wattroff(menu_win, COLOR_PAIR(4));


        for (int i = 0; i < TOTAL_OPCIONES; i++) {
            if (i == seleccion) {
                wattron(menu_win, COLOR_PAIR(2));
                mvwprintw(menu_win, 14 + i * 2, xWin / 2 - 10, "> %s", opciones[i].c_str());
                wattroff(menu_win, COLOR_PAIR(2));

            } else {
                mvwprintw(menu_win, 14 + i * 2, xWin / 2 - 8, "%s", opciones[i].c_str());
            }
        }

        mostrar_diseno(menu_win);

        wrefresh(menu_win);
        tecla = wgetch(menu_win);

        switch (tecla) {
            case 'w':
            case 'W':
                seleccion--; // se mueve hacia arriba
                if (seleccion < 0)
                    seleccion = TOTAL_OPCIONES - 1;
                break;

            case 's':
            case 'S':
                seleccion++; // se mueve hacia abajo
                if (seleccion >= TOTAL_OPCIONES)
                    seleccion = 0;
                break;

            case 10: // enter en ASCII
                switch (seleccion) {
                    case 0:
                        iniciar_juego();
                        break;

                    case 1:
                        mostrar_instrucciones();
                        break;

                    case 2:
                        mostrar_puntajes_destacados();
                        break;

                    case 3:
                        ejecutando = false;
                }

                break;
        }
        
        werase(menu_win);
        wrefresh(menu_win);
        delwin(menu_win);

        clear();
        refresh();

    }
}