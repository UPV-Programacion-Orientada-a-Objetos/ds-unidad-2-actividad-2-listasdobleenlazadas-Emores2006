/**
 * @mainpage Decodificador PRT-7
 * @brief Programa principal del Decodificador PRT-7
 * @author Eliezer Mores Oyervides
 * @date 2025
 * * @section intro_sec Introducción
 * Este programa implementa un **Decodificador PRT-7** basado en un protocolo 
 * industrial simple. Su función principal es leer tramas de un puerto serial 
 * y, en tiempo real, decodificar un mensaje aplicando una lógica de mapeo 
 * controlada por un rotor.
 * * Las tramas recibidas son de dos tipos:
 * - **Trama LOAD ('L'):** Contiene un carácter a decodificar.
 * - **Trama MAP ('M'):** Contiene un valor de rotación para el Rotor de Mapeo.
 * * @section struct_sec Estructura del Programa
 * El decodificador utiliza las siguientes clases:
 * - **SerialReader:** Maneja la conexión y lectura del puerto serial.
 * - **ListaDeCarga:** Almacena todas las tramas recibidas.
 * - **RotorDeMapeo:** Lógica de mapeo de caracteres.
 * - **TramaBase:** Clase base para polimorfismo.
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "SerialReader.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include "Tramas.h"
#include "TramaBase.h"

/**
 * @brief Parsea una línea de trama y crea el objeto correspondiente
 * @param linea Línea leída del puerto serial (ej: "L,A" o "M,5")
 * @return Puntero a TramaBase (TramaLoad o TramaMap), o nullptr si hay error
 */
TramaBase* parsearTrama(char* linea) {
    // Eliminar espacios en blanco al inicio
    while (*linea == ' ' || *linea == '\t') linea++;
    
    if (linea[0] == '\0') return nullptr;
    
    char tipo = linea[0];
    
    // Buscar la coma
    char* coma = linea;
    while (*coma != '\0' && *coma != ',') coma++;
    
    if (*coma != ',') {
        return nullptr;
    }
    
    coma++; // Saltar la coma
    
    // Eliminar espacios después de la coma
    while (*coma == ' ' || *coma == '\t') coma++;
    
    if (tipo == 'L' || tipo == 'l') {
        // Trama LOAD
        if (*coma == '\0') {
            return nullptr;
        }
        
        // Manejar "Space" como carácter especial
        if (coma[0] == 'S' && coma[1] == 'p' && coma[2] == 'a' && 
            coma[3] == 'c' && coma[4] == 'e') {
            return new TramaLoad(' ');
        }
        
        return new TramaLoad(coma[0]);
    }
    else if (tipo == 'M' || tipo == 'm') {
        // Trama MAP
        int rotacion = 0;
        
        // Parsear el número (puede ser negativo)
        bool negativo = false;
        if (*coma == '-') {
            negativo = true;
            coma++;
        } else if (*coma == '+') {
            coma++;
        }
        
        while (*coma >= '0' && *coma <= '9') {
            rotacion = rotacion * 10 + (*coma - '0');
            coma++;
        }
        
        if (negativo) rotacion = -rotacion;
        
        return new TramaMap(rotacion);
    }
    
    return nullptr;
}

/**
 * @brief Función principal del decodificador
 * @return Código de salida
 */
int main() {
    std::cout << "Iniciando Decodificador PRT-7. Conectando a puerto..." << std::endl;
    
    // Crear las estructuras de datos
    ListaDeCarga miListaDeCarga;
    RotorDeMapeo miRotorDeMapeo;
    
    // Configurar puerto serial
    SerialReader serial;
    char nombrePuerto[100];
    
    std::cout << "Ingrese el nombre del puerto (ej: /dev/ttyUSB0): ";
    std::cin.getline(nombrePuerto, 100);
    
    if (!serial.conectar(nombrePuerto, 9600)) {
        std::cerr << "ERROR: No se pudo conectar al puerto serial" << std::endl;
        return 1;
    }
    
    std::cout << "Conexión establecida. Esperando tramas..." << std::endl;
    std::cout << std::endl;
    
    // Buffer para leer líneas y construir mensaje
    char buffer[256];
    char mensajeParcial[1000];
    int posMensaje = 0;
    mensajeParcial[0] = '\0';
    
    // Bucle principal de procesamiento
    while (true) {
        if (serial.leerLinea(buffer, sizeof(buffer))) {
            
            // Verificar si es la señal de finalización
            if (buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D') {
                std::cout << std::endl;
                std::cout << "---" << std::endl;
                std::cout << "Flujo de datos terminado." << std::endl;
                break;
            }
            
            // Ignorar líneas que no sean tramas válidas
            if (buffer[0] != 'L' && buffer[0] != 'l' && 
                buffer[0] != 'M' && buffer[0] != 'm') {
                continue;
            }
            
            // Parsear y crear objeto trama
            TramaBase* trama = parsearTrama(buffer);
            
            if (trama != nullptr) {
                // Almacenar en la lista doblemente enlazada
                miListaDeCarga.insertarAlFinal(trama);
                
                // Mostrar trama recibida y procesarla en tiempo real
                std::cout << "Trama recibida: [" << buffer << "] -> Procesando... -> ";
                
                // Verificar tipo de trama usando polimorfismo
                TramaLoad* tramaLoad = dynamic_cast<TramaLoad*>(trama);
                TramaMap* tramaMap = dynamic_cast<TramaMap*>(trama);
                
                if (tramaLoad != nullptr) {
                    // Procesar TRAMA LOAD
                    char original = tramaLoad->getCaracter();
                    char decodificado = miRotorDeMapeo.getMapeo(original);
                    mensajeParcial[posMensaje++] = decodificado;
                    mensajeParcial[posMensaje] = '\0';
                    
                    std::cout << "Fragmento '" << original 
                              << "' decodificado como '" << decodificado << "'. ";
                    std::cout << "Mensaje: [";
                    
                    // Imprimir mensaje con formato [X][X][X]
                    for (int i = 0; i < posMensaje; i++) {
                        std::cout << "[" << mensajeParcial[i] << "]";
                    }
                    std::cout << "]" << std::endl;
                    
                } else if (tramaMap != nullptr) {
                    // Procesar TRAMA MAP
                    int rotacion = tramaMap->getRotacion();
                    miRotorDeMapeo.rotar(rotacion);
                    
                    std::cout << "ROTANDO ROTOR " 
                              << (rotacion >= 0 ? "+" : "") << rotacion << ". ";
                    
                    // Calcular qué mapeo genera (A->?)
                    char mapeado = miRotorDeMapeo.getMapeo('A');
                    std::cout << "(Ahora 'A' se mapea a '" << mapeado << "')" << std::endl;
                }
                
                std::cout << std::endl;
            }
        }
    }
    
    // Imprimir mensaje final
    std::cout << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    std::cout << mensajeParcial << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "Liberando memoria... Sistema apagado." << std::endl;
    
    serial.cerrar();
    
    return 0;
}