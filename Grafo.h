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

            for (int i = 1; i <= numNodos - 1; ++i) {
                for (const Conexion& conexion : conexiones) {
                    int u = conexion.getIdCliente();
                    int v = conexion.getIdServidor();
                    int peso = conexion.getDistancia();

                    // Considerar ambas direcciones
                    if (distancia[u] != numeric_limits<int>::max() && distancia[u] + peso < distancia[v]) {
                        distancia[v] = distancia[u] + peso;
                        predecesor[v] = u;
                    }

                    if (distancia[v] != numeric_limits<int>::max() && distancia[v] + peso < distancia[u]) {
                        distancia[u] = distancia[v] + peso;
                        predecesor[u] = v;
                    }
                }

                // Nuevas conexiones entre routers
                for (const Servidor& routerA : servidores) {
                    for (const Servidor& routerB : servidores) {
                        // Evitar autoconexiones
                        if (routerA.getId() != routerB.getId()) {
                            int idRouterA = routerA.getId();
                            int idRouterB = routerB.getId();

                            Conexion conexionRouterARouterB(idRouterA, idRouterB, 0, 0);
                            Conexion conexionRouterBRouterA(idRouterB, idRouterA, 0, 0);

                            if (distancia[idRouterA] != numeric_limits<int>::max() && distancia[idRouterA] + conexionRouterARouterB.getDistancia() < distancia[idRouterB]) {
                                distancia[idRouterB] = distancia[idRouterA] + conexionRouterARouterB.getDistancia();
                                predecesor[idRouterB] = idRouterA;
                            }

                            if (distancia[idRouterB] != numeric_limits<int>::max() && distancia[idRouterB] + conexionRouterBRouterA.getDistancia() < distancia[idRouterA]) {
                                distancia[idRouterA] = distancia[idRouterB] + conexionRouterBRouterA.getDistancia();
                                predecesor[idRouterA] = idRouterB;
                            }
                        }
                    }
                }
            }

        return predecesor;
    }


    

