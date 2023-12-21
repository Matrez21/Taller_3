#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Cliente.h"
#include "Servidor.h"
#include "Conexion.h"
#include "Grafo.h"

using namespace std;

// Funciones para leer archivos CSV y construir el grafo
Grafo construirGrafoDesdeArchivos(const string& archivoServidores, const string& archivoConexiones) {
    Grafo grafo;

    // Leer servidores
    ifstream archivoServ(archivoServidores);
    string lineaServ;

    while (getline(archivoServ, lineaServ)) {
        istringstream ss(lineaServ);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        int id = stoi(tokens[0]);
        string nombre = tokens[1];
        string tipo = tokens[2];

        if (tipo == "cliente") {
            Cliente cliente(id);
            grafo.agregarCliente(cliente);
        } else if (tipo == "router") {
            Servidor router(id, nombre, tipo);
            grafo.agregarServidor(router);
        }
    }

    // Leer conexiones
    ifstream archivoConex(archivoConexiones);
    string lineaConex;

    while (getline(archivoConex, lineaConex)) {
        istringstream ss(lineaConex);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        int idCliente = stoi(tokens[0]);
        int idRouter = stoi(tokens[1]);
        int velocidad = stoi(tokens[2]);
        int distancia = stoi(tokens[3]);

        // Conexión cliente-router (bidireccional)
        Conexion conexionCliente(idCliente, idRouter, velocidad, distancia);
        Conexion conexionRouter(idRouter, idCliente, velocidad, distancia);
        grafo.agregarConexion(conexionCliente);
        grafo.agregarConexion(conexionRouter);

        // Conexiones entre routers (bidireccionales)
        for (const Servidor& router : grafo.getServidores()) {
            if (router.getId() != idRouter) {
                Conexion conexionRouterRouter(idRouter, router.getId(), velocidad, distancia);
                Conexion conexionRouterRouterReversa(router.getId(), idRouter, velocidad, distancia);
                grafo.agregarConexion(conexionRouterRouter);
                grafo.agregarConexion(conexionRouterRouterReversa);
            }
        }
    }


    return grafo;
}

int main() {
    srand(time(NULL));

    int pesoArchivo;
    int clienteOrigenId;
    int nodoDestinoId;

    cout << "Ingrese el peso del archivo (en MB): ";
    cin >> pesoArchivo;

    cout << "Ingrese el ID del nodo de origen: ";
    cin >> clienteOrigenId;

    cout << "Ingrese el ID del nodo destino: ";
    cin >> nodoDestinoId;

    // Archivos CSV
    string archivoServidores = "servidores.csv";
    string archivoConexiones = "conexiones.csv";

    // Construir el grafo desde los archivos CSV
    Grafo grafo = construirGrafoDesdeArchivos(archivoServidores, archivoConexiones);

    // Enviar un archivo desde el cliente de origen al nodo destino con el peso especificado
    grafo.enviarArchivo(clienteOrigenId, nodoDestinoId, pesoArchivo);

    return 0;
}
