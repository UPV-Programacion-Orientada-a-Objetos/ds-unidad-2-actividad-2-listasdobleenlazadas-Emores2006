/**
 * @file ListaDeCarga.cpp
 * @brief Implementación de la clase ListaDeCarga
 * @author Eliezer Mores Oyervides
 */

#include "ListaDeCarga.h"
#include "Tramas.h"
#include <iostream>

ListaDeCarga::ListaDeCarga() : cabeza(nullptr), cola(nullptr), tamano(0) {}

ListaDeCarga::~ListaDeCarga() {
    NodoCarga* actual = cabeza;
    while (actual != nullptr) {
        NodoCarga* siguiente = actual->siguiente;
        // Liberar la trama apuntada
        delete actual->trama;
        // Liberar el nodo
        delete actual;
        actual = siguiente;
    }
}

void ListaDeCarga::insertarAlFinal(TramaBase* trama) {
    NodoCarga* nuevo = new NodoCarga(trama);
    
    if (cabeza == nullptr) {
        // Lista vacía
        cabeza = nuevo;
        cola = nuevo;
    } else {
        // Insertar al final
        cola->siguiente = nuevo;
        nuevo->previo = cola;
        cola = nuevo;
    }
    
    tamano++;
}

void ListaDeCarga::procesarTramas(RotorDeMapeo* rotor) {
    // Este método es para procesamiento batch (no se usa en el modo tiempo real)
    // pero se mantiene por si se necesita reprocesar la lista
    
    if (cabeza == nullptr) {
        std::cout << "[Lista vacía - sin tramas para procesar]" << std::endl;
        return;
    }
    
    // Buffer temporal para almacenar caracteres decodificados
    char* mensajeTemp = new char[tamano + 1];
    int posicionMensaje = 0;
    
    std::cout << "Procesando " << tamano << " tramas almacenadas..." << std::endl;
    std::cout << "========================================" << std::endl;
    
    NodoCarga* actual = cabeza;
    int numeroTrama = 1;
    
    while (actual != nullptr) {
        TramaBase* trama = actual->trama;
        
        // Determinar el tipo de trama (usando dynamic_cast)
        TramaLoad* tramaLoad = dynamic_cast<TramaLoad*>(trama);
        TramaMap* tramaMap = dynamic_cast<TramaMap*>(trama);
        
        if (tramaLoad != nullptr) {
            // Es una trama LOAD
            char original = tramaLoad->getCaracter();
            char decodificado = rotor->getMapeo(original);
            mensajeTemp[posicionMensaje++] = decodificado;
            
            std::cout << "Trama #" << numeroTrama << " [LOAD,'" << original << "'] -> ";
            std::cout << "Decodificado como '" << decodificado << "'" << std::endl;
            
        } else if (tramaMap != nullptr) {
            // Es una trama MAP
            int rotacion = tramaMap->getRotacion();
            rotor->rotar(rotacion);
            
            std::cout << "Trama #" << numeroTrama << " [MAP," << rotacion << "] -> ";
            std::cout << "ROTANDO ROTOR " << (rotacion >= 0 ? "+" : "") << rotacion << std::endl;
        }
        
        actual = actual->siguiente;
        numeroTrama++;
    }
    
    mensajeTemp[posicionMensaje] = '\0';
    
    std::cout << "========================================" << std::endl;
    std::cout << "MENSAJE OCULTO DECODIFICADO:" << std::endl;
    std::cout << mensajeTemp << std::endl;
    std::cout << "========================================" << std::endl;
    
    delete[] mensajeTemp;
}

void ListaDeCarga::imprimirMensajeFinal() {
    if (cabeza == nullptr) {
        std::cout << "[Sin mensaje]" << std::endl;
        return;
    }
    
    std::cout << "Contenido de la lista de tramas:" << std::endl;
    
    NodoCarga* actual = cabeza;
    int contador = 1;
    
    while (actual != nullptr) {
        TramaLoad* tramaLoad = dynamic_cast<TramaLoad*>(actual->trama);
        TramaMap* tramaMap = dynamic_cast<TramaMap*>(actual->trama);
        
        std::cout << "  " << contador << ". ";
        
        if (tramaLoad != nullptr) {
            std::cout << "[LOAD: '" << tramaLoad->getCaracter() << "']";
        } else if (tramaMap != nullptr) {
            std::cout << "[MAP: " << tramaMap->getRotacion() << "]";
        } else {
            std::cout << "[Trama desconocida]";
        }
        
        std::cout << std::endl;
        
        actual = actual->siguiente;
        contador++;
    }
}