// MaterialBibliografico.hpp — Practica S4, Actividad 5 (Proyecto Integrador)
// Jerarquia de herencia (Semana 3) + refactor con interfaz y composicion (Semana 4)
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
using namespace std;

// Jerarquia de herencia (Semana 3 — sin cambios)

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
        if (!disponible_) throw runtime_error(titulo_ + " ya esta prestado");
        disponible_ = false;
    }
    void marcarDevuelto() { disponible_ = true; }

    virtual int diasPrestamo() const = 0;

    virtual string describir() const {
        return "[" + codigo_ + "] " + titulo_ + " - " +
               (disponible_ ? "disponible" : "prestado") + ", " +
               to_string(diasPrestamo()) + " dias de prestamo";
    }
};

class LibroFisico : public MaterialBibliografico {
    string ubicacionEstante_;
public:
    LibroFisico(string codigo, string titulo, string autor, string ubicacionEstante)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          ubicacionEstante_(move(ubicacionEstante)) {}
    int diasPrestamo() const override { return 14; }
};

class LibroDigital : public MaterialBibliografico {
    string formato_;
    double tamanoMb_;
public:
    LibroDigital(string codigo, string titulo, string autor, string formato, double tamanoMb)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          formato_(move(formato)), tamanoMb_(tamanoMb) {
        if (formato_ != "PDF" && formato_ != "EPUB")
            throw invalid_argument("Formato no soportado: " + formato_);
    }
    int diasPrestamo() const override { return 7; }
};

class Lector {
    string codigo_, nombre_, tipo_;
public:
    Lector(string codigo, string nombre, string tipo)
        : codigo_(move(codigo)), nombre_(move(nombre)), tipo_(move(tipo)) {}
    const string& getNombre() const { return nombre_; }
};


// ═══════════════════════════════════════════════════════════════════
// NUEVO (Semana 4) — Interfaz pura INotificable
// ═══════════════════════════════════════════════════════════════════
class INotificable {
public:
    virtual ~INotificable() = default;
    virtual bool notificar(const string& destinatario, const string& mensaje) = 0;
    virtual bool estaDisponibleNotificador() const = 0;
};

class NotificadorConsola : public INotificable {
public:
    bool estaDisponibleNotificador() const override { return true; }
    bool notificar(const string& destinatario, const string& mensaje) override {
        cout << "[AVISO BIBLIOTECA -> " << destinatario << "]: " << mensaje << "\n";
        return true;
    }
};


class Prestamo {
    MaterialBibliografico* material_;
    Lector* lector_;
    INotificable* notificador_;  // puede ser nullptr: notificador es opcional
    int diasPrestamo_;

public:
    Prestamo(MaterialBibliografico& material, Lector& lector,
             INotificable* notificador = nullptr)
        : material_(&material), lector_(&lector), notificador_(notificador),
          diasPrestamo_(material.diasPrestamo()) {
        material_->marcarPrestado();
        if (notificador_ && notificador_->estaDisponibleNotificador()) {
            notificador_->notificar(
                lector_->getNombre(),
                "Registraste el prestamo de '" + material_->getTitulo() +
                "'. Dias de prestamo: " + to_string(diasPrestamo_));
        }
    }

    string describir() const {
        return "Prestamo: " + material_->getTitulo() + " -> " + lector_->getNombre() +
               " (" + to_string(diasPrestamo_) + " dias)";
    }
};

// Itera sobre una lista HETEROGENEA de punteros a la base: polimorfismo dinamico.
inline void mostrarCatalogo(const vector<unique_ptr<MaterialBibliografico>>& catalogo) {
    for (const auto& m : catalogo) cout << m->describir() << "\n";
}