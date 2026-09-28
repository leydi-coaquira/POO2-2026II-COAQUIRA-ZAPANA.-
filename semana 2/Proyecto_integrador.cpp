#include "Proyecto_integrador.hpp"
#include <ctime>

int main() {
    Libro libro("BIB001", "Los rios profundos", "Jose Maria Arguedas", "general");
    libro.imprimir();
    std::cout << "Dias de prestamo: " << libro.diasPrestamo() << "\n";

    Lector lector("LEC001", "Rosa Mamani", "comunidad");
    lector.imprimir();

    Prestamo prestamo(libro, lector, std::time(nullptr));
    std::cout << "Fecha devolucion esperada (epoch): " << prestamo.fechaDevolucionEsperada() << "\n";
    libro.imprimir();
    lector.imprimir();

    try {
        Libro bad("X9", "titulo", "autor", "general");
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    try {
        Libro bad("BIB002", "titulo", "autor", "rara");
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    try {
        Lector bad("X9", "nombre", "comunidad");
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    try {
        Prestamo otro(libro, lector, std::time(nullptr)); // libro ya prestado
    } catch (const std::runtime_error& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }

    return 0;
}