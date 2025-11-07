/**
 * @file RotorDeMapeo.h
 * @brief Implementación de lista circular doblemente enlazada para mapeo de caracteres
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef ROTOR_DE_MAPEO_H
#define ROTOR_DE_MAPEO_H

/**
 * @struct NodoRotor
 * @brief Nodo de la lista circular que contiene un carácter
 */
struct NodoRotor {
    char dato;           ///< Carácter almacenado
    NodoRotor* siguiente; ///< Puntero al siguiente nodo
    NodoRotor* previo;    ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param c Carácter a almacenar
     */
    NodoRotor(char c) : dato(c), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class RotorDeMapeo
 * @brief Lista circular doblemente enlazada que simula un disco de cifrado
 * 
 * Funciona como una Rueda de César que puede rotar para cambiar
 * el mapeo entre caracteres de entrada y salida
 */
class RotorDeMapeo {
private:
    NodoRotor* cabeza;  ///< Puntero a la posición 'cero' actual del rotor
    int tamano;         ///< Número de elementos en el rotor
    
public:
    /**
     * @brief Constructor que inicializa el rotor con el alfabeto A-Z
     * 
     * IMPORTANTE: El espacio NO se incluye en el rotor circular.
     * Los espacios siempre se mantienen como espacios sin cifrado.
     */
    RotorDeMapeo();
    
    /**
     * @brief Destructor que libera toda la memoria
     */
    ~RotorDeMapeo();
    
    /**
     * @brief Rota el rotor N posiciones
     * @param n Número de posiciones a rotar (positivo: adelante, negativo: atrás)
     * 
     * Ejemplo: rotar(2) hace que 'A' se mapee a 'C'
     */
    void rotar(int n);
    
    /**
     * @brief Obtiene el carácter mapeado según la rotación actual
     * @param in Carácter de entrada
     * @return Carácter decodificado según la posición del rotor
     * 
     * La lógica: encuentra 'in' en el rotor, calcula su distancia
     * desde cabeza, y devuelve el carácter que está a esa distancia
     */
    char getMapeo(char in);
    
    /**
     * @brief Imprime el estado actual del rotor (para debugging)
     */
    void imprimirEstado();
};

#endif // ROTOR_DE_MAPEO_H