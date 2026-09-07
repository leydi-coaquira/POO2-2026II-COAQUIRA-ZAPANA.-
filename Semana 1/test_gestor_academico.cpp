// test_gestor_academico.cpp
// Pruebas de caracterizacion en C++ (equivalente a las de pytest).
// No usa ninguna libreria externa: se compila igual que los demas .cpp.

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cmath>

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

    std::size_t cantidadNotas() const { return notas_.size(); }

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

// ---------------- Pruebas ----------------

int total = 0;
int pasadas = 0;

void verificar(bool condicion, const std::string& nombre) {
    total++;
    if (condicion) {
        std::cout << "[PASSED] " << nombre << "\n";
        pasadas++;
    } else {
        std::cout << "[FAILED] " << nombre << "\n";
    }
}

void test_registrar_y_recuperar_estudiante_por_codigo() {
    RepositorioEstudiantes repo;
    repo.registrar(Estudiante("E001", "Ana Quispe"));
    Estudiante& e = repo.obtener("E001");
    verificar(e.codigo() == "E001" && e.nombre() == "Ana Quispe",
              "registrar_y_recuperar_estudiante_por_codigo");
}

void test_agregar_notas_en_limites_validos_0_y_20() {
    Estudiante e("E001", "Ana Quispe");
    e.registrarNota(0);
    e.registrarNota(20);
    verificar(e.cantidadNotas() == 2, "agregar_notas_en_limites_validos_0_y_20");
}

void test_calcular_promedio_con_tres_notas_conocidas() {
    Estudiante e("E001", "Ana Quispe");
    e.registrarNota(12);
    e.registrarNota(15);
    e.registrarNota(10);
    double esperado = (12.0 + 15.0 + 10.0) / 3.0;
    verificar(std::abs(e.promedio() - esperado) < 0.0001,
              "calcular_promedio_con_tres_notas_conocidas");
}

void test_promedio_es_cero_sin_notas() {
    Estudiante e("E003", "Rosa Huanca");
    verificar(e.promedio() == 0.0, "promedio_es_cero_sin_notas");
}

void test_aprobacion_en_el_limite_10_5() {
    Estudiante e("E001", "Ana");
    e.registrarNota(10);
    e.registrarNota(11);
    verificar(e.promedio() == 10.5 && e.estaAprobado(), "aprobacion_en_el_limite_10_5");
}

void test_no_aprueba_justo_debajo_del_limite() {
    Estudiante e("E001", "Ana Quispe");
    e.registrarNota(10.49);
    verificar(!e.estaAprobado(), "no_aprueba_justo_debajo_del_limite");
}

void test_rechaza_nota_fuera_de_rango_bajo() {
    Estudiante e("E002", "Luis Mamani");
    bool lanzo = false;
    try {
        e.registrarNota(-0.1);
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    verificar(lanzo, "rechaza_nota_fuera_de_rango_bajo");
}

void test_rechaza_nota_fuera_de_rango_alto() {
    Estudiante e("E002", "Luis Mamani");
    bool lanzo = false;
    try {
        e.registrarNota(21);
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    verificar(lanzo, "rechaza_nota_fuera_de_rango_alto");
}

void test_rechaza_codigo_de_estudiante_duplicado() {
    RepositorioEstudiantes repo;
    repo.registrar(Estudiante("E001", "Ana Quispe"));
    bool lanzo = false;
    try {
        repo.registrar(Estudiante("E001", "Ana Quispe (duplicado)"));
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    verificar(lanzo, "rechaza_codigo_de_estudiante_duplicado");
}

void test_generar_reporte_no_altera_la_entidad() {
    RepositorioEstudiantes repo;
    repo.registrar(Estudiante("E001", "Ana Quispe"));
    repo.obtener("E001").registrarNota(12);

    double promedioAntes = repo.obtener("E001").promedio();
    GeneradorReporte::generarTexto(repo.listar());
    GeneradorReporte::generarCsv(repo.listar());

    verificar(repo.obtener("E001").promedio() == promedioAntes,
              "generar_reporte_no_altera_la_entidad");
}

int main() {
    test_registrar_y_recuperar_estudiante_por_codigo();
    test_agregar_notas_en_limites_validos_0_y_20();
    test_calcular_promedio_con_tres_notas_conocidas();
    test_promedio_es_cero_sin_notas();
    test_aprobacion_en_el_limite_10_5();
    test_no_aprueba_justo_debajo_del_limite();
    test_rechaza_nota_fuera_de_rango_bajo();
    test_rechaza_nota_fuera_de_rango_alto();
    test_rechaza_codigo_de_estudiante_duplicado();
    test_generar_reporte_no_altera_la_entidad();

    std::cout << "\n" << pasadas << "/" << total << " pruebas pasaron\n";
    return 0;
}