// Cliente.h
#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>

class Cliente {
public:
    Cliente(int id) : id(id) {}

    int getId() const {
        return id;
    }

private:
    int id;
};

#endif // CLIENTE_H
