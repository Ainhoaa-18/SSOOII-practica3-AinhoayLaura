#ifndef SISTEMABUSQUEDA_HPP
#define SISTEMABUSQUEDA_HPP

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Usuario.hpp"


struct PeticionBusqueda {
    Usuario usuario;
    bool completada = false;

    std::mutex mtx;
    std::condition_variable cv;

    PeticionBusqueda(Usuario u) : usuario(u) {}
};

extern std::queue<PeticionBusqueda*> colaPremium;
extern std::queue<PeticionBusqueda*> colaGratis;
extern std::mutex mtxCola;
extern std::condition_variable cvCola;

struct Diccionario{
    std::vector<std::string> palabras;
};




#endif