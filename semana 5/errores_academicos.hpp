// ── errores_academicos.hpp — jerarquía de dominio ──────────────────────
#pragma once
#include <stdexcept>    // std::runtime_error, std::invalid_argument
#include <string>
#include <sstream>
 
// ── Raíz de dominio: hereda de runtime_error ───────────────────────────
class ErrorAcademico : public std::runtime_error {
public:
    // Constructor que recibe el mensaje y lo reenvía a runtime_error
    explicit ErrorAcademico(const std::string& msg)
        : std::runtime_error("[Académico] " + msg) {}
};
 
// ── Primer nivel de especialización ────────────────────────────────────
class ErrorValidacion : public ErrorAcademico {
public:
    explicit ErrorValidacion(const std::string& msg)
        : ErrorAcademico("[Validación] " + msg) {}
};
 
// ── Segundo nivel: error con valor concreto ─────────────────────────────
class ErrorCalificacion : public ErrorValidacion {
    float valor_;          // dato contextual del error
public:
    explicit ErrorCalificacion(float v)
        : ErrorValidacion("Calificación " + std::to_string(v)
                         + " fuera de rango [0, 20]"),
          valor_(v) {}
 
    float valor() const noexcept { return valor_; }
    //                    ↑ noexcept = poscondición: nunca lanza
};
 
class ErrorCapacidad : public ErrorAcademico {
    int actual_, maximo_;
public:
    ErrorCapacidad(int actual, int maximo, const std::string& entidad)
        : ErrorAcademico(entidad + ": capacidad agotada ("
                        + std::to_string(actual) + "/"
                        + std::to_string(maximo) + ")"),
          actual_(actual), maximo_(maximo) {}
 
    int actual() const noexcept { return actual_; }
    int maximo() const noexcept { return maximo_; }
};
 
class ErrorRecursoNoEncontrado : public ErrorAcademico {
    std::string tipo_;
    int         id_;
public:
    ErrorRecursoNoEncontrado(std::string tipo, int id)
        : ErrorAcademico(tipo + " id=" + std::to_string(id)
                        + " no encontrado"),
          tipo_(std::move(tipo)), id_(id) {}
 
    const std::string& tipo() const noexcept { return tipo_; }
    int id()                 const noexcept { return id_;   }
};
