#include "Enemigos.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern DatosProyectil proyectiles_enemigos[];

void* mover_enemigo(void* arg) {
    DatosEnemigo* enemigo = (DatosEnemigo*)arg;

    while (enemigo -> vivo) {
        if (salon_actual == enemigo->salon_pertenece) {
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
                continue;
            }

            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            if (nuevaX == linkX && nuevaY == linkY) {
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
            } else if (sig_posicion != '#' && sig_posicion != '|' && sig_posicion != '/') {
                enemigo->x = nuevaX;
                enemigo->y = nuevaY;
            } else {
                enemigo->direccion = rand() % 4;
            }

            if (enemigo->simbolo == 'E' && (rand() % 100 < 5)) {
                for (int i = 0; i < MAX_PROYECTILES; i++) {
                    if (!proyectiles_enemigos[i].activo) {
                        proyectiles_enemigos[i].x = enemigo->x;
                        proyectiles_enemigos[i].y = enemigo->y;
                        proyectiles_enemigos[i].salon_pertenece = enemigo->salon_pertenece;
                        
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
            }
        }
        usleep(VELOCIDAD_ENEMIGOS);
    }
    return nullptr;
}