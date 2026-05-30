#include "Proyectiles.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern int vida_link;
extern bool invulnerable;
extern int frames_invulnerable;

void* mover_proyectil(void* arg) {
    DatosProyectil* proyectil = (DatosProyectil*) arg;

    while (proyectil -> activo) {
        pthread_mutex_lock(&mutex_salon);
        bool mismo_salon = salon_actual == proyectil->salon_pertenece;
        pthread_mutex_unlock(&mutex_salon);

        if (mismo_salon) {
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

            int alto, ancho;
            dim_salon(alto, ancho);

            if (nuevaX < 0 || nuevaX >= ancho || nuevaY < 0 || nuevaY >= alto) {
                proyectil->activo = false;
                continue;
            }
            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            pthread_mutex_lock(&mutex_jugador);
            if (nuevaX == linkX && nuevaY == linkY) {
                if (!invulnerable) {
                    vida_link--;
                    invulnerable = true;
                    frames_invulnerable = 0;
                }
                link_recibe_dano = true;
                proyectil->activo = false;
            } else if (sig_posicion == '#' || sig_posicion == '|') {
                proyectil -> activo = false;
            } else {
                proyectil -> x = nuevaX;
                proyectil ->y = nuevaY;
            }
            pthread_mutex_unlock(&mutex_jugador);
        }
        usleep(100000);
    }
    return nullptr;
}