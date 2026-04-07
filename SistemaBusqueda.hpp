#ifndef SISTEMABUSQUEDA_HPP
#define SISTEMABUSQUEDA_HPP

#include <iostream>
#include <string>

struct Usuario{
    int idUsuario;
    std::string categoria; //gratuita / saldo limitado / saldo ilimitado
    std::string palabraBuscada;

};

struct Diccionario{
    std::vector<std::string> palabras;
};



#endif