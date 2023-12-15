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

    void bellmanFord(int source) {
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
                return;
            }
        }

        // Imprimir distancias y rutas
        for (int i = 0; i < numNodos; ++i) {
            cout << "Desde el nodo " << source << " al nodo " << i << ": ";
            if (distancia[i] == numeric_limits<int>::max()) {
                cout << "No alcanzable";
            } else {
                cout << distancia[i];
            }

            // Imprimir ruta
            cout << " [Ruta: ";
            imprimirRuta(predecesor, i);
            cout << "]" << endl;
        }

    }

    

    void enviarArchivo(int clienteId, int servidorId, int pesoArchivo) {
        const int maxTamanoParte = 300;  // Tamaño máximo de cada parte en MB
        const int velocidadConexion = 1000;  // Velocidad de conexión en MB por segundo

        int tiempoTotal = 0;

        // Calcular la cantidad total de partes que se enviarán
        int numPartesTotal = (pesoArchivo % maxTamanoParte == 0) ? pesoArchivo / maxTamanoParte : pesoArchivo / maxTamanoParte + 1;

        cout << "Enviando archivo desde el cliente " << clienteId << " al servidor " << servidorId << "..." << endl;

        for (int i = 0; i < pesoArchivo; i += maxTamanoParte) {
            int tamanoParte = min(maxTamanoParte, pesoArchivo - i);
            int numPartes = (tamanoParte % maxTamanoParte == 0) ? tamanoParte / maxTamanoParte : tamanoParte / maxTamanoParte + 1;

            // Calcular tiempo de conexión para cada parte
            int tiempoConexion = numPartes / velocidadConexion;

            // Simular la conexión de cada parte al servidor
            for (int j = 0; j < numPartes; ++j) {
                // Lógica para enviar la parte al servidor (simulado)
                int parteActual = (i / maxTamanoParte) + j + 1;
                cout << "Enviando parte " << parteActual << " de " << numPartesTotal << " desde el cliente " << clienteId
                    << " al servidor " << servidorId << "..." << endl;

                // Simular el tiempo de conexión
                cout << "Conexión establecida. Transfiriendo datos..." << endl;
                for (int tiempo = 1; tiempo <= tiempoConexion; ++tiempo) {
                    cout << "Tiempo transcurrido: " << tiempo << " segundo(s)." << endl;
                }
                cout << "Transferencia completa." << endl;
            }

            tiempoTotal += tiempoConexion;
        }

        // Imprimir el tiempo total de conexión
        cout << "Archivo completo enviado en un tiempo total de " << tiempoTotal << " segundos." << endl;
    }


private:
    std::vector<Cliente> clientes;
    std::vector<Servidor> servidores;
    std::vector<Conexion> conexiones;

    void imprimirRuta(const vector<int>& predecesor, int nodoActual) const {
        if (nodoActual == -1) {
            return;
        }
        imprimirRuta(predecesor, predecesor[nodoActual]);
        cout << nodoActual << " ";
    }
};

#endif // GRAFO_H
