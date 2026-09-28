// Practica S2 - Actividad 5
// Encapsulamiento estricto de Libro, Lector y Prestamo
#pragma once

#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <ctime>
#include <map>
#include <iostream>
#include <algorithm>

class Libro {
public:
    Libro(std::string codigo, std::string titulo, std::string autor, std::string categoria) {
        setCodigo(codigo);
        setTitulo(titulo);
        setAutor(autor);
        setCategoria(categoria);
    }

    void setCodigo(const std::string& cod) {
        static const std::regex patron(R"(^BIB[0-9]{3}$)");
        if (!std::regex_match(cod, patron)) {
            throw std::invalid_argument("Codigo de libro invalido: " + cod);
        }
        codigo_ = cod;
    }

    void setTitulo(const std::string& t) {
        if (t.empty()) {
            throw std::invalid_argument("El titulo no puede estar vacio");
        }
        titulo_ = t;
    }

    void setAutor(const std::string& a) {
        if (a.empty()) {
            throw std::invalid_argument("El autor no puede estar vacio");
        }
        autor_ = a;
    }

    void setCategoria(const std::string& c) {
        static const std::vector<std::string> validas = {"infantil", "general", "referencia"};
        if (std::find(validas.begin(), validas.end(), c) == validas.end()) {
            throw std::invalid_argument("Categoria invalida: " + c);
        }
        categoria_ = c;
    }

    const std::string& getCodigo() const { return codigo_; }
    const std::string& getTitulo() const { return titulo_; }
    const std::string& getAutor() const { return autor_; }
    const std::string& getCategoria() const { return categoria_; }
    bool estaDisponible() const { return disponible_; }

    // Propiedad calculada (const, sin atributo de respaldo)
    int diasPrestamo() const {
        if (categoria_ == "infantil") return 21;
        if (categoria_ == "referencia") return 0;
        return 14; // general
    }

    // Metodo de negocio: verifica precondicion antes de operar
    void prestar() {
        if (categoria_ == "referencia") {
            throw std::runtime_error("'" + titulo_ + "' es de referencia: no se presta");
        }
        if (!disponible_) {
            throw std::runtime_error("'" + titulo_ + "' ya esta prestado");
        }
        disponible_ = false;
    }

    void devolver() { disponible_ = true; }

    void imprimir() const {
        std::cout << "[" << codigo_ << "] " << titulo_ << " (" << autor_
                  << ") - " << categoria_ << ", "
                  << (disponible_ ? "disponible" : "prestado") << "\n";
    }

private:
    std::string codigo_;
    std::string titulo_;
    std::string autor_;
    std::string categoria_;
    bool disponible_ = true;
};

class Lector {
public:
    static constexpr int MAX_LIMITE = 3;

    Lector(std::string codigo, std::string nombre, std::string tipo) {
        setCodigo(codigo);
        setNombre(nombre);
        setTipo(tipo);
    }

    void setCodigo(const std::string& cod) {
        static const std::regex patron(R"(^LEC[0-9]{3}$)");
        if (!std::regex_match(cod, patron)) {
            throw std::invalid_argument("Codigo de lector invalido: " + cod);
        }
        codigo_ = cod;
    }

    void setNombre(const std::string& n) {
        if (n.empty()) {
            throw std::invalid_argument("El nombre no puede estar vacio");
        }
        nombre_ = n;
    }

    void setTipo(const std::string& t) {
        static const std::vector<std::string> validos = {"estudiante", "docente", "comunidad"};
        if (std::find(validos.begin(), validos.end(), t) == validos.end()) {
            throw std::invalid_argument("Tipo de lector invalido: " + t);
        }
        tipo_ = t;
    }

    const std::string& getCodigo() const { return codigo_; }
    const std::string& getNombre() const { return nombre_; }
    const std::string& getTipo() const { return tipo_; }
    int getPrestamosActivos() const { return prestamosActivos_; }

    // Propiedad calculada (const)
    int limitePrestamos() const {
        if (tipo_ == "comunidad") return 1;
        return 3; // estudiante o docente
    }

    // Metodo de negocio: verifica precondicion antes de operar
    void registrarPrestamo() {
        if (prestamosActivos_ >= limitePrestamos()) {
            throw std::runtime_error(nombre_ + " ya alcanzo su limite de " +
                                      std::to_string(limitePrestamos()) + " prestamos");
        }
        ++prestamosActivos_;
    }

    void registrarDevolucion() {
        if (prestamosActivos_ > 0) --prestamosActivos_;
    }

    void imprimir() const {
        std::cout << "[" << codigo_ << "] " << nombre_ << " (" << tipo_ << ") - "
                  << prestamosActivos_ << "/" << limitePrestamos() << " prestamos\n";
    }

private:
    std::string codigo_;
    std::string nombre_;
    std::string tipo_;
    int prestamosActivos_ = 0;
};

class Prestamo {
public:
    Prestamo(Libro& libro, Lector& lector, std::time_t fechaPrestamo)
        : libro_(libro), lector_(lector) {
        setFechaPrestamo(fechaPrestamo);
        // Precondiciones de negocio antes de crear el prestamo:
        libro_.prestar();
        lector_.registrarPrestamo();
    }

    void setFechaPrestamo(std::time_t f) {
        if (f > std::time(nullptr)) {
            throw std::invalid_argument("La fecha de prestamo no puede estar en el futuro");
        }
        fechaPrestamo_ = f;
    }

    std::time_t getFechaPrestamo() const { return fechaPrestamo_; }

    // Propiedad calculada (const): fecha de devolucion esperada
    std::time_t fechaDevolucionEsperada() const {
        return fechaPrestamo_ + static_cast<std::time_t>(libro_.diasPrestamo() * 86400);
    }

    void cerrar() {
        libro_.devolver();
        lector_.registrarDevolucion();
    }

private:
    Libro& libro_;
    Lector& lector_;
    std::time_t fechaPrestamo_;
};