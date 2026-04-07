#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <condition_variable>
#include "SistemaBusqueda.hpp"
#include "SistemaPago.hpp"

#define NUM_MAX_PALABRAS_GRATUITO 10

const int NUM_HILOS = 50; // Número de hilos a crear
std::vector<std::string> categorias = {"Gratuito","Premium limite saldo","Premium ilimitado"};
std::condition_variable cvSaldo;
std::mutex mtxSaldo;
Diccionario d;

void crearDiccionario(){
    Diccionario d;
    d.palabras = {"amor","castillo","sirena","trabajo","casa","gente","corazón","mundo","vida","bien"};
}

void crearUsuarios(){
    std::vector<std::thread> hilos;

    for(int i=0; i<NUM_HILOS; i++){
        Usuario u;
        u.idUsuario = i;
        int valor = rand() % 3;
        u.categoria = categorias[valor];
        u.palabraBuscada = d.palabras[rand() % d.palabras.size()];
        switch(valor){
            case 0: //gratuito
                std::cout<<"Usuario "<<u.idUsuario<<" creado con categoria "<<u.categoria<<std::endl;
                hilos.push_back(std::thread(u.idUsuario, u.categoria, u.palabraBuscada));
                break;
            case 1: //premium limite saldo
                int saldo = 1+rand() % 50;
                std::cout<<"Usuario "<<u.idUsuario<<" creado con categoria "<<u.categoria<<" y saldo "<<saldo<<std::endl;
                hilos.push_back(std::thread(u.idUsuario, u.categoria, u.palabraBuscada, saldo));
                break;
            case 2: //premium ilimitado
                std::cout<<"Usuario "<<u.idUsuario<<" creado con categoria "<<u.categoria<<std::endl;
                hilos.push_back(std::thread(u.idUsuario, u.categoria, u.palabraBuscada));
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula el tiempo de creación del hilo

    }
    
}


void comprobarCategoria(std::string categoria){
    if(categoria == "Gratuito"){
        
    }else if(categoria == "Premium limite saldo"){
        
    }else if(categoria == "Premium ilimitado"){
        
    }
}

void realizarBusquedaGratis(){
    
}

void realizarBusquedaPremiumLimiteSaldo(){
    
}

void comprobarSaldo(Usuario u, int saldo){
    std::unique_lock<std::mutex> lock(mtxSaldo);
    cvSaldo.wait(lock, [&]{return saldo<=0;});

    solicitarRecarga(u.idUsuario, saldo);
    

}


int main() {
    srand(time(NULL));
    crearUsuarios();
    return 0;
}