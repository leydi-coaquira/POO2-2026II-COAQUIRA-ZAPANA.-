// main.cpp — Práctica S1, Actividad 3
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

struct Nota {
    std::string curso;
    float calificacion;
};

class Estudiante {
    std::string nombre_;
    int codigo_;
    std::string escuela_;
    std::vector<Nota> notas_;

public:
    Estudiante(std::string nombre, int codigo, std::string escuela)
        : nombre_(std::move(nombre)), codigo_(codigo), escuela_(std::move(escuela)) {}

    const std::string& getNombre() const { return nombre_; }
    int getCodigo() const { return codigo_; }

    void agregarNota(const std::string& curso, float cal) {
        if (cal < 0.0f || cal > 20.0f) {
            throw std::invalid_argument(
                "La calificacion debe estar entre 0 y 20 (recibido: " +
                std::to_string(cal) + ")");
        }
        notas_.push_back({curso, cal});
    }

    float calcularPPA() const {
        if (notas_.empty()) return 0.0f;
        float suma = 0.0f;
        for (const auto& n : notas_) suma += n.calificacion;
        return suma / notas_.size();
    }

    void imprimir() const {
        std::cout << "[" << codigo_ << "] " << nombre_
                  << " (" << escuela_ << ")"
                  << " PPA: " << calcularPPA() << '\n';
    }
};

int main() {
    Estudiante e("Juan Carlos Mamani Quispe", 20210500, "Ing. Sistemas");
    e.agregarNota("POO II", 16);
    e.agregarNota("Base de Datos I", 14);
    e.imprimir();

    try {
        e.agregarNota("Redes", 25); // debe lanzar
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error capturado: " << ex.what() << '\n';
    }
    return 0;
}