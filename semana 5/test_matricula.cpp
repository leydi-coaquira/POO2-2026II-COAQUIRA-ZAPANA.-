// ── tests_matricula.cpp ─
#include "matricula.hpp"
#include <iostream>
#include <cassert>

void test_constructor_valido() {
    Matricula m("20210500", {"SIS206", "SIS207", "SIS208"});
    assert(m.numCursos() == 3);
    assert(!m.anulada());
    std::cout << "[PASS] constructor válido\n";
}

void test_constructor_sin_cursos() {
    try {
        Matricula m("20210500", {});    // debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorCapacidad& e) {
        std::cout << "[PASS] constructor sin cursos: " << e.what() << "\n";
    }
}

void test_agregar_curso_duplicado() {
    Matricula m("20210500", {"SIS206"});
    try {
        m.agregarCurso("SIS206");       // duplicado — debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorValidacion& e) {
        std::cout << "[PASS] duplicado: " << e.what() << "\n";
    }
}

void test_anular_y_modificar() {
    Matricula m("20210500", {"SIS206"});
    m.anular();
    try {
        m.agregarCurso("SIS207");       // anulada — debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorAcademico& e) {
        std::cout << "[PASS] anulada: " << e.what() << "\n";
    }
}

int main() {
    test_constructor_valido();
    test_constructor_sin_cursos();
    test_agregar_curso_duplicado();
    test_anular_y_modificar();
    std::cout << "\nTodas las pruebas pasaron.\n";
    return 0;
}