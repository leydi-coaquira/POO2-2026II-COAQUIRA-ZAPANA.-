// ── main.cpp — Práctica S5, Actividad 1 (C++17) ──────────────────────────
// Ejecutar: ./act1
#include "excepciones_dominio.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<int> matriculados = {1001, 1002, 1003};

    std::cout << "=== Caso 1: matrícula nueva (no lanza) ===\n";
    try {
        validar_matricula(1004, matriculados);
        std::cout << "[OK] Código 1004 matriculado. Total: "
                  << matriculados.size() << "\n";
    } catch (const ErrorAcademico& e) {
        std::cerr << "No debería llegar aquí: " << e.what() << "\n";
    }

    std::cout << "\n=== Caso 2: matrícula duplicada ===\n";
    // Captura selectiva: SIEMPRE del tipo más específico al más general,
    // porque C++ usa la primera coincidencia (no la "mejor").
    try {
        validar_matricula(1001, matriculados);   // 1001 ya está en el vector
    }
    catch (const ErrorMatriculaDuplicada& e) {        // 1.° más específico
        std::cerr << "Duplicado: " << e.what() << "\n";
        std::cerr << "  codigoEstudiante() = " << e.codigoEstudiante() << "\n";
    }
    catch (const ErrorValidacion& e) {                // 2.° rama validación
        std::cerr << "Validación: " << e.what() << "\n";
    }
    catch (const ErrorAcademico& e) {                 // 3.° cualquier dominio
        std::cerr << "Dominio: " << e.what() << "\n";
    }
    catch (const std::exception& e) {                 // 4.° cualquier std
        std::cerr << "Sistema: " << e.what() << "\n";
    }

    std::cout << "\n=== Caso 3: la misma captura también atrapa "
                 "ErrorCalificacion (es subclase de ErrorValidacion) ===\n";
    try {
        throw ErrorCalificacion(25.0f);
    }
    catch (const ErrorMatriculaDuplicada& e) {
        std::cerr << "Duplicado: " << e.what() << "\n";
    }
    catch (const ErrorValidacion& e) {                // atrapa aquí
        std::cerr << "Validación (genérica): " << e.what() << "\n";
    }
    catch (const ErrorAcademico& e) {
        std::cerr << "Dominio: " << e.what() << "\n";
    }

    std::cout << "\nTotal final de matriculados: " << matriculados.size() << "\n";
    return 0;
}