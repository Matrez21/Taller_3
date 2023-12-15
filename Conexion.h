// Conexion.h
#ifndef CONEXION_H
#define CONEXION_H

#include <iostream>

class Conexion {
public:
    Conexion(int idCliente, int idServidor, int velocidad, int distancia)
        : idCliente(idCliente), idServidor(idServidor), velocidad(velocidad), distancia(distancia) {}

    int getIdCliente() const {
        return idCliente;
    }

    int getIdServidor() const {
        return idServidor;
    }

    int getVelocidad() const {
        return velocidad;
    }

    int getDistancia() const {
        return distancia;
    }

private:
    int idCliente;
    int idServidor;
    int velocidad;
    int distancia;
};

#endif // CONEXION_H
