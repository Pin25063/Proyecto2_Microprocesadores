#include "Proyectiles.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;

void* mover_proyectil(void* arg) {
    DatosProyectil* proyectil = (DatosProyectil*) arg;

    while (proyectil -> activo) {
        if (salon_actual == proyectil -> salon_pertenece) {
            int nuevaX = proyectil->x;
            int nuevaY = proyectil->y;

            if (proyectil ->orientacion == '^') {
                nuevaY--;
            } else if (proyectil -> orientacion == 'v') {
                nuevaY++;
            } else if (proyectil -> orientacion == '<') {
                nuevaX--;
            } else if (proyectil -> orientacion == '>') {
                nuevaX++;
            }

            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            if (nuevaX == linkX && nuevaY == linkY) {
                link_recibe_dano = true;
                proyectil ->activo = false;
            } else if (sig_posicion == '#' || sig_posicion == '|') {
                proyectil -> activo = false;
            } else {
                proyectil -> x = nuevaX;
                proyectil ->y = nuevaY;
            }
        }
        usleep(100000);
    }
    return nullptr;
}