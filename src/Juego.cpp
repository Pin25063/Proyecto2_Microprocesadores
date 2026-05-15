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

void ejecutar_partida() {
    clear();
    mvprintw(0, 0, "presiones cualquier tecla para salir");
    refresh();
    getch();
}