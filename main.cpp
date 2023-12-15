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
        int idServidor = stoi(tokens[1]);
        int velocidad = stoi(tokens[2]);
        int distancia = stoi(tokens[3]);

        Conexion conexion(idCliente, idServidor, velocidad, distancia);
        grafo.agregarConexion(conexion);
    }

    return grafo;
}

int main() {
    
    srand(time(NULL));

    int pesoArchivo;
    int clienteId;

    cout << "Ingrese el peso del archivo (en MB): ";
    cin >> pesoArchivo;

    cout << "Ingrese el ID del cliente: ";
    cin >> clienteId;

    // Archivos CSV
    string archivoServidores = "servidores.csv";
    string archivoConexiones = "conexiones.csv";

    // Construir el grafo desde los archivos CSV
    Grafo grafo = construirGrafoDesdeArchivos(archivoServidores, archivoConexiones);

    // Obtener un servidor destino disponible
    int servidorDestinoId = grafo.obtenerServidorDestino(clienteId);

    if (servidorDestinoId == -1) {
        // Salir del programa con un código de error si no hay servidores destino disponibles
        return 1;
    }

    // Enviar un archivo desde el cliente 0 al servidor destino con un peso de 1200 MB
    grafo.enviarArchivo(clienteId, servidorDestinoId, pesoArchivo);

    return 0;
}

