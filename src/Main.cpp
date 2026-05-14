#include <ncurses.h>
#include "Menu.hpp"

int main() {
 // Inicializar ncurses 
    initscr();
    cbreak();             // Desactiva el buffering de línea
    noecho();             // No muestra las teclas que se precionen
    curs_set(0);          // Esconde el cursor
    start_color();
    use_default_colors();

    // Coloresinit_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_CYAN, -1);
    init_pair(4, COLOR_BLUE, -1);

    menu_principal();

    // Finalizar ncurses
    endwin();
    return 0;
}