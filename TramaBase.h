/**
 * @file TramaBase.h
 * @brief Clase base abstracta para el sistema de tramas PRT-7
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef TRAMA_BASE_H
#define TRAMA_BASE_H

// Forward declaration
class RotorDeMapeo;

/**
 * @class TramaBase
 * @brief Clase base abstracta que define la interfaz para todas las tramas
 * 
 * Esta clase utiliza polimorfismo para permitir el procesamiento
 * uniforme de diferentes tipos de tramas (LOAD y MAP)
 */
class TramaBase {
public:
    /**
     * @brief Destructor virtual
     * 
     * CRÍTICO: Debe ser virtual para asegurar la correcta destrucción
     * de objetos derivados cuando se eliminan a través de un puntero base
     */
    virtual ~TramaBase() {}
};

#endif // TRAMA_BASE_H