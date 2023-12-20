# Taller_3

Integrante: Matías Araya Álvarez

La implementación del algoritmo de Bellman-Ford se encuentra dentro de la clase Grafo, que representa la estructura del grafo.

El método bellmanFord(int source) calcula las distancias más cortas desde un nodo fuente hasta todos los demás nodos en el grafo. Se utiliza un enfoque de relajación de aristas para actualizar las distancias.

El método enviarArchivo(int clienteId, int servidorId, int pesoArchivo) utiliza el resultado del algoritmo de Bellman-Ford para simular el envío de un archivo desde un cliente a un servidor. Se tiene en cuenta la velocidad máxima de la conexión y se imprimen las rutas con los tiempos para cada parte del archivo.

Se han incluido varios métodos auxiliares, como obtenerServidorDestino, obtenerDistanciaEntreNodos, e imprimirRutaConTiempos, que facilitan el acceso a la información del grafo y mejoran la legibilidad del código.

En el archivo main.cpp, se utiliza la clase Grafo para construir un grafo a partir de archivos CSV que contienen información sobre clientes, servidores y conexiones. Luego, se solicita al usuario ingresar el peso del archivo, el ID del cliente y el id del cliente o servidor/router que recibira el archivo/mensaje. Finalmente, se realiza el envío simulado del archivo y se muestran los tiempos asociados.