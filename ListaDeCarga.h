/**
 * @file ListaDeCarga.h
 * @brief Implementación de lista doblemente enlazada para almacenar tramas
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef LISTA_DE_CARGA_H
#define LISTA_DE_CARGA_H

#include "TramaBase.h"

/**
 * @struct NodoCarga
 * @brief Nodo de la lista doblemente enlazada que almacena punteros a TramaBase
 */
struct NodoCarga {
    TramaBase* trama;     ///< Puntero polimórfico a la trama (LOAD o MAP)
    NodoCarga* siguiente; ///< Puntero al siguiente nodo
    NodoCarga* previo;    ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param t Puntero a la trama a almacenar
     */
    NodoCarga(TramaBase* t) : trama(t), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class ListaDeCarga
 * @brief Lista doblemente enlazada que almacena las tramas recibidas en orden
 * 
 * Esta lista mantiene todas las tramas (LOAD y MAP) en el orden en que fueron
 * recibidas del puerto serial, permitiendo procesarlas secuencialmente
 */
class ListaDeCarga {
private:
    NodoCarga* cabeza;  ///< Primer nodo de la lista
    NodoCarga* cola;    ///< Último nodo de la lista
    int tamano;         ///< Número de elementos
    
public:
    /**
     * @brief Constructor que inicializa una lista vacía
     */
    ListaDeCarga();
    
    /**
     * @brief Destructor que libera toda la memoria
     * 
     * IMPORTANTE: También libera las tramas apuntadas (delete trama)
     */
    ~ListaDeCarga();
    
    /**
     * @brief Inserta una trama al final de la lista
     * @param trama Puntero a la trama a insertar
     */
    void insertarAlFinal(TramaBase* trama);
    
    /**
     * @brief Procesa todas las tramas en orden
     * @param rotor Puntero al rotor de mapeo
     * 
     * Recorre la lista y ejecuta el método procesar() de cada trama polimórficamente
     */
    void procesarTramas(RotorDeMapeo* rotor);
    
    /**
     * @brief Imprime el mensaje final decodificado
     * 
     * Extrae y muestra solo los caracteres resultantes de las TramaLoad
     */
    void imprimirMensajeFinal();
    
    /**
     * @brief Obtiene el tamaño de la lista
     * @return Número de tramas almacenadas
     */
    int getTamano() const { return tamano; }
    
    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía, false en caso contrario
     */
    bool estaVacia() const { return cabeza == nullptr; }
};

#endif // LISTA_DE_CARGA_H