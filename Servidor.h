// Servidor.h
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <string>

class Servidor {
public:
    Servidor(int id, const std::string& nombre, const std::string& tipo)
        : id(id), nombre(nombre), tipo(tipo) {}

    int getId() const {
        return id;
    }

    std::string getNombre() const {
        return nombre;
    }

    std::string getTipo() const {
        return tipo;
    }

private:
    int id;
    std::string nombre;
    std::string tipo;
};

#endif // SERVIDOR_H
