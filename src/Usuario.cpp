#include "Usuario.hpp"
#include <iostream>
#include <thread>
#include "SistemaBusqueda.hpp"

void funcionCliente(Usuario u){
    std::cout << "Cliente " << u.getId() << " crea petición\n";

    //Creo la peticion
    PeticionBusqueda* p = new PeticionBusqueda(u);

    //La meto en la cola
    {
        std::lock_guard<std::mutex> lock(mtxCola);
        if(u.getCategoria() == "Gratuito") {
            colaGratis.push(p);
        }else {
            colaPremium.push(p);
        }
    }

    cvCola.notify_one();

    //Espero los resultados
    {
        std::unique_lock<std::mutex> lock(p->mtx);
        p->cv.wait(lock, [&]{ return p->completada; });
    }

    std::cout << "Cliente " << u.getId() << " recibe resultado\n";
}

