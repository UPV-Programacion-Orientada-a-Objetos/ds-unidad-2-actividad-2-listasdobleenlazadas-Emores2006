cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(DecodificadorPRT7 VERSION 1.0 LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Verificar que estamos en Linux
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "Este proyecto solo es compatible con Linux")
endif()

# Archivos fuente
set(SOURCES
    main.cpp
    ListaDeCarga.cpp
    RotorDeMapeo.cpp
    Tramas.cpp
    SerialReader.cpp
)

# Archivos de cabecera
set(HEADERS
    TramaBase.h
    ListaDeCarga.h
    RotorDeMapeo.h
    Tramas.h
    SerialReader.h
)

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Incluir directorio actual para los headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Bibliotecas necesarias en Linux
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

# Opciones de compilación con warnings
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall 
    -Wextra 
    -pedantic
    -Werror=return-type
)

# Modo de depuración con símbolos
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
endif()

# Instalación
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Script de ayuda para permisos del puerto serial
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/setup_serial.sh
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
    OPTIONAL
)

# Documentación con Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    
    doxygen_add_docs(
        documentation
        ${SOURCES} ${HEADERS}
        COMMENT "Generando documentación con Doxygen"
    )
endif()

# Mensaje de información
message(STATUS "========================================")
message(STATUS "Configuración del proyecto:")
message(STATUS "  - Nombre: ${PROJECT_NAME}")
message(STATUS "  - Versión: ${PROJECT_VERSION}")
message(STATUS "  - Autor: Eliezer Mores Oyervides")
message(STATUS "  - C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  - Sistema: Linux")
if(DOXYGEN_FOUND)
    message(STATUS "  - Doxygen: Disponible")
    message(STATUS "    (use 'make documentation')")
else()
    message(STATUS "  - Doxygen: No encontrado")
endif()
message(STATUS "========================================")

# Target personalizado para compilar y ejecutar
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Compilando y ejecutando ${PROJECT_NAME}..."
)