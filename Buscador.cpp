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
#include "Usuario.hpp"

#define NUM_MAX_PALABRAS_GRATUITO 10

const int NUM_HILOS = 50; // Número de hilos a crear
std::vector<std::string> categorias = {"Gratuito","Premium limite saldo","Premium ilimitado"};
std::condition_variable cvSaldo;
std::mutex mtxSaldo;
Diccionario d;
std::queue<PeticionBusqueda*> colaPremium;
std::queue<PeticionBusqueda*> colaGratis;
std::mutex mtxCola;
std::condition_variable cvCola;

void crearDiccionario(){
    d.palabras = {"amor","castillo","sirena","trabajo","casa","gente","corazón","mundo","vida","bien"};
}

void crearUsuarios(){
    std::vector<std::thread> hilos;

    for(int i=0; i<NUM_HILOS; i++){
        int valor = rand() % 3;
        std::string palabra = d.palabras[rand() % d.palabras.size()];
        
        switch(valor){
            case 0: {
                //gratuito
                Usuario u(i, categorias[valor], palabra);
                std::cout<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            }
            case 1:{
                //premium limite saldo
                int saldo = 1 + rand() % 50;
                Usuario u(i, categorias[valor], palabra, saldo);
                std::cout<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<" y saldo "<<saldo<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            } 
            case 2: {//premium ilimitado
                Usuario u(i, categorias[valor], palabra);
                std::cout<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula el tiempo de creación del hilo

    }
    for(auto &h : hilos){
    h.join();
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

    solicitarRecarga(u.getId(), saldo);
    
}

void atenderPeticiones(){
    while(true){
        PeticionBusqueda* p;

        //Espero a que haya peticiones
        {
            std::unique_lock<std::mutex> lock(mtxCola);
            cvCola.wait(lock, []{ return !colaGratis.empty(); });

            if(!colaPremium.empty()) {
                p = colaPremium.front();
                colaPremium.pop();
            } else {
                p = colaGratis.front();
                colaGratis.pop();
            }
        }

        std::string categoria = p->usuario.getCategoria();

        if(categoria == "Gratuito"){
            int restantes = p->usuario.getBusquedasRestantes();

            if(restantes <= 0) {
                std::cout << "Cliente " << p->usuario.getId() << " sin búsquedas disponibles\n";
            } else {
                p->usuario.setBusquedasRestantes(restantes -1);
            }
            std::cout << "Cliente " << p->usuario.getId() << " (Gratis) realizando búsqueda\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else if(categoria == "Premium limite saldo"){
            int saldo = p->usuario.getSaldo();

            std::cout << "Cliente " << p->usuario.getId() << " (Premium saldo: " << saldo << ")\n";

            if(saldo <= 0){
                std::cout << "Cliente " << p->usuario.getId() << " sin saldo, recargando...\n";

                solicitarRecarga(p->usuario.getId(), saldo);
                p->usuario.setSaldo(saldo);
            }

            saldo--;
            p->usuario.setSaldo(saldo);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else{ // Premium ilimitado
            std::cout << "Cliente " << p->usuario.getId() << " (Premium ilimitado)\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        {
            std::lock_guard<std::mutex> lock(p->mtx);
            p->completada = true;
        }

        //Notifico al cliente
        p->cv.notify_one();
    }
}

int main() {
    srand(time(NULL));
    crearDiccionario();

    std::thread hiloPago(sistema);
    hiloPago.detach();

    const int NUM_SERVIDORES = 4;
    std::vector<std::thread> servidores;

    for(int i = 0; i < NUM_SERVIDORES; i++){
        servidores.emplace_back(atenderPeticiones);
    }

    for(auto &s : servidores){
        s.detach();
    }

    crearUsuarios();

    return 0;
}