#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "resultado.hpp"

struct Estudiante {
    int id;
    std::string nombre;
    std::vector<double> notas;
    double promedio;
    std::string categoria;

    Estudiante() : id(0), nombre(""), promedio(0.0), categoria("") {}
};

inline std::vector<Estudiante> obtenerEstudiantes() {
    std::vector<Estudiante> lista;

    Estudiante e1;
    e1.id = 1;
    e1.nombre = "Yesenia Zapana";
    e1.notas.push_back(18.0);
    e1.notas.push_back(16.5);
    e1.notas.push_back(19.0);
    lista.push_back(e1);

    Estudiante e2;
    e2.id = 2;
    e2.nombre = "Carlos Rojas";
    e2.notas.push_back(12.0);
    e2.notas.push_back(10.5);
    e2.notas.push_back(11.0);
    lista.push_back(e2);

    Estudiante e3;
    e3.id = 3;
    e3.nombre = "Maria Flores";
    e3.notas.push_back(14.0);
    e3.notas.push_back(15.0);
    e3.notas.push_back(13.5);
    lista.push_back(e3);

    return lista;
}

inline Result<Estudiante, std::string> buscarEstudiante(int id) {
    std::vector<Estudiante> estudiantes = obtenerEstudiantes();

    for (std::size_t i = 0; i < estudiantes.size(); ++i) {
        if (estudiantes[i].id == id) {
            return Result<Estudiante, std::string>::Ok(estudiantes[i]);
        }
    }

    std::ostringstream oss;
    oss << "No se encontro ningun estudiante con id " << id;
    return Result<Estudiante, std::string>::Err(oss.str());
}

inline Estudiante calcularPromedio(Estudiante est) {
    if (est.notas.empty()) {
        est.promedio = 0.0;
        return est;
    }

    double suma = 0.0;
    for (std::size_t i = 0; i < est.notas.size(); ++i) {
        suma += est.notas[i];
    }
    est.promedio = suma / static_cast<double>(est.notas.size());
    return est;
}

inline Estudiante asignarCategoria(Estudiante est) {
    if (est.promedio >= 17.0) {
        est.categoria = "Excelente";
    } else if (est.promedio >= 14.0) {
        est.categoria = "Bueno";
    } else if (est.promedio >= 11.0) {
        est.categoria = "Regular";
    } else {
        est.categoria = "Deficiente";
    }
    return est;
}

inline std::string formatearResultado(Estudiante est) {
    std::ostringstream oss;
    oss << "Estudiante: " << est.nombre
        << " | Promedio: " << est.promedio
        << " | Categoria: " << est.categoria;
    return oss.str();
}

inline Result<std::string, std::string> procesarEstudiante(int id) {
    return buscarEstudiante(id)
        .map(calcularPromedio)
        .map(asignarCategoria)
        .map(formatearResultado);
}

#endif // PIPELINE_HPP

void mostrarResultado(int id) {
    Result<std::string, std::string> r = procesarEstudiante(id);

    if (r.esExito()) {
        std::cout << "[OK] " << r.valor() << std::endl;
    } else {
        std::cout << "[ERROR] " << r.error() << std::endl;
    }
}

int main() {
    std::cout << "=== Pipeline de procesamiento de estudiantes ===" << std::endl;

    mostrarResultado(1);
    mostrarResultado(2);
    mostrarResultado(3);

    mostrarResultado(99);

    return 0;
}