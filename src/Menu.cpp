#include "Menu.hpp"
#include "Juego.hpp"
#include <ncurses.h>
#include <pthread.h> 
//#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

void mostrar_diseno(WINDOW *ventana) {
    int xMax, yMax;
    getmaxyx(ventana, yMax, xMax);

    wattron(ventana, COLOR_PAIR(3));
    mvwprintw(ventana, yMax - 6, 10, "   /\\");
    mvwprintw(ventana, yMax - 5, 10, "  /__\\");
    mvwprintw(ventana, yMax - 4, 10, " /\\  /\\");
    mvwprintw(ventana, yMax - 3, 10, "/__\\/__\\");
    mvwprintw(ventana, yMax - 6, xMax - 18, "   /\\");
    mvwprintw(ventana, yMax - 5, xMax - 18, "  /__\\");
    mvwprintw(ventana, yMax - 4, xMax - 18, " /\\  /\\");
    mvwprintw(ventana, yMax - 3, xMax - 18, "/__\\/__\\");
    wattroff(ventana, COLOR_PAIR(3));

    wattron(ventana, COLOR_PAIR(2));
    mvwhline(ventana, yMax - 2, 1, *"#", xMax - 2);
    wattroff(ventana, COLOR_PAIR(2));

    wattron(ventana, COLOR_PAIR(1));
    mvwprintw(ventana, yMax - 6, xMax / 2 - 7,  " /\\");
    mvwprintw(ventana, yMax - 5, xMax / 2 - 7, "(..)");
    mvwprintw(ventana, yMax - 4, xMax / 2 - 7,  "/||\\");
    mvwprintw(ventana, yMax - 3, xMax / 2 - 7,  " /\\");
    wattroff(ventana, COLOR_PAIR(1));
    wattron(ventana, COLOR_PAIR(2));
    mvwprintw(ventana, yMax - 4, xMax / 2 - 1, "o==[]::::>");
    wattroff(ventana, COLOR_PAIR(2));
}

void mostrar_game_over(bool victoria, int puntaje) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *fin_win = newwin(10, 40, (yMax - 10) / 2, (xMax - 40) / 2);
    box(fin_win, 0, 0);

    int yWin, xWin;
    getmaxyx(fin_win, yWin, xWin);

    if (victoria) {
        wattron(fin_win, COLOR_PAIR(1)); // Verde
        mvwprintw(fin_win, 3, xWin / 2 - 5, "¡VICTORIA!");
        wattroff(fin_win, COLOR_PAIR(1));
        mvwprintw(fin_win, 5, xWin / 2 - 13, "Lograste escapar del templo");
        mvwprintw(fin_win, 6, xWin / 2 - 8, "Puntaje: %d", puntaje);
    } else {
        wattron(fin_win, COLOR_PAIR(6)); // Rojo (Color 6 que definieron en main)
        mvwprintw(fin_win, 3, xWin / 2 - 5, "GAME OVER");
        wattroff(fin_win, COLOR_PAIR(6));
        mvwprintw(fin_win, 5, xWin / 2 - 12, "Link ha sido derrotado...");
    }

    mvwprintw(fin_win, 8, xWin / 2 - 17, "Presiona R para reiniciar o Q salir");
    
    wrefresh(fin_win);
    

}

