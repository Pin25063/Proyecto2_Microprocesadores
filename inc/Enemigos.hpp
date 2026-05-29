#ifndef ENEMIGOS_HPP
#define ENEMIGOS_HPP

#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

#include "Juego.hpp"
#include "Mapa.hpp"

void* mover_enemigo(void* arg);

#endif