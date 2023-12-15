#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Cliente.h"
#include "Servidor.h"
#include "Conexion.h"
#include "Grafo.h"

using namespace std;
int main() {
    srand(time(NULL));
    ofstream archivo;
    ofstream archivo2;
    archivo2.open("conexiones.csv");
    archivo.open("servidores.csv");
    archivo<<"id,nombre,tipo"<<endl;
    archivo2<<"idCliente,idServidor,velocidad,distancia"<<endl;
    vector<string> cliente;
    vector<string> servidor;
    for (int i = 0; i < 300; i++) {
        int num = i;
        string nombre = "Servidor" + to_string(i);
        int random = rand()%5+1;
        string tipo;
        if(random == 5){
            tipo = "router";
            servidor.push_back(to_string(i));
            for(string client:cliente){
                int random2 = rand()%10+1;
                int velocidadrand = (rand()%3+1)*300;
                archivo2<<client<<","<<i<<","<<velocidadrand<<","<<random2<<endl;
            }
            cliente.clear();
        }else{
            tipo="cliente";
            cliente.push_back(to_string(i));
        }
        archivo << num << "," << nombre << "," << tipo << endl;
    }
    archivo.close();
    for(int i = 0; i< servidor.size(); i++){
        for(int j = i+1; j< servidor.size(); j++){
            if(i!=j){
                int moneda = rand()%4+1;
                if(moneda == 1){
                    int random = rand()%1000+1;
                    int velocidad = (rand()%3+1)*1000;
                    archivo2<<servidor[i]<<","<<servidor[j]<<","<<velocidad<<","<<random<<endl;
                }
            }
        }
        
    }
    archivo2.close();

    Grafo grafo;

    // Generar clientes y agregar al grafo
    for (int i = 0; i < 5; i++) {  // Reducido para demostración, ajusta según tus necesidades
        Cliente cliente(i);
        grafo.agregarCliente(cliente);
    }

    // Generar servidores y agregar al grafo
    for (int i = 0; i < 5; i++) {  // Reducido para demostración, ajusta según tus necesidades
        string nombre = "Servidor" + to_string(i);
        int random = rand() % 5 + 1;
        string tipo = (random == 5) ? "router" : "cliente";
        Servidor servidor(i, nombre, tipo);
        grafo.agregarServidor(servidor);
    }

    // Generar conexiones y agregar al grafo
    for (int i = 0; i < grafo.getServidores().size(); i++) {
        for (int j = i + 1; j < grafo.getServidores().size(); j++) {
            if (i != j) {
                int moneda = rand() % 4 + 1;
                if (moneda == 1) {
                    int random = rand() % 1000 + 1;
                    int velocidad = (rand() % 3 + 1) * 1000;
                    Conexion conexion(grafo.getClientes()[i].getId(), grafo.getServidores()[j].getId(), velocidad, random);
                    grafo.agregarConexion(conexion);
                }
            }
        }
    }

    // Aplicar Bellman-Ford desde el cliente con id 0
    int sourceClienteId = 0;
    grafo.bellmanFord(sourceClienteId);

    // Enviar un archivo desde el cliente 0 al servidor 1 con un peso de 1200 MB
    int pesoArchivo = 1200;
    int servidorDestinoId = 1;
    grafo.enviarArchivo(sourceClienteId, servidorDestinoId, pesoArchivo);










    return 0;
}
