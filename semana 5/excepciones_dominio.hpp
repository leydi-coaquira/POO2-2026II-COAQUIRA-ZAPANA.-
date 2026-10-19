// excepciones_dominio.hpp — Práctica S5, Actividad 1 (C++17)
// Jerarquía de excepciones del dominio académico + ErrorMatriculaDuplicada
#pragma once
#include <stdexcept>
#include <string>
#include <vector>

// ── Raíz de dominio: hereda de runtime_error 
class ErrorAcademico : public std::runtime_error {
public:
    explicit ErrorAcademico(const std::string& msg)
        : std::runtime_error("[Académico] " + msg) {}
};

// ── Primer nivel de especialización 
class ErrorValidacion : public ErrorAcademico {
public:
    explicit ErrorValidacion(const std::string& msg)
        : ErrorAcademico("[Validación] " + msg) {}
};

// ── Segundo nivel: error con valor concreto 
class ErrorCalificacion : public ErrorValidacion {
    float valor_;
public:
    explicit ErrorCalificacion(float v)
        : ErrorValidacion("Calificación " + std::to_string(v)
                          + " fuera de rango [0, 20]"),
          valor_(v) {}

    float valor() const noexcept { return valor_; }
};

// Actividad 1 matrícula duplicada 
// Hereda de ErrorValidacion 
class ErrorMatriculaDuplicada : public ErrorValidacion {
    std::string codigo_estudiante_;
public:
    explicit ErrorMatriculaDuplicada(const std::string& codigoEstudiante)
        : ErrorValidacion("El estudiante " + codigoEstudiante
                          + " ya está matriculado"),
          codigo_estudiante_(codigoEstudiante) {}

    const std::string& codigoEstudiante() const noexcept {
        return codigo_estudiante_;
    }
};

// ── Función de validación (firma pedida por la guía) 
inline void validar_matricula(int codigo, std::vector<int>& matriculados) {
    for (int c : matriculados) {
        if (c == codigo) {
            throw ErrorMatriculaDuplicada(std::to_string(codigo));
        }
    }
    matriculados.push_back(codigo);
}