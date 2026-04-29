#include "BusquedaArchivos.hpp"
#include <fstream>
#include <iostream>

int buscarEnArchivo(const std::string& nombreArchivo, const std::string& palabra) {
    std::ifstream file(nombreArchivo);

    if (!file.is_open()) {
        std::cerr << "Error abriendo archivo: " << nombreArchivo << std::endl;
        return 0;
    }

    int contador = 0;
    std::string palabraLeida;

    while (file >> palabraLeida) {
        if (palabraLeida == palabra) {
            contador++;
        }
    }

    file.close();
    return contador;
}