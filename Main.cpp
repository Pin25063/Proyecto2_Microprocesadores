#include <ncurses.h>
#include <pthread.h> 

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
    box(score_win, 0, 0); // Se agrega el borde
    keypad(score_win, TRUE);

    // Título y encabezados
    mvwprintw(score_win, 1, (xMax - 10) / 2 - 9, "PUNTAJES DESTACADOS");
    mvwhline(score_win, 2, 1, ACS_HLINE, xMax - 12);

    mvwprintw(score_win, 4, (xMax - 10) / 2 - 12, "JUGADOR         PUNTAJE");
    mvwhline(score_win, 5, (xMax - 10) / 2 - 12, '-', 23);

    mvwprintw(score_win, yMax - 7, (xMax - 10) / 2 - 18, "Presiona cualquier tecla para volver al menu");

    wrefresh(score_win);
    wgetch(score_win); 
    delwin(score_win);
}

int main() {
    initscr();
    cbreak();             
    noecho();             
    curs_set(0);          

    int opcion;
    bool salir = false;

    while(!salir) {
        clear();
        mvprintw(2, 5, "MENU PRINCIPAL ZELDA 1");
        mvprintw(4, 5, "1. Ver Instrucciones");
        mvprintw(5, 5, "2. Ver Puntajes Destacados");
        mvprintw(6, 5, "3. Salir");
        mvprintw(8, 5, "Selecciona una opcion (1-3): ");
        refresh();

        opcion = getch();

        switch(opcion) {
            case '1':
                mostrar_instrucciones();
                break;
            case '2':
                mostrar_puntajes_destacados();
                break;
            case '3':
                salir = true;
                break;
        }
    }

    endwin();
    return 0;
}