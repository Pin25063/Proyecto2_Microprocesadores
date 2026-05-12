#include <ncurses.h>
#include <pthread.h> 
#include <vector>
#include <string>
#include <fstream>

void mostrar_instrucciones() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); 

    WINDOW *inst_win = newwin(yMax - 4, xMax - 10, 2, 5);
    box(inst_win, 0, 0);
    keypad(inst_win, TRUE);

    mvwprintw(inst_win, 1, (xMax - 10) / 2 - 6, "INSTRUCCIONES");
    mvwhline(inst_win, 2, 1, ACS_HLINE, xMax - 12);

    mvwprintw(inst_win, 4, 3, "OBJETIVO: Explorar el laberinto y llegar a la zona final.");
    mvwprintw(inst_win, 7, 3, "CONTROLES:");
    mvwprintw(inst_win, 8, 5, "W, A, S, D  - Mover a Link");
    mvwprintw(inst_win, 9, 5, "J           - Atacar con Espada");
    mvwprintw(inst_win, 10, 5, "K           - Usar objeto secundario");
    mvwprintw(inst_win, 12, 5, "ESC o Q     - Salir de la partida");

    mvwprintw(inst_win, yMax - 7, (xMax - 10) / 2 - 18, "Presiona cualquier tecla para volver al menu");

    wrefresh(inst_win);
    wgetch(inst_win); 
    delwin(inst_win);
}

void mostrar_puntajes_destacados() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *score_win = newwin(yMax - 4, xMax - 10, 2, 5);
    box(score_win, 0, 0);
    keypad(score_win, TRUE);

    mvwprintw(score_win, 1, (xMax - 10) / 2 - 9, "PUNTAJES DESTACADOS");
    mvwhline(score_win, 2, 1, ACS_HLINE, xMax - 12);

    mvwprintw(score_win, 4, (xMax - 10) / 2 - 12, "JUGADOR         PUNTAJE");
    mvwhline(score_win, 5, (xMax - 10) / 2 - 12, '-', 23);

    // Datos simulados agregados a la tabla
    mvwprintw(score_win, 6, (xMax - 10) / 2 - 12, "1. Jugador 1         9500 pts");
    mvwprintw(score_win, 7, (xMax - 10) / 2 - 12, "2. Jugador 2         8200 pts");
    mvwprintw(score_win, 8, (xMax - 10) / 2 - 12, "3. Jugador 3         7100 pts");
    mvwprintw(score_win, 9, (xMax - 10) / 2 - 12, "4. Jugador 4         5000 pts");
    mvwprintw(score_win, 10, (xMax - 10) / 2 - 12, "5. Jugador 5         4300 pts");

    mvwprintw(score_win, yMax - 7, (xMax - 10) / 2 - 18, "Presiona cualquier tecla para volver al menu");

    wrefresh(score_win);
    wgetch(score_win); 
    delwin(score_win);
}

void iniciar_juego() {
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(10, 10, "INICIANDO JUEGO..."); //placeholder temporal
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
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
        WINDOW *menu_win = newwin(18, 50, 4, 10);
        box(menu_win, 0, 0);

        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        wattron(menu_win, COLOR_PAIR(1));
        mvwprintw(menu_win, 2, 14, "THE LEGEND OF ZELDA");
        wattroff(menu_win, COLOR_PAIR(1));
        mvwprintw(menu_win, 4, 13, "Usa W/S para navegar");

        for (int i = 0; i < TOTAL_OPCIONES; i++) {
            if (i == seleccion) {
                wattron(menu_win, COLOR_PAIR(2));
                mvwprintw(menu_win, 7 + i * 2, 14, "> %s", opciones[i].c_str());
                wattroff(menu_win, COLOR_PAIR(2));

            } else {
                mvwprintw(menu_win, 7 + i * 2, 16, "%s", opciones[i].c_str());
            }
        }

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
        delwin(menu_win);

    }
}

int main() {
 // Inicializar ncurses 
    initscr();
    cbreak();             // Desactiva el buffering de línea
    noecho();             // No muestra las teclas que se precionen
    curs_set(0);          // Esconde el cursor
    start_color();
    use_default_colors();

    // Colores
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_CYAN, -1);

    menu_principal();

    // Finalizar ncurses
    endwin();
    return 0;
}