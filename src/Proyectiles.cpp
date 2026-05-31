/*
Implementación del comportamiento de los proyectiles
del jugador y de los enemigos

Cada proyectil es gestionado mediante un hilo independiente
que controla su desplazamiento, detección de colisiones
y finalización
*/

#include "Proyectiles.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern int vida_link;
extern bool invulnerable;
extern int frames_invulnerable;
extern DatosEnemigo enemigos[];
extern int puntaje;

/*
Hilo encargado de controlar el movimiento de un proyectil
- Controla el desplazamiento según orientación
- detecta impactos con enemigos y jugador
- Colisiones con paredes y límites del mapa
*/
void* mover_proyectil(void* arg) {
    DatosProyectil* proyectil = (DatosProyectil*) arg;

    while (proyectil -> activo) {
        pthread_mutex_lock(&mutex_salon);
        bool mismo_salon = salon_actual == proyectil->salon_pertenece; // Verifica que el proyectil se encuentre en el salón activo
        pthread_mutex_unlock(&mutex_salon);

        if (mismo_salon) {
            int nuevaX = proyectil->x;
            int nuevaY = proyectil->y; // Calcula la siguiente posición del proyectil

            if (!proyectil->es_enemigo) {
                // Si el proyectil pertenece al jugador, verifica colisiones contra enemigos
                pthread_mutex_lock(&mutex_enemigos);
                bool impacto = false;

                for (int i = 0; i < NUM_ENEMIGOS; i++) {
                    if (!enemigos[i].vivo)
                        continue;

                    if (enemigos[i].salon_pertenece != proyectil->salon_pertenece)
                        continue;

                    if (enemigos[i].x == nuevaX && enemigos[i].y == nuevaY) {
                        enemigos[i].vida--;

                        if (enemigos[i].vida > 0) {
                            puntaje += 5;
                        } else {
                            enemigos[i].vivo = false;
                            puntaje += 10;
                        }
                        proyectil->activo = false;
                        impacto = true;
                        break;
                    }
                }
                pthread_mutex_unlock(&mutex_enemigos);

                if (impacto)
                    continue;
            }

            if (proyectil ->orientacion == '^') { // actualiza la posición según la dirección de disparo
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

            if (nuevaX < 0 || nuevaX >= ancho || nuevaY < 0 || nuevaY >= alto) { //Verifica límites del mapa
                proyectil->activo = false;
                continue;
            }
            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            pthread_mutex_lock(&mutex_jugador);
            if (proyectil->es_enemigo && nuevaX == linkX && nuevaY == linkY) { // Detecta impactos de proyectiles enemigos contra el jugador
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
                proyectil ->y = nuevaY; // Si no existe colisión, actualiza la posición del proyectil
            }
            pthread_mutex_unlock(&mutex_jugador);
        }
        usleep(100000);
    }
    return nullptr;
}