// ── tests_dominio.cpp ───────────────────────────────────────────────────
// Practica S5, Actividad 5 - 6 pruebas de contratos sobre el Proyecto
// Integrador (Biblioteca Digital Huata): 3 validas, 3 que deben lanzar.
#include "Material_bibliografico.hpp"
#include "dominio_errores.hpp"
#include <iostream>
#include <cassert>

// ── Validas (no deben lanzar) 
void test_valida_1_libro_fisico_ok() {
    LibroFisico libro("BIB001", "Los rios profundos", "Jose Maria Arguedas", "Estante A-3");
    assert(libro.estaDisponible());
    assert(libro.diasPrestamo() == 14);
    std::cout << "[PASS] valida 1: LibroFisico valido\n";
}

void test_valida_2_prestamo_ok() {
    LibroDigital libro("BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2);
    Lector lector("LEC001", "Rosa Mamani", "comunidad");
    Prestamo p(libro, lector);
    assert(!libro.estaDisponible());
    std::cout << "[PASS] valida 2: Prestamo ok\n";
}

void test_valida_3_buscar_en_catalogo_result_ok() {
    vector<unique_ptr<MaterialBibliografico>> catalogo;
    catalogo.push_back(make_unique<LibroFisico>(
        "BIB003", "Cuentos andinos", "Jose Maria Arguedas", "Estante B-1"));

    auto encontrado = buscarEnCatalogo(catalogo, "BIB003");
    assert(encontrado.esExito());

    auto noEncontrado = buscarEnCatalogo(catalogo, "BIB999");
    assert(noEncontrado.esError());

    std::cout << "[PASS] valida 3: buscarEnCatalogo() con Result (encontrado y no encontrado)\n";
}

// ── Deben lanzar ────────────────────────────────────────────────────────
void test_lanza_1_codigo_invalido() {
    try {
        LibroFisico libro("X9", "titulo", "autor", "Estante A-1");
        assert(false && "Debio lanzar ErrorValidacion");
    } catch (const ErrorValidacion& e) {
        std::cout << "[PASS] lanza 1: " << e.what() << "\n";
    }
}

void test_lanza_2_prestamo_duplicado() {
    LibroFisico libro("BIB004", "Todas las sangres", "Jose Maria Arguedas", "Estante A-2");
    Lector lector("LEC002", "Juan Quispe", "estudiante");
    Prestamo p1(libro, lector);
    try {
        Prestamo p2(libro, lector); // el material ya esta prestado
        assert(false && "Debio lanzar ErrorEstado");
    } catch (const ErrorEstado& e) {
        std::cout << "[PASS] lanza 2: " << e.what() << "\n";
    }
}

void test_lanza_3_lector_tipo_invalido() {
    try {
        Lector lector("LEC003", "Carlos Rojas", "vecino"); // tipo no valido
        assert(false && "Debio lanzar ErrorValidacion");
    } catch (const ErrorValidacion& e) {
        std::cout << "[PASS] lanza 3: " << e.what() << "\n";
    }
}

int main() {
    test_valida_1_libro_fisico_ok();
    test_valida_2_prestamo_ok();
    test_valida_3_buscar_en_catalogo_result_ok();

    test_lanza_1_codigo_invalido();
    test_lanza_2_prestamo_duplicado();
    test_lanza_3_lector_tipo_invalido();

    std::cout << "\nTodas las pruebas de dominio pasaron.\n";
    return 0;
}