void guardar_puntaje(int puntaje_final) {
    std::ofstream archivo("puntajes.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << "Link " << puntaje_final << "\n";
        archivo.close();
    }
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

    mvwprintw(inst_win, 4, 3, "OBJETIVO: Escapar por la salida sur del Salon 3.");

    mvwprintw(inst_win, 6, 3, "CONTROLES:");
    mvwprintw(inst_win, 7, 5, "W, A, S, D  - Mover a Link");
    mvwprintw(inst_win, 8, 5, "J           - Atacar con Espada");
    mvwprintw(inst_win, 9, 5, "K           - Disparar Proyectil");
    mvwprintw(inst_win, 10, 5, "Q           - Salir de la partida");

    mvwprintw(inst_win, 12, 3, "ENEMIGOS:");
    mvwprintw(inst_win, 13, 5, "'E' y 'X' se mueven solos y quitan vida al contacto.");

    mvwprintw(inst_win, 15, 3, "LLAVES Y PUERTAS:");
    mvwprintw(inst_win, 16, 5, "Recoge la llave 'K' de cada salon.");
    mvwprintw(inst_win, 17, 5, "Cada llave abre su puerta ( | o / ) respectiva.");

    mvwprintw(inst_win, 19, 3, "MODO GUIADO:");
    mvwprintw(inst_win, 20, 5, "Al iniciar puedes elegir Modo Guiado (G) para ver a Link moverse automaticamente como demostracion.");

    mvwprintw(inst_win, yWin - 8, xWin / 2 - 22, "Presiona cualquier tecla para volver al menu");

    mostrar_diseno(inst_win);

    wgetch(inst_win); 
    werase(inst_win);
    wrefresh(inst_win);
    delwin(inst_win);

    clear();
    refresh();
}

struct PuntajeRegistro {
    std::string nombre;
    int puntos;
};

bool compararPuntajes(const PuntajeRegistro& a, const PuntajeRegistro& b) {
    return a.puntos > b.puntos;
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

    std::vector<PuntajeRegistro> lista_puntajes;
    std::ifstream archivo("puntajes.txt");
    
    if (archivo.is_open()) {
        std::string nom;
        int pts;
        // Leer el archivo
        while (archivo >> nom >> pts) {
            lista_puntajes.push_back({nom, pts});
        }
        archivo.close();

        // Ordenar de mayor a menor
        std::sort(lista_puntajes.begin(), lista_puntajes.end(), compararPuntajes);

        // Mostrar el Top 5
        int filas_a_mostrar = (lista_puntajes.size() < 5) ? lista_puntajes.size() : 5;
        for (int i = 0; i < filas_a_mostrar; i++) {
            mvwprintw(score_win, 6 + i, xWin / 2 - 15, "%d. %-15s %d pts", i + 1, lista_puntajes[i].nombre.c_str(), lista_puntajes[i].puntos);
        }
    } else {
        mvwprintw(score_win, 7, xWin / 2 - 14, "No hay puntajes registrados.");
    }

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
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW* modo_win = newwin(7, 42, (yMax - 7) / 2, (xMax - 42) / 2);
    box(modo_win, 0, 0);
    keypad(modo_win, TRUE);
    mvwprintw(modo_win, 2, 4, "Modo Manual (M) o Modo Guiado (G)?");
    mvwprintw(modo_win, 4, 14, "Presiona M o G");
    wrefresh(modo_win);
    int resp;
    do { resp = wgetch(modo_win); } while (resp != 'm' && resp != 'M' && resp != 'g' && resp != 'G');
    werase(modo_win);
    wrefresh(modo_win);
    delwin(modo_win);
    clear();
    refresh();
    bool reiniciar;
    do {
        if (resp == 'g' || resp == 'G')
            reiniciar = ejecutar_partida(true);
        else
            reiniciar = ejecutar_partida(false);

    } while (reiniciar);
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
        mvwprintw(menu_win, 2, xWin / 2 - 8, "THE LEGEND OF");
        mvwprintw(menu_win, 3, xWin / 2 - 22, "  _____  ______   _      _____          ");
        mvwprintw(menu_win, 4, xWin / 2 - 22, " |__  / |  ____| | |     |  __ \\     /\\    ");
        mvwprintw(menu_win, 5, xWin / 2 - 22, "   / /  | |__    | |     | |  | |   /  \\   ");
        mvwprintw(menu_win, 6, xWin / 2 - 22, "  / /   |  __|   | |     | |  | |  / /\\ \\  ");
        mvwprintw(menu_win, 7, xWin / 2 - 22, " / /__  | |____  | |____ | |__| / / ____ \\ ");
        mvwprintw(menu_win, 8, xWin / 2 - 22, "/_____| |______| |______||_____/ /_/    \\_\\");
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