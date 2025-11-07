/**
 * @file Tramas.h
 * @brief Clases derivadas para los dos tipos de tramas: LOAD y MAP
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef TRAMAS_H
#define TRAMAS_H

#include "TramaBase.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaLoad
 * @brief Representa una trama de carga (L,X)
 * 
 * Contiene un fragmento de dato que será decodificado
 * usando el estado actual del rotor
 */
class TramaLoad : public TramaBase {
private:
    char caracter;  ///< Carácter a decodificar
    
public:
    /**
     * @brief Constructor
     * @param c Carácter de la trama
     */
    TramaLoad(char c) : caracter(c) {}
    
    /**
     * @brief Obtiene el carácter de la trama
     * @return Carácter almacenado
     */
    char getCaracter() const { return caracter; }
};

/**
 * @class TramaMap
 * @brief Representa una trama de mapeo (M,N)
 * 
 * Contiene una instrucción para rotar el disco de cifrado,
 * cambiando el mapeo para las futuras tramas LOAD
 */
class TramaMap : public TramaBase {
private:
    int rotacion;  ///< Número de posiciones a rotar
    
public:
    /**
     * @brief Constructor
     * @param n Número de posiciones a rotar
     */
    TramaMap(int n) : rotacion(n) {}
    
    /**
     * @brief Obtiene el valor de rotación
     * @return Número de posiciones a rotar
     */
    int getRotacion() const { return rotacion; }
};

#endif // TRAMAS_H