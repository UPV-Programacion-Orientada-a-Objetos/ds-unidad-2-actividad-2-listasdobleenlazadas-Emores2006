/**
 * @file SerialReader.cpp
 * @brief Implementación de la clase SerialReader para Linux
 * @author Eliezer Mores Oyervides
 */

#include "SerialReader.h"
#include <iostream>
#include <cstring>

SerialReader::SerialReader() : puerto(-1), conectado(false) {}

SerialReader::~SerialReader() {
    cerrar();
}

bool SerialReader::conectar(const char* nombrePuerto, int baudRate) {
    // Abrir el puerto serial
    puerto = open(nombrePuerto, O_RDWR | O_NOCTTY);
    
    if (puerto < 0) {
        std::cerr << "Error: No se pudo abrir el puerto " << nombrePuerto << std::endl;
        std::cerr << "Verifique que el dispositivo exista y tenga permisos." << std::endl;
        return false;
    }
    
    // Obtener configuración actual
    struct termios tty;
    if (tcgetattr(puerto, &tty) != 0) {
        std::cerr << "Error: No se pudo obtener la configuración del puerto" << std::endl;
        close(puerto);
        puerto = -1;
        return false;
    }
    
    // Configurar velocidad (baud rate)
    speed_t speed = B9600;
    if (baudRate == 115200) speed = B115200;
    else if (baudRate == 57600) speed = B57600;
    else if (baudRate == 38400) speed = B38400;
    else if (baudRate == 19200) speed = B19200;
    else if (baudRate == 4800) speed = B4800;
    
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    
    // Configuración 8N1 (8 bits, sin paridad, 1 bit de parada)
    tty.c_cflag &= ~PARENB;        // Sin paridad
    tty.c_cflag &= ~CSTOPB;        // 1 bit de parada
    tty.c_cflag &= ~CSIZE;         // Limpiar tamaño de bits
    tty.c_cflag |= CS8;            // 8 bits por byte
    
    // Deshabilitar control de flujo por hardware
    tty.c_cflag &= ~CRTSCTS;
    
    // Habilitar lectura e ignorar líneas de control
    tty.c_cflag |= CREAD | CLOCAL;
    
    // Modo no canónico (raw)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;          // Deshabilitar eco
    tty.c_lflag &= ~ECHOE;         // Deshabilitar eco de borrado
    tty.c_lflag &= ~ECHONL;        // Deshabilitar eco de nueva línea
    tty.c_lflag &= ~ISIG;          // Deshabilitar señales
    
    // Deshabilitar control de flujo por software
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    // Deshabilitar procesamiento especial de entrada
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    
    // Deshabilitar procesamiento especial de salida
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    
    // Configurar timeout de lectura
    tty.c_cc[VTIME] = 1;  // Timeout de 0.1 segundos
    tty.c_cc[VMIN] = 0;   // Retornar inmediatamente con datos disponibles
    
    // Aplicar configuración
    if (tcsetattr(puerto, TCSANOW, &tty) != 0) {
        std::cerr << "Error: No se pudo aplicar la configuración" << std::endl;
        close(puerto);
        puerto = -1;
        return false;
    }
    
    // Limpiar buffers
    tcflush(puerto, TCIOFLUSH);
    
    conectado = true;
    return true;
}

bool SerialReader::leerLinea(char* buffer, int maxLen) {
    if (!conectado || puerto < 0) return false;
    
    int pos = 0;
    
    while (pos < maxLen - 1) {
        char c;
        int n = read(puerto, &c, 1);
        
        if (n < 0) {
            // Error de lectura
            std::cerr << "Error al leer del puerto serial" << std::endl;
            return false;
        } else if (n == 0) {
            // No hay datos disponibles, continuar esperando
            continue;
        }
        
        // Verificar fin de línea
        if (c == '\n' || c == '\r') {
            if (pos > 0) {
                buffer[pos] = '\0';
                return true;
            }
            // Ignorar líneas vacías
        } else {
            buffer[pos++] = c;
        }
    }
    
    // Buffer lleno
    buffer[pos] = '\0';
    return pos > 0;
}

void SerialReader::cerrar() {
    if (conectado && puerto >= 0) {
        close(puerto);
        puerto = -1;
        conectado = false;
    }
}