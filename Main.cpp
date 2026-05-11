#include <ncurses.h>
#include <pthread.h> 

void mostrar_instrucciones() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); //Funciona para obtener la dimension de la terminal

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

    mvwprintw(inst_win, yMax - 7, (xMax - 10) / 2 - 12, "Presiona cualquier tecla para volver");

    wrefresh(inst_win);
    wgetch(inst_win); 
    delwin(inst_win);
}

int main() {
    // Inicializar ncurses 
    initscr();
    cbreak();             // Desactiva el buffering de línea
    noecho();             // No muestra las teclas que se precionen
    curs_set(0);          // Esconde el cursor

    mostrar_instrucciones();

    // Finalizar ncurses
    endwin();
    return 0;
}