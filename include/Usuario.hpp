#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>

class Usuario{
private:
    int idUsuario;
    std::string categoria;
    std::string palabraBuscada;
    int saldo;
    int busquedasRestantes = 10;

    public:
        Usuario(int id, std::string cat, std::string palabra, int saldo = -1)
        : idUsuario(id), categoria(cat), palabraBuscada(palabra), saldo(saldo) {}

        int getId() const { return idUsuario; }
        std::string getCategoria() const { return categoria; }
        std::string getPalabra() const { return palabraBuscada; }
        int getSaldo() const {return saldo; }
        void setSaldo(int s) { saldo = s; }
        int getBusquedasRestantes() const { return busquedasRestantes; }
        void setBusquedasRestantes(int n) { busquedasRestantes = n; }

        static void crearHilos();
        static void realizarPeticionBusqueda(int id);
        static void mostrarResultadosBusqueda(int id);

        
};
void funcionCliente(Usuario u);
#endif