#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <cstdlib>
#include "Usuario.hpp"
#include "SistemaBusqueda.hpp"

const int NUM_HILOS = 5; // Número de hilos a crear
std::vector<std::string> categorias = {"Gratuito","Premium limite saldo","Premium ilimitado"};

void Usuario::crearHilos(){
    for(int i=0; i<NUM_HILOS; i++){
        int valor = rand() % 3;
        std::string categoria = categorias[valor];
        std::thread hiloUsuario(i, categoria);
        std::cout<<"Hilo creado para el usuario "<<i<<" con categoria "<<categoria<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula el tiempo de creación del hilo

    }
    
}