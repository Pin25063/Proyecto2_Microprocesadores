/*
Implementación del comportamiento de los enemigos
Cada enemigo es ejecutado mediante un hilo independiente que
controla su movimiento, colisiones con el jugador y generación
de proyectiles
*/

#include "Enemigos.hpp"

extern int salon_actual;
extern int linkX;
extern int linkY;
extern bool link_recibe_dano;
extern DatosProyectil proyectiles_enemigos[];
extern int vida_link;
extern bool invulnerable;
extern int frames_invulnerable;

/*
Hilo encargado de controlar todo el comportamiento del enemigo
- Controla el movimiento aleatorio dentro de la habitación
- Detecta colisiones con Link
- Hace daño a Link
- Genera los proyectiles para los enemigos "E"
*/
void* mover_enemigo(void* arg) {
    DatosEnemigo* enemigo = (DatosEnemigo*)arg;

    while (enemigo -> vivo) {
        pthread_mutex_lock(&mutex_salon);
        bool mismo_salon = salon_actual == enemigo->salon_pertenece; // valida si el enemigo está en la habitación actual
        pthread_mutex_unlock(&mutex_salon);

        if (mismo_salon) {
            // calcula la siguiente dirección según la posición actual
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

            if (nuevaX < 0 || nuevaX >= ancho || nuevaY < 0 || nuevaY >= alto) { // evita que salga de los límites del mapa
                enemigo->direccion = rand() % 4;
                usleep(VELOCIDAD_ENEMIGOS);
                continue;
            }

            char sig_posicion = mapa_ptr()[nuevaY][nuevaX];

            // detecta colisiones con Link
            pthread_mutex_lock(&mutex_jugador);
            if (nuevaX == linkX && nuevaY == linkY) {
                if (!invulnerable) { // si Link no es invulnerable, le hace daño
                    vida_link--;
                    invulnerable = true;
                    frames_invulnerable = 0;
                }

                link_recibe_dano = true;
                // retrocede tras impactar a link
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

            // actualiza la posición si se puede pasar a la siguiente casilla
            if (sig_posicion != '#' && sig_posicion != '|' && sig_posicion != '/') {
                pthread_mutex_lock(&mutex_enemigos);
                enemigo->x = nuevaX;
                enemigo->y = nuevaY;
                pthread_mutex_unlock(&mutex_enemigos);
            } else {
                enemigo->direccion = rand() % 4;
            }

            if (enemigo->simbolo == 'E' && (rand() % 100 < 5)) { // los enemigos tipo "E" pueden lanzar proyectiles aleatoriamente
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