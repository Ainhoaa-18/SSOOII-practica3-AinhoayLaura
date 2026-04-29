#include "SistemaPago.hpp"
#include "SistemaBusqueda.hpp"
#include <iostream>

std::mutex semaforo;
std::condition_variable cvSistema;
std::queue<Peticion*> cola;


void solicitarRecarga(int idCliente, int& saldo){ //llama cliente a este metodo cuando saldo = 0;
    Peticion* p = new Peticion(idCliente, &saldo);

    p->cantidadPago = rand() % 50 + 1;
    p->procesada = false;

    {
        std::lock_guard<std::mutex> lock(semaforo);
        cola.push(p);
        std::cout<<rojo<<"Cliente "<<idCliente<<" solicita recarga de saldo"<<std::endl;
    }
    cvSistema.notify_one();

    {
        std::unique_lock<std::mutex> lock(p->semCliente);
        p->cvCliente.wait(lock,[&]{ return p->procesada; });
    }

    

}

void sistema(){
    while(true) {
        Peticion *p;
        {
            std::unique_lock<std::mutex> lock(semaforo);
            cvSistema.wait(lock,[]{return !cola.empty();});

            p = cola.front();
            cola.pop();
        }
        
        std::cout<<verde<<"Procesando petición: id cliente = "<<p->idCliente<<", cantidad de pago = "<<p->cantidadPago<<", saldo actual = "<<p->saldoActual<<std::endl;

        *(p->saldoActual) += p->cantidadPago;
        
        {
            std::lock_guard<std::mutex> lock(p->semCliente);
            p->procesada = true;
        }

        p->cvCliente.notify_one();
    }   
}
