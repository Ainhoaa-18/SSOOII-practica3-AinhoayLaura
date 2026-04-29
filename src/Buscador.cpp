#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include "SistemaBusqueda.hpp"
#include "SistemaPago.hpp"
#include "Usuario.hpp"
#include "BusquedaArchivos.hpp"

#define NUM_MAX_PALABRAS_GRATUITO 10
std::string verde = "\033[32m";
std::string rojo = "\033[31m";
std::string azul = "\033[34m";
std::string amarillo = "\033[33m";
std::string rosa = "\033[35m";


const int NUM_HILOS = 50; // Número de hilos a crear
std::vector<std::string> categorias = {"Gratuito","Premium limite saldo","Premium ilimitado"};
std::condition_variable cvSaldo;
std::mutex mtxSaldo;
Diccionario d;
std::queue<PeticionBusqueda*> colaPremium;
std::queue<PeticionBusqueda*> colaGratis;
std::mutex mtxCola;
std::condition_variable cvCola;
std::vector<std::string> libros {
    "lib/17 LEYES DEL TRABAJO EN EQUIPO.txt",
    "lib/21 LEYES DEL LIDERAZGO - JOHN C. MAXWELL.txt",
    "lib/25 MANERAS DE GANARSE A LA GENTE - .txt",
    "lib/Abe Shana - La última sirena.txt",
    "lib/ACTITUD DE VENCEDOR - JOHN C. MAXWELL.txt",
    "lib/El Oro Y La Ceniza - Abecassis Eliette .txt",
    "lib/SEAMOS PERSONAS DE INFLUENCIA - JOHN MAXWELL.txt",
    "lib/VIVE TU SUEÑO - JOHN MAXWELL.txt"
};
std::atomic<int> clientesAtendidos(0);
bool terminado = false;


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
                std::cout<<verde<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            }
            case 1:{
                //premium limite saldo
                int saldo = 1 + rand() % 50;
                Usuario u(i, categorias[valor], palabra, saldo);
                std::cout<<verde<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<" y saldo "<<saldo<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            } 
            case 2: {//premium ilimitado
                Usuario u(i, categorias[valor], palabra);
                std::cout<<verde<<"Usuario "<<u.getId()<<" creado con categoria "<<u.getCategoria()<<std::endl;
                hilos.emplace_back(funcionCliente, u);
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Simula el tiempo de creación del hilo

    }
    for(auto &h : hilos){
    h.join();
    }
    
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
            cvCola.wait(lock, []{ return terminado || !colaGratis.empty() || !colaPremium.empty(); });
            
            if(terminado && colaGratis.empty() && colaPremium.empty()){
                return; 
            }
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
                std::cout <<rojo<< "Cliente " << p->usuario.getId() << " sin búsquedas disponibles\n";
            } else {
                p->usuario.setBusquedasRestantes(restantes -1);
            }
            std::cout <<azul<< "Cliente " << p->usuario.getId() << " (Gratis) realizando búsqueda\n";
            int totalEncontradas = 0;

            for(const auto& libro : libros) {
                totalEncontradas += buscarEnArchivo(libro, p->usuario.getPalabra());
            }

            std::cout <<amarillo<< "Cliente " << p->usuario.getId() << " encontró " << totalEncontradas << " palabras de '" << p->usuario.getPalabra() << "'\n";
        }
        else if(categoria == "Premium limite saldo"){
            int saldo = p->usuario.getSaldo();

            std::cout <<rosa<< "Cliente " << p->usuario.getId() << " (Premium saldo: " << saldo << ")\n";

            if(saldo <= 0){
                std::cout <<rojo<< "Cliente " << p->usuario.getId() << " sin saldo, recargando...\n";

                int saldo = p->usuario.getSaldo();

                if(saldo <= 0){
                    solicitarRecarga(p->usuario.getId(), saldo);
                    p->usuario.setSaldo(saldo);
                }
            }

            saldo--;
            p->usuario.setSaldo(saldo);
            int totalEncontradas = 0;

            for(const auto& libro : libros) {
                totalEncontradas += buscarEnArchivo(libro, p->usuario.getPalabra());
            }

            std::cout <<amarillo<< "El cliente Premium " << p->usuario.getId() << " encontró " << totalEncontradas << " palabras de '" << p->usuario.getPalabra() << "'\n";

        }
        else{ // Premium ilimitado
            std::cout <<rosa<< "Cliente " << p->usuario.getId() << " (Premium ilimitado)\n";
            int totalEncontradas = 0;

            for(const auto& libro : libros) {
                totalEncontradas += buscarEnArchivo(libro, p->usuario.getPalabra());
            }

            std::cout <<amarillo<< "El cliente Premium ilimitado" << p->usuario.getId() << " encontró " << totalEncontradas << " palabras de '" << p->usuario.getPalabra() << "'\n";

        }

        {
            std::lock_guard<std::mutex> lock(p->mtx);
            p->completada = true;
        }

        //Notifico al cliente
        p->cv.notify_one();
        delete p;
        clientesAtendidos++;

        if(clientesAtendidos == NUM_HILOS){
            std::lock_guard<std::mutex> lock(mtxCola);
            terminado = true;
            cvCola.notify_all(); 
        }
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

    crearUsuarios();

    for(auto &s : servidores){
        s.join();
    }

    return 0;
}