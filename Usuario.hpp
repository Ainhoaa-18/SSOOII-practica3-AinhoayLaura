#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>

class Usuario{
    private:
    int id;
    std::string categoria;

    public:
        Usuario(int id, std::string categoria); 
        static void crearHilos();
        static void realizarPeticionBusqueda(int id);
        static void mostrarResultadosBusqueda(int id);
    
};

#endif