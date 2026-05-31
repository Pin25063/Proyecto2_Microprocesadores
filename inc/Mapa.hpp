#ifndef MAPA_HPP
#define MAPA_HPP

#include <string>

// Dimensiones
extern const int ALTO0, ANCHO0;
extern const int ALTO1, ANCHO1;
extern const int ALTO2, ANCHO2;
extern const int ALTO3, ANCHO3;
extern const int ALTO4, ANCHO4;

// Variables globales
extern int salon_actual;

// Funciones
const std::string* mapa_ptr();
void dim_salon(int& alto, int& ancho);

#endif