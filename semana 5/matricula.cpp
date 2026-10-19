#include "matricula.hpp"
#include <iostream>

int main() {
    std::cout << "=== Construcción válida ===\n";
    Matricula m("20210500", {"SIS206", "SIS207", "SIS208"});
    std::cout << m.diagnostico() << "\n";

    std::cout << "\n=== agregarCurso ===\n";
    m.agregarCurso("SIS209");
    std::cout << m.diagnostico() << "\n";

    std::cout << "\n=== agregarCurso duplicado (debe lanzar) ===\n";
    try {
        m.agregarCurso("SIS209");
    } catch (const ErrorValidacion& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    std::cout << "\n=== eliminarCurso ===\n";
    m.eliminarCurso("SIS209");
    std::cout << m.diagnostico() << "\n";

    std::cout << "\n=== eliminarCurso dejando 1 solo curso, luego intentar vaciar (debe lanzar) ===\n";
    Matricula m2("20210777", {"SIS210", "SIS211"});
    m2.eliminarCurso("SIS210");
    std::cout << m2.diagnostico() << "\n";
    try {
        m2.eliminarCurso("SIS211");   // dejaría la matrícula en 0 cursos
    } catch (const ErrorAcademico& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    std::cout << "\n=== anular y luego intentar modificar (debe lanzar) ===\n";
    m.anular();
    std::cout << m.diagnostico() << "\n";
    try {
        m.agregarCurso("SIS210");
    } catch (const ErrorAcademico& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    std::cout << "\n=== Construcción inválida: más de 6 cursos (debe lanzar) ===\n";
    try {
        Matricula invalida("20210888", {"A","B","C","D","E","F","G"});
    } catch (const ErrorCapacidad& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    std::cout << "\n=== Construcción inválida: cursos duplicados (debe lanzar) ===\n";
    try {
        Matricula invalida2("20210999", {"SIS206", "SIS206"});
    } catch (const ErrorValidacion& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    return 0;
}