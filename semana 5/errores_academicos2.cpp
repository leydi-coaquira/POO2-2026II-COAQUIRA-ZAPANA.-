// ── contrato_curso.hpp — Diseño por Contrato en C++17 ──────────────────
#include "errores_academicos.hpp"
#include <string>
#include <vector>
#include <cassert>    // assert() — para poscondiciones e invariantes
 
class Curso {
    /*
     * INVARIANTE de clase:
     *   0 <= inscritos_ <= cupos_max_
     *   creditos_ in [1, 6]
     *   !codigo_.empty()
     *
     * Cada mutación termina llamando a verificarInvariante().
     */
    std::string codigo_;
    int         creditos_;
    int         cupos_max_;
    int         inscritos_ = 0;
 
    // ── Verificador central de invariante ───────────────────────────────
    void verificarInvariante() const {
        assert(!codigo_.empty()                    && "INV: código vacío");
        assert(creditos_ >= 1 && creditos_ <= 6   && "INV: créditos fuera [1,6]");
        assert(inscritos_ >= 0                     && "INV: inscritos negativo");
        assert(inscritos_ <= cupos_max_            && "INV: inscritos > cupos");
    }
 
public:
    // CONSTRUCTOR — precondiciones explícitas + verificación de invariante
    Curso(std::string codigo, int creditos, int cuposMax)
        : codigo_(std::move(codigo)),
          creditos_(creditos),
          cupos_max_(cuposMax) {
        // PRECONDICIÓN: el llamador garantiza estos valores
        if (codigo_.empty())
            throw ErrorValidacion("Código del curso no puede ser vacío");
        if (creditos < 1 || creditos > 6)
            throw ErrorValidacion("Créditos deben estar en [1, 6]");
        if (cuposMax < 1)
            throw ErrorValidacion("cuposMax debe ser >= 1");
        // POST: el objeto fue construido en estado válido
        verificarInvariante();
    }
 
    // MÉTODO MUTADOR — pre + pos + invariante
    void inscribir() {
        // PRECONDICIÓN
        if (inscritos_ >= cupos_max_)
            throw ErrorCapacidad(inscritos_, cupos_max_, codigo_);
 
        inscritos_++;
 
        // POSCONDICIÓN (assert porque es un bug interno si falla)
        assert(inscritos_ <= cupos_max_ && "POST: invariante rota tras inscribir");
        // INVARIANTE
        verificarInvariante();
    }
 
    // GETTER — noexcept porque solo lee, no puede lanzar
    int inscritos()  const noexcept { return inscritos_;  }
    int cuposMax()   const noexcept { return cupos_max_;  }
    int cuposLibres()const noexcept { return cupos_max_ - inscritos_; }
};
#include <iostream>

int main() {
    std::cout << "=== Prueba de Diseño por Contrato: Curso ===\n";

    Curso c("SIS206", 4, 2);   // construcción válida (invariante se cumple)
    std::cout << "Curso creado. Cupos: " << c.cuposMax() << "\n";

    c.inscribir();
    c.inscribir();
    std::cout << "Inscritos: " << c.inscritos()
              << " | Libres: " << c.cuposLibres() << "\n";

    // PRECONDICIÓN violada por el llamador -> throw, no assert
    std::cout << "\nIntentando inscribir un tercero (cupos llenos):\n";
    try {
        c.inscribir();
    } catch (const ErrorCapacidad& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    // Construcción inválida -> también lanza, no revienta con assert
    std::cout << "\nIntentando crear un curso con código vacío:\n";
    try {
        Curso invalido("", 3, 10);
    } catch (const ErrorValidacion& e) {
        std::cerr << "Error esperado: " << e.what() << "\n";
    }

    return 0;
}