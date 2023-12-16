// Grafo.h
#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "Cliente.h"
#include "Servidor.h"
#include "Conexion.h"

using namespace std;

class Grafo {
public:
    void agregarCliente(const Cliente& cliente) {
        clientes.push_back(cliente);
    }

    void agregarServidor(const Servidor& servidor) {
        servidores.push_back(servidor);
    }

    void agregarConexion(const Conexion& conexion) {
        conexiones.push_back(conexion);
    }

    const vector<Cliente>& getClientes() const {
        return clientes;
    }

    const vector<Servidor>& getServidores() const {
        return servidores;
    }

    const vector<Conexion>& getConexiones() const {
        return conexiones;
    }

    vector<int> bellmanFord(int source) {
        int numNodos = clientes.size() + servidores.size();
        vector<int> distancia(numNodos, numeric_limits<int>::max());
        vector<int> predecesor(numNodos, -1);

        distancia[source] = 0;

        // Relajación de las aristas
        for (int i = 1; i < numNodos - 1; ++i) {
            for (const Conexion& conexion : conexiones) {
                int u = conexion.getIdCliente();
                int v = conexion.getIdServidor();
                int peso = conexion.getDistancia();

                if (distancia[u] != numeric_limits<int>::max() && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                    predecesor[v] = u;
                }
            }
        }

        // Verificación de ciclos negativos
        for (const Conexion& conexion : conexiones) {
            int u = conexion.getIdCliente();
            int v = conexion.getIdServidor();
            int peso = conexion.getDistancia();

            if (distancia[u] != numeric_limits<int>::max() && distancia[u] + peso < distancia[v]) {
                cerr << "El grafo contiene ciclos negativos." << endl;
                return vector<int>();  // Retorna un vector vacío si hay ciclos negativos
            }
        }

        // Devolver el vector de predecesores
        return predecesor;
    }
    

    void enviarArchivo(int clienteId, int servidorId, int pesoArchivo) {
        // Aplicar Bellman-Ford desde el cliente con id proporcionado
        vector<int> predecesor = bellmanFord(clienteId);

        // Verificar si hay ciclos negativos
        if (predecesor.empty()) {
            cerr << "No se puede enviar el archivo debido a ciclos negativos." << endl;
            return;
        }

        // Calcular la cantidad total de partes que se enviarán
        int velocidadMaxima = obtenerVelocidadMaxima(clienteId, servidorId);
        const int maxTamanoParte = velocidadMaxima;  // Tamaño máximo de cada parte en MB

        // Calcular el número total de partes y el tiempo total
        int numPartesTotal = (pesoArchivo % maxTamanoParte == 0) ? pesoArchivo / maxTamanoParte : pesoArchivo / maxTamanoParte + 1;

        cout << "Enviando archivo desde el cliente " << clienteId << " al servidor " << servidorId << "..." << endl;

        int tiempoTotal = 0;  // Variable para almacenar el tiempo total

        for (int i = 0; i < pesoArchivo; i += maxTamanoParte) {
            int tamanoParte = min(maxTamanoParte, pesoArchivo - i);
            int parteActual = (i / maxTamanoParte) + 1;

            cout << "Enviando parte " << parteActual << " de " << numPartesTotal << " desde el cliente " << clienteId
                << " al servidor " << servidorId << "...";

            // Obtener nodo destino para la parte actual
            int siguienteNodo = obtenerServidorDestino(clienteId);

            // Imprimir ruta con tiempos
            cout << "Ruta: ";
            imprimirRutaConTiempos(predecesor, siguienteNodo);
            int tiempo = obtenerDistanciaEntreNodos(clienteId, siguienteNodo);
            cout << " (Tiempo: " << tiempo << " segundos)";

            // Sumar el tiempo al tiempo total
            tiempoTotal += tiempo;

            cout << endl;
        }

        // Imprimir mensaje de archivo completo enviado
        cout << "Archivo completo enviado." << endl;

        // Imprimir el tiempo total
        cout << "Tiempo total en enviarse el mensaje: " << tiempoTotal << " segundos" << endl;
    }








    // Dentro de la clase Grafo
    int obtenerServidorDestino(int clienteId) const {
        vector<int> servidoresDestino;

        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdCliente() == clienteId) {
                servidoresDestino.push_back(conexion.getIdServidor());
            }
        }

        if (servidoresDestino.empty()) {
            cerr << "No hay servidores destino disponibles para el cliente " << clienteId << "." << endl;
            return -1; // Retorna un valor no válido
        }

        // Devuelve un servidor destino de forma aleatoria
        return servidoresDestino[rand() % servidoresDestino.size()];
    }
    

    int obtenerDistanciaEntreNodos(int nodoOrigen, int nodoDestino) const {
        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdCliente() == nodoOrigen && conexion.getIdServidor() == nodoDestino) {
                return conexion.getDistancia();
            }
        }
        return -1; // Retorna un valor no válido si no hay conexión entre los nodos
    }

    void imprimirRutaConTiempos(const vector<int>& predecesor, int nodoActual) const {
        if (nodoActual == -1) {
            return;
        }

        // Obtener nodo anterior
        int nodoAnterior = predecesor[nodoActual];

        imprimirRutaConTiempos(predecesor, nodoAnterior);

        if (nodoAnterior != -1) {
            // Obtener la distancia entre nodos
            int distancia = obtenerDistanciaEntreNodos(nodoAnterior, nodoActual);
            cout << " -> (Tiempo: " << distancia << " segundos) -> ";
        }

        cout << nodoActual << " ";
    }





private:
    std::vector<Cliente> clientes;
    std::vector<Servidor> servidores;
    std::vector<Conexion> conexiones;

    int obtenerVelocidadMaxima(int clienteId, int servidorId) const {
        int velocidadMaxima = 0;

        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdCliente() == clienteId && conexion.getIdServidor() == servidorId) {
                velocidadMaxima = max(velocidadMaxima, conexion.getVelocidad());
            }
        }

        return velocidadMaxima;
    }

};

#endif // GRAFO_H
