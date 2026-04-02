#ifndef SISTEMAPAGO_HPP
#define SISTEMAPAGO_HPP

#include <mutex>
#include <condition_variable>
#include <queue>

struct Peticion{
    int idCliente;
    int cantidadPago;
    int *saldoActual;
    bool procesada;
    
    std::mutex semCliente;
    std::condition_variable cvCliente;

    Peticion(int id, int *saldo) : idCliente(id), saldoActual(saldo){}; 

};

void sistema();
void solicitarRecarga(int idCliente, int& saldo);

#endif

