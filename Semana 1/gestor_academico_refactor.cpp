// gestor_academico_refactor.cpp
// Diseno refactorizado del Gestor Academico (equivalente al de Python)

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

const double NOTA_MINIMA = 0.0;
const double NOTA_MAXIMA = 20.0;
const double PROMEDIO_APROBACION = 10.5;

class Nota {
public:
    explicit Nota(double valor) : valor_(valor) {
        if (valor_ < NOTA_MINIMA || valor_ > NOTA_MAXIMA) {
            throw std::invalid_argument("La nota esta fuera de rango [0, 20]");
        }
    }
    double valor() const { return valor_; }
private:
    double valor_;
};

class Estudiante {
public:
    Estudiante(std::string codigo, std::string nombre)
        : codigo_(std::move(codigo)), nombre_(std::move(nombre)) {}

    const std::string& codigo() const { return codigo_; }
    const std::string& nombre() const { return nombre_; }

    void registrarNota(double valor) {
        notas_.push_back(Nota(valor));
    }

    double promedio() const {
        if (notas_.empty()) return 0.0;
        double suma = 0.0;
        for (const auto& n : notas_) suma += n.valor();
        return suma / notas_.size();
    }

    bool estaAprobado() const {
        return promedio() >= PROMEDIO_APROBACION;
    }

private:
    std::string codigo_;
    std::string nombre_;
    std::vector<Nota> notas_;
};

class RepositorioEstudiantes {
public:
    void registrar(const Estudiante& estudiante) {
        for (const auto& e : estudiantes_) {
            if (e.codigo() == estudiante.codigo()) {
                throw std::invalid_argument("Ya existe un estudiante con ese codigo");
            }
        }
        estudiantes_.push_back(estudiante);
    }

    Estudiante& obtener(const std::string& codigo) {
        for (auto& e : estudiantes_) {
            if (e.codigo() == codigo) return e;
        }
        throw std::invalid_argument("Estudiante no encontrado");
    }

    std::vector<Estudiante>& listar() { return estudiantes_; }

private:
    std::vector<Estudiante> estudiantes_;
};

class GeneradorReporte {
public:
    static std::string estado(const Estudiante& e) {
        return e.estaAprobado() ? "APROBADO" : "DESAPROBADO";
    }

    static std::string generarTexto(std::vector<Estudiante>& estudiantes) {
        std::ostringstream out;
        for (auto& e : estudiantes) {
            out << e.codigo() << " - " << e.nombre()
                << ": promedio=" << std::fixed << std::setprecision(2) << e.promedio()
                << " (" << estado(e) << ")\n";
        }
        return out.str();
    }

    static std::string generarCsv(std::vector<Estudiante>& estudiantes) {
        std::ostringstream out;
        out << "codigo,nombre,promedio,estado\n";
        for (auto& e : estudiantes) {
            out << e.codigo() << "," << e.nombre() << ","
                << std::fixed << std::setprecision(2) << e.promedio()
                << "," << estado(e) << "\n";
        }
        return out.str();
    }
};

int main() {
    RepositorioEstudiantes repo;
    Estudiante e("E001", "Ana Quispe");
    repo.registrar(e);
    repo.obtener("E001").registrarNota(12);

    std::cout << GeneradorReporte::generarTexto(repo.listar());
    return 0;
}