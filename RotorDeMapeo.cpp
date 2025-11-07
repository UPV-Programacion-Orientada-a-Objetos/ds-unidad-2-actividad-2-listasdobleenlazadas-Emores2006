/**
 * @file RotorDeMapeo.cpp
 * @brief Implementación de la clase RotorDeMapeo
 * @author Eliezer Mores Oyervides
 */

#include "RotorDeMapeo.h"
#include <iostream>

RotorDeMapeo::RotorDeMapeo() : cabeza(nullptr), tamano(0) {
    // Crear el alfabeto A-Z (SIN espacio, el espacio no se cifra)
    const char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    NodoRotor* ultimo = nullptr;
    
    for (int i = 0; alfabeto[i] != '\0'; i++) {
        NodoRotor* nuevo = new NodoRotor(alfabeto[i]);
        
        if (cabeza == nullptr) {
            // Primer nodo
            cabeza = nuevo;
            ultimo = nuevo;
        } else {
            // Enlazar con el anterior
            ultimo->siguiente = nuevo;
            nuevo->previo = ultimo;
            ultimo = nuevo;
        }
        tamano++;
    }
    
    // Cerrar el círculo
    if (cabeza != nullptr && ultimo != nullptr) {
        ultimo->siguiente = cabeza;
        cabeza->previo = ultimo;
    }
}

RotorDeMapeo::~RotorDeMapeo() {
    if (cabeza == nullptr) return;
    
    // Romper el círculo
    NodoRotor* ultimo = cabeza->previo;
    ultimo->siguiente = nullptr;
    
    // Eliminar todos los nodos
    NodoRotor* actual = cabeza;
    while (actual != nullptr) {
        NodoRotor* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

void RotorDeMapeo::rotar(int n) {
    if (cabeza == nullptr) return;
    
    // Normalizar n al rango del tamaño
    n = n % tamano;
    if (n < 0) n += tamano;
    
    // Rotar moviendo la cabeza
    for (int i = 0; i < n; i++) {
        cabeza = cabeza->siguiente;
    }
}

char RotorDeMapeo::getMapeo(char in) {
    if (cabeza == nullptr) return in;
    
    // Convertir a mayúscula si es letra
    if (in >= 'a' && in <= 'z') {
        in = in - 'a' + 'A';
    }
    
    // IMPORTANTE: El espacio NO se cifra, siempre retorna espacio
    if (in == ' ') {
        return ' ';
    }
    
    // Si no es una letra mayúscula, devolver sin cambios
    if (in < 'A' || in > 'Z') {
        return in;
    }
    
    // Crear un rotor de referencia (sin rotación) para encontrar posición original
    const char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int posicionOriginal = -1;
    
    // Encontrar la posición del carácter en el alfabeto original
    for (int i = 0; alfabeto[i] != '\0'; i++) {
        if (alfabeto[i] == in) {
            posicionOriginal = i;
            break;
        }
    }
    
    if (posicionOriginal == -1) return in;
    
    // Ahora encontrar qué carácter está en esa posición en el rotor rotado
    NodoRotor* resultado = cabeza;
    for (int i = 0; i < posicionOriginal; i++) {
        resultado = resultado->siguiente;
    }
    
    return resultado->dato;
}

void RotorDeMapeo::imprimirEstado() {
    if (cabeza == nullptr) {
        std::cout << "Rotor vacío" << std::endl;
        return;
    }
    
    std::cout << "Estado del rotor (cabeza='";
    std::cout << cabeza->dato << "'): ";
    
    NodoRotor* actual = cabeza;
    int count = 0;
    do {
        std::cout << actual->dato;
        actual = actual->siguiente;
        count++;
        if (count >= tamano) break;
    } while (actual != cabeza);
    
    std::cout << std::endl;
}