// errores_dominio.hpp
// Practica S5, Actividad 5 - Jerarquia completa de excepciones del
// dominio de la Biblioteca Digital - Huata (Proyecto Integrador).
#ifndef ERRORES_DOMINIO_HPP
#define ERRORES_DOMINIO_HPP

#include <stdexcept>
#include <string>

class ErrorBiblioteca : public std::runtime_error {
public:
    explicit ErrorBiblioteca(const std::string& mensaje)
        : std::runtime_error("[Biblioteca] " + mensaje) {}
};

class ErrorValidacion : public ErrorBiblioteca {
public:
    explicit ErrorValidacion(const std::string& mensaje)
        : ErrorBiblioteca("[Validacion] " + mensaje) {}
};

class ErrorEstado : public ErrorBiblioteca {
public:
    explicit ErrorEstado(const std::string& mensaje)
        : ErrorBiblioteca("[Estado] " + mensaje) {}
};

class ErrorCapacidad : public ErrorBiblioteca {
public:
    ErrorCapacidad(int actual, int maximo, const std::string& entidad)
        : ErrorBiblioteca(entidad + ": capacidad agotada (" +
                          std::to_string(actual) + "/" +
                          std::to_string(maximo) + ")") {}
};

// Busqueda fallida. Se deja disponible para los casos que NO se hayan
// refactorizado a Result<T>; buscarEnCatalogo() ya NO lanza esto.
class ErrorNoEncontrado : public ErrorBiblioteca {
public:
    explicit ErrorNoEncontrado(const std::string& mensaje)
        : ErrorBiblioteca("[NoEncontrado] " + mensaje) {}
};

#endif // ERRORES_DOMINIO_HPP