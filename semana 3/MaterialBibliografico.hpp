// MaterialBibliografico.hpp — Practica S3, Actividad 5 (Proyecto Integrador)
// Jerarquia de herencia MaterialBibliografico() LibroFisico, LibroDigital
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
using namespace std;

class MaterialBibliografico {
protected:
    string codigo_;
    string titulo_;
    string autor_;
    bool disponible_ = true;

public:
    MaterialBibliografico(string codigo, string titulo, string autor)
        : codigo_(move(codigo)), titulo_(move(titulo)), autor_(move(autor)) {
        if (codigo_.size() < 3) {
            throw invalid_argument("Codigo de material invalido: " + codigo_);
        }
    }

    virtual ~MaterialBibliografico() = default;  // destructor virtual OBLIGATORIO

    const string& getCodigo() const { return codigo_; }
    const string& getTitulo() const { return titulo_; }
    bool estaDisponible() const { return disponible_; }

    void marcarPrestado() {
        if (!disponible_) {
            throw runtime_error(titulo_ + " ya esta prestado");
        }
        disponible_ = false;
    }

    void marcarDevuelto() { disponible_ = true; }

    // Metodo virtual puro: cada subclase DEBE sobrescribirlo
    virtual int diasPrestamo() const = 0;

    // Metodo virtual con implementacion por defecto (Template Method)
    // usa diasPrestamo() de forma polimorfica sin necesitar override.
    virtual string describir() const {
        return "[" + codigo_ + "] " + titulo_ + " (" + autor_ + ") - " +
               (disponible_ ? "disponible" : "prestado") + ", " +
               to_string(diasPrestamo()) + " dias de prestamo";
    }
};

class LibroFisico : public MaterialBibliografico {
    string ubicacionEstante_;

public:
    LibroFisico(string codigo, string titulo, string autor,
                string ubicacionEstante)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          ubicacionEstante_(move(ubicacionEstante)) {}

    const string& getUbicacionEstante() const { return ubicacionEstante_; }

    int diasPrestamo() const override { return 14; }
};

class LibroDigital : public MaterialBibliografico {
    string formato_;
    double tamanoMb_;

public:
    LibroDigital(string codigo, string titulo, string autor,
                 string formato, double tamanoMb)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          formato_(move(formato)), tamanoMb_(tamanoMb) {
        if (formato_ != "PDF" && formato_ != "EPUB") {
            throw invalid_argument("Formato no soportado: " + formato_);
        }
    }

    const string& getFormato() const { return formato_; }

    int diasPrestamo() const override { return 7; }
};

// Itera sobre una lista HETEROGENEA de punteros a la base: polimorfismo dinamico.
inline void mostrarCatalogo(const vector<unique_ptr<MaterialBibliografico>>& catalogo) {
    for (const auto& m : catalogo) {
        cout << m->describir() << "\n";
    }
}

// Contrato LSP: toda subclase debe cumplirlo sin excepcion.
inline bool verificarContratoMaterial(const MaterialBibliografico& m) {
    bool ok = !m.getTitulo().empty();
    ok = ok && m.diasPrestamo() > 0;
    ok = ok && m.describir().find(m.getCodigo()) != string::npos;
    return ok;
}