/**
 * @file SerialReader.h
 * @brief Clase para manejar la lectura desde el puerto serial en Linux
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef SERIAL_READER_H
#define SERIAL_READER_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

/**
 * @class SerialReader
 * @brief Maneja la comunicación con el puerto serial (Arduino) en Linux
 * 
 * Proporciona métodos para conectar y leer datos desde un puerto
 * /dev/ttyUSB0 o /dev/ttyACM0
 */
class SerialReader {
private:
    int puerto;      ///< File descriptor del puerto
    bool conectado;  ///< Estado de la conexión
    
public:
    /**
     * @brief Constructor
     */
    SerialReader();
    
    /**
     * @brief Destructor que cierra el puerto si está abierto
     */
    ~SerialReader();
    
    /**
     * @brief Conecta al puerto serial
     * @param nombrePuerto Nombre del puerto (ej: "/dev/ttyUSB0")
     * @param baudRate Velocidad de comunicación (por defecto 9600)
     * @return true si la conexión fue exitosa, false en caso contrario
     */
    bool conectar(const char* nombrePuerto, int baudRate = 9600);
    
    /**
     * @brief Lee una línea del puerto serial
     * @param buffer Buffer donde se almacenará la línea leída
     * @param maxLen Tamaño máximo del buffer
     * @return true si se leyó una línea completa, false si hubo error
     * 
     * Lee hasta encontrar '\n' o hasta llenar el buffer
     */
    bool leerLinea(char* buffer, int maxLen);
    
    /**
     * @brief Cierra la conexión con el puerto
     */
    void cerrar();
    
    /**
     * @brief Verifica si el puerto está conectado
     * @return true si está conectado, false en caso contrario
     */
    bool estaConectado() const { return conectado; }
};

#endif // SERIAL_READER_H