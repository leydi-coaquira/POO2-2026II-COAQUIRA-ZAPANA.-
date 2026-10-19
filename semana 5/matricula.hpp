//matricula.hpp — clase Matricula con DbC completo 
#pragma once
#include "errores_academicos.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <chrono>

class Matricula {
    /*
     * INVARIANTE de clase:
     *   !estudiante_codigo_.empty()
     *   1 <= cursos_.size() <= MAX_CURSOS
     *   No hay duplicados en cursos_
     *   anulada_ == true  =>  cursos_ no se modifica
     */
    static constexpr int MAX_CURSOS = 6;

    std::string              estudiante_codigo_;
    std::vector<std::string> cursos_;
    bool                     anulada_ = false;

    void verificarInvariante() const {
        assert(!estudiante_codigo_.empty()        && "INV: código vacío");
        assert(cursos_.size() >= 1                && "INV: sin cursos");
        assert((int)cursos_.size() <= MAX_CURSOS  && "INV: excede máximo");
        for (int i = 0; i < (int)cursos_.size(); ++i)
            for (int j = i+1; j < (int)cursos_.size(); ++j)
                assert(cursos_[i] != cursos_[j] && "INV: cursos duplicados");
    }

    // Auxiliar para revisar duplicados en datos de entrada (constructor)
    static bool tieneDuplicados(const std::vector<std::string>& v) {
        for (size_t i = 0; i < v.size(); ++i)
            for (size_t j = i + 1; j < v.size(); ++j)
                if (v[i] == v[j]) return true;
        return false;
    }

public:
    Matricula(std::string codigoEst, std::vector<std::string> cursos) {
        // ── PRECONDICIONES 
        if (codigoEst.empty())
            throw ErrorValidacion("Código de estudiante vacío");
        if (cursos.empty() || (int)cursos.size() > MAX_CURSOS)
            throw ErrorCapacidad((int)cursos.size(), MAX_CURSOS, "matricula");
        if (tieneDuplicados(cursos))                       // TODO completado
            throw ErrorValidacion("Cursos duplicados en la matrícula");

        estudiante_codigo_ = std::move(codigoEst);
        cursos_            = std::move(cursos);
        verificarInvariante();  // POST-constructor
    }

    void agregarCurso(const std::string& curso) {
        // PRECONDICIÓN: no anulada < MAX_CURSOS no hay duplicado
        if (anulada_)
            throw ErrorAcademico("Matrícula anulada — no se puede modificar");
        if ((int)cursos_.size() >= MAX_CURSOS)
            throw ErrorCapacidad((int)cursos_.size(), MAX_CURSOS, "matricula");
        if (std::find(cursos_.begin(), cursos_.end(), curso) != cursos_.end())
            throw ErrorValidacion("Curso '" + curso + "' ya está en la matrícula");

        cursos_.push_back(curso);

        // POSCONDICIÓN
        assert(std::find(cursos_.begin(), cursos_.end(), curso) != cursos_.end()
               && "POST: curso no agregado");
        assert((int)cursos_.size() <= MAX_CURSOS && "POST: excede máximo");
        verificarInvariante();
    }

    // NUEVO: pedido en la descripción ("puede agregarse o eliminarse un
    // curso"), no venía en la plantilla.
    void eliminarCurso(const std::string& curso) {
        // PRECONDICIÓN: no anulada, el curso debe existir, y no puede
        // dejar la matrícula con 0 cursos (invariante exige al menos 1)
        if (anulada_)
            throw ErrorAcademico("Matrícula anulada — no se puede modificar");
        auto it = std::find(cursos_.begin(), cursos_.end(), curso);
        if (it == cursos_.end())
            throw ErrorValidacion("Curso '" + curso + "' no está en la matrícula");
        if (cursos_.size() <= 1)
            throw ErrorAcademico("No se puede eliminar: la matrícula requiere al menos 1 curso");

        cursos_.erase(it);

        // POSCONDICIÓN
        assert(std::find(cursos_.begin(), cursos_.end(), curso) == cursos_.end()
               && "POST: curso no eliminado");
        assert(cursos_.size() >= 1 && "POST: queda sin cursos");
        verificarInvariante();
    }

    void anular() {
        if (anulada_)
            throw ErrorAcademico("Matrícula ya está anulada");
        anulada_ = true;
        // No se llama verificarInvariante — anular no rompe la invariante
    }

    std::string diagnostico() const {
        // Nota: se envuelve el primer literal en std::string(...) porque
        // "texto" + "texto" (dos const char*) no compila en C++.
        return std::string("Matrícula [") + (anulada_ ? "ANULADA" : "ACTIVA") + "] — "
               + std::to_string(cursos_.size()) + "/" + std::to_string(MAX_CURSOS)
               + " cursos";
    }

    // Getters
    bool        anulada()   const noexcept { return anulada_;            }
    int         numCursos() const noexcept { return (int)cursos_.size(); }
    const auto& cursos()    const noexcept { return cursos_;             }
};