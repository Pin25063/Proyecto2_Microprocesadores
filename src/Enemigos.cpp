#include "Enemigos.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern DatosProyectil proyectiles_enemigos[];
extern int vida_link;
extern bool invulnerable;
extern int frames_invulnerable;

void* mover_enemigo(void* arg) {
    DatosEnemigo* enemigo = (DatosEnemigo*)arg;

    while (enemigo -> vivo) {
        pthread_mutex_lock(&mutex_salon);
        bool mismo_salon = salon_actual == enemigo->salon_pertenece;
        pthread_mutex_unlock(&mutex_salon);

        if (mismo_salon) {
            int direccion = enemigo->direccion;
            int nuevaX = enemigo-> x;
            int nuevaY = enemigo-> y;

            switch (direccion) {
                case 0: nuevaY--; break;
                case 1: nuevaY++; break;
                case 2: nuevaX--; break;
                case 3: nuevaX++; break;
            }

            int alto, ancho;
            dim_salon(alto, ancho);

            if (nuevaX < 0 || nuevaX >= ancho || nuevaY < 0 || nuevaY >= alto) {
                enemigo->direccion = rand() % 4;
                usleep(VELOCIDAD_ENEMIGOS);
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
                if (direccion == 0) {
                    enemigo->y++;
                } else if (direccion == 1) {
                    enemigo->y--;
                } else if (direccion == 2) {
                    enemigo->x++;
                } else if (direccion == 3) {
                    enemigo->x--;
                }
            }
            pthread_mutex_unlock(&mutex_jugador);

            if (sig_posicion != '#' && sig_posicion != '|' && sig_posicion != '/') {
                pthread_mutex_lock(&mutex_enemigos);
                enemigo->x = nuevaX;
                enemigo->y = nuevaY;
                pthread_mutex_unlock(&mutex_enemigos);
            } else {
                enemigo->direccion = rand() % 4;
            }

            if (enemigo->simbolo == 'E' && (rand() % 100 < 5)) {
                pthread_mutex_lock(&mutex_proyectiles);
                for (int i = 0; i < MAX_PROYECTILES; i++) {
                    if (!proyectiles_enemigos[i].activo) {
                        proyectiles_enemigos[i].x = enemigo->x;
                        proyectiles_enemigos[i].y = enemigo->y;
                        proyectiles_enemigos[i].salon_pertenece = enemigo->salon_pertenece;
                        proyectiles_enemigos[i].es_enemigo = true;
                        
                        if (direccion == 0) proyectiles_enemigos[i].orientacion = '^';
                        else if (direccion == 1) proyectiles_enemigos[i].orientacion = 'v';
                        else if (direccion == 2) proyectiles_enemigos[i].orientacion = '<';
                        else if (direccion == 3) proyectiles_enemigos[i].orientacion = '>';
                        
                        proyectiles_enemigos[i].activo = true;
                        
                        pthread_t hilo_flecha_enemiga;
                        pthread_create(&hilo_flecha_enemiga, NULL, mover_proyectil, (void*)&proyectiles_enemigos[i]);
                        pthread_detach(hilo_flecha_enemiga);
                        break;
                    }
                }
                pthread_mutex_unlock(&mutex_proyectiles);
            }
        }
        usleep(VELOCIDAD_ENEMIGOS);
    }
    return nullptr;
}