// ── main_demo.cpp — captura selectiva ──────────────────────────────────
#include "errores_academicos.hpp"
#include <iostream>
 
void registrar_nota(float cal) {
    if (cal < 0.0f || cal > 20.0f)
        throw ErrorCalificacion(cal);   // lanza excepción de dominio
}
 
int main() {
    try {
        registrar_nota(25.0f);
    }
    catch (const ErrorCalificacion& e) {      // 1.° más específico
        std::cerr << "Nota inválida: " << e.valor() << "\n";
        std::cerr << e.what() << "\n";
    }
    catch (const ErrorValidacion& e) {        // 2.° rama validación
        std::cerr << "Validación: " << e.what() << "\n";
    }
    catch (const ErrorAcademico& e) {         // 3.° cualquier dominio
        std::cerr << "Dominio: " << e.what() << "\n";
    }
    catch (const std::exception& e) {         // 4.° cualquier std
        std::cerr << "Sistema: " << e.what() << "\n";
    }
    // No catch (...) — dejar crashear con stack-trace es mejor que silencio
    return 0;
}
  