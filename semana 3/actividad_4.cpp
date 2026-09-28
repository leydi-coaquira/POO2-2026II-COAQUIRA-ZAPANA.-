// ── main.cpp - Práctica S3, Actividad 4 ─────────────────────────────────
// Compilar:  g++ -std=c++17 -Wall -o planilla main.cpp
// Ejecutar:  ./planilla      (en Windows: planilla.exe)
#include "personal_unap.hpp"

int main() {
    // Vector heterogéneo: punteros a la clase base, objetos de subclases
    std::vector<std::unique_ptr<PersonalUniversitario>> personal;
    personal.push_back(std::make_unique<Docente>(
        "12345678", "Juan Carlos", "Mamani Quispe", 16, "Asociado"));
    personal.push_back(std::make_unique<Docente>(
        "23456789", "Maria Elena", "Quispe Torres", 12, "Principal"));
    personal.push_back(std::make_unique<Administrativo>(
        "34567890", "Pedro", "Huanca Lazo", 2800.0f, 350.0f));
    personal.push_back(std::make_unique<Investigador>(
        "45678901", "Ana Lucia", "Flores Ramos", 2));

    planilla(personal);

    // Pruebas de validación: datos inválidos deben ser rechazados
    std::cout << "\nPruebas de validacion:\n";
    try {
        Docente d("11111111", "Test", "Horas", 50, "Asociado");
    } catch (const std::invalid_argument& e) {
        std::cout << "  [OK] Dato invalido rechazado: " << e.what() << "\n";
    }
    try {
        Docente d("11111111", "Test", "Categoria", 10, "Titular");
    } catch (const std::invalid_argument& e) {
        std::cout << "  [OK] Dato invalido rechazado: " << e.what() << "\n";
    }
    try {
        Administrativo a("22222222", "Test", "Salario", -500.0f);
    } catch (const std::invalid_argument& e) {
        std::cout << "  [OK] Dato invalido rechazado: " << e.what() << "\n";
    }

    return 0;   // unique_ptr libera la memoria; el destructor virtual
                // garantiza que se destruya correctamente cada subclase
}