void enviarArchivo(int clienteOrigenId, int nodoDestinoId, int pesoArchivo) {
    // Aplicar Bellman-Ford desde el cliente con id proporcionado
    vector<int> predecesor = bellmanFord(clienteOrigenId);

    // Verificar si hay ciclos negativos
    if (predecesor.empty()) {
        cerr << "No se puede enviar el archivo debido a ciclos negativos." << endl;
        return;
    }

    // Calcular la cantidad total de partes que se enviarán
    int velocidadMaxima = obtenerVelocidadMaxima(clienteOrigenId);
    const int maxTamanoParte = velocidadMaxima;  // Tamaño máximo de cada parte en MB

    int tiempoTotal = 0;  // Variable para almacenar el tiempo total
    int numPartesTotal = (pesoArchivo % maxTamanoParte == 0) ? pesoArchivo / maxTamanoParte : pesoArchivo / maxTamanoParte + 1;

    for (int i = 0; i < numPartesTotal; ++i) {
        int tamanoParte = min(maxTamanoParte, pesoArchivo - i * maxTamanoParte);
        int parteActual = i + 1;

        cout << "Enviando parte " << parteActual << " de " << numPartesTotal << " desde ";

        if (esCliente(clienteOrigenId)) {
            cout << "el cliente " << clienteOrigenId;
        } else if (esRouter(clienteOrigenId)) {
            cout << "el router " << clienteOrigenId;
        } else {
            cerr << "Tipo de nodo no reconocido para " << clienteOrigenId;
            return;
        }

        cout << " al ";

        if (esCliente(nodoDestinoId)) {
            cout << "cliente " << nodoDestinoId;
        } else if (esRouter(nodoDestinoId)) {
            cout << "router " << nodoDestinoId;
        } else {
            cerr << "Tipo de nodo no reconocido para " << nodoDestinoId;
            return;
        }

        cout << "...";

        // Obtener nodo destino para la parte actual
        int siguienteNodoParte = obtenerSiguienteNodoEnRuta(predecesor, clienteOrigenId, nodoDestinoId);

        // Calcular el tiempo para esta parte y sumarlo al tiempo total
        int tiempo = obtenerDistanciaEntreNodos(clienteOrigenId, siguienteNodoParte);
        tiempoTotal += tiempo;

        cout << " (Tiempo: " << tiempo << " segundos)" << endl;
    }

    // Imprimir la ruta completa una vez
    int siguienteNodoCompleto = obtenerSiguienteNodoEnRuta(predecesor, clienteOrigenId, nodoDestinoId);
    cout << "Ruta completa: ";
    imprimirRutaCompleta(predecesor, clienteOrigenId, siguienteNodoCompleto);

    // Imprimir mensaje de archivo completo enviado
    cout << "Archivo completo enviado." << endl;

    // Imprimir el tiempo total
    cout << "Tiempo total en enviarse el mensaje: " << tiempoTotal << " segundos" << endl;
}




    bool esCliente(int nodoId) const {
        for (const Cliente& cliente : clientes) {
            if (cliente.getId() == nodoId) {
                return true;
            }
        }

        return false;
    }

    bool esRouter(int nodoId) const {
        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdServidor() == nodoId || conexion.getIdCliente() == nodoId) {
                return true;
            }
        }

        return false;
    }


    void imprimirRutaCompleta(const vector<int>& predecesor, int nodoOrigen, int nodoDestino) const {
        if (nodoDestino == -1) {
            cerr << "No se encontró una conexión válida entre " << nodoOrigen << " y el destino." << endl;
            return;
        }

        imprimirRutaConTiempos(predecesor, nodoDestino);
        cout << endl;
    }


    
    int obtenerSiguienteNodoEnRuta(const vector<int>& predecesor, int clienteOrigenId, int nodoDestinoId) {
        int siguienteNodo = predecesor[nodoDestinoId];

        // Si no hay conexión directa, buscar un nodo intermedio
        if (siguienteNodo == -1) {
            // Buscar el primer router en la ruta
            for (int i = 0; i < predecesor.size(); ++i) {
                if (predecesor[i] != -1 && esRouter(i)) {
                    return i;
                }
            }

            cerr << "No se encontró una conexión entre " << clienteOrigenId << " y " << nodoDestinoId << endl;
            return -1;
        }

        return siguienteNodo;
    }

    
    int obtenerServidorDestino(int clienteId) const {
        vector<int> nodosDestino;

        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdCliente() == clienteId) {
                nodosDestino.push_back(conexion.getIdServidor());
            }
        }

        if (nodosDestino.empty()) {
            cerr << "No hay nodos destino disponibles para el cliente " << clienteId << "." << endl;
            return -1; // Retorna un valor no válido
        }

        // Devuelve un nodo destino de forma aleatoria
        return nodosDestino[rand() % nodosDestino.size()];
    }

    

int obtenerDistanciaEntreNodos(int nodoOrigen, int nodoDestino) const {
    for (const Conexion& conexion : conexiones) {
        if (conexion.getIdCliente() == nodoOrigen) {
            return conexion.getDistancia();
        }
    }

    cout << "DEBUG: No se encontró una conexión entre " << nodoOrigen << " y " << nodoDestino << endl;

    return -1; // Retorna un valor no válido si no hay conexión entre los nodos
}

    void imprimirRutaConTiempos(const vector<int>& predecesor, int nodoActual) const {
        if (nodoActual == -1) {
            return;
        }

        // Obtener nodo anterior
        int nodoAnterior = predecesor[nodoActual];

        imprimirRutaConTiempos(predecesor, nodoAnterior);

        cout << "-> " <<nodoActual << " ";
    }









private:
    std::vector<Cliente> clientes;
    std::vector<Servidor> servidores;
    std::vector<Conexion> conexiones;

    int obtenerVelocidadMaxima(int clienteId) const {
        int velocidadMaxima = 0;

        for (const Conexion& conexion : conexiones) {
            if (conexion.getIdCliente() == clienteId) {
                velocidadMaxima = max(velocidadMaxima, conexion.getVelocidad());
            }
        }

        return velocidadMaxima;
    }

};

#endif // GRAFO_H
