// MaterialBibliografico.hpp
// Proyecto Integrador - Biblioteca Digital Huata

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <regex>
#include <algorithm>
using namespace std;

#include "dominio_errores.hpp"
#include "resultados.hpp"

// Jerarquia de herencia (Semana 3) -- con precondiciones (Semana 5)

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
            throw ErrorValidacion("Codigo de material invalido: " + codigo_);
        }
        if (titulo_.empty()) {
            throw ErrorValidacion("El titulo no puede estar vacio");
        }
        if (autor_.empty()) {
            throw ErrorValidacion("El autor no puede estar vacio");
        }
    }

    virtual ~MaterialBibliografico() = default; // destructor virtual OBLIGATORIO

    const string& getCodigo() const { return codigo_; }
    const string& getTitulo() const { return titulo_; }
    bool estaDisponible() const { return disponible_; }

    // Mutador: marcar como prestado.
    void marcarPrestado() {
        if (!disponible_) {
            throw ErrorEstado(titulo_ + " ya esta prestado");
        }
        disponible_ = false;
    }

    // Mutador: marcar como devuelto.
    void marcarDevuelto() {
        if (disponible_) {
            throw ErrorEstado(titulo_ + " no estaba prestado, no se puede devolver");
        }
        disponible_ = true;
    }

    // Metodo virtual puro: cada subclase DEBE sobrescribirlo.
    virtual int diasPrestamo() const = 0;

    // Metodo virtual con implementacion por defecto (Template Method).
    virtual string describir() const {
        return "[" + codigo_ + "] " + titulo_ + " (" + autor_ + ") - " +
               (disponible_ ? "disponible" : "prestado") + ", " +
               to_string(diasPrestamo()) + " dias de prestamo";
    }

    // Invariante de clase (Actividad 5, Practica S5).
    void verificarInvariante() const {
        if (codigo_.size() < 3) {
            throw ErrorEstado("Invariante violado: codigo invalido");
        }
        if (titulo_.empty() || autor_.empty()) {
            throw ErrorEstado("Invariante violado: titulo o autor vacio");
        }
    }
};

class LibroFisico : public MaterialBibliografico {
    string ubicacionEstante_;

public:
    LibroFisico(string codigo, string titulo, string autor, string ubicacionEstante)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          ubicacionEstante_(move(ubicacionEstante)) {
        if (ubicacionEstante_.empty()) {
            throw ErrorValidacion("La ubicacion de estante no puede estar vacia");
        }
    }

    const string& getUbicacionEstante() const { return ubicacionEstante_; }
    int diasPrestamo() const override { return 14; }
};

class LibroDigital : public MaterialBibliografico {
    string formato_;
    double tamanoMb_;

public:
    LibroDigital(string codigo, string titulo, string autor, string formato, double tamanoMb)
        : MaterialBibliografico(move(codigo), move(titulo), move(autor)),
          formato_(move(formato)), tamanoMb_(tamanoMb) {
        if (formato_ != "PDF" && formato_ != "EPUB") {
            throw ErrorValidacion("Formato no soportado: " + formato_);
        }
        if (tamanoMb_ <= 0) {
            throw ErrorValidacion("El tamano en MB debe ser positivo");
        }
    }

    const string& getFormato() const { return formato_; }
    int diasPrestamo() const override { return 7; }
};

// No pertenece a la jerarquia; se relaciona por composicion con Prestamo.
class Lector {
    string codigo_, nombre_, tipo_;

    static void validar(const string& codigo, const string& nombre, const string& tipo) {
        static const regex patron(R"(^LEC[0-9]{3}$)");
        if (!regex_match(codigo, patron)) {
            throw ErrorValidacion("Codigo de lector invalido: " + codigo +
                                   " (formato LEC+3 digitos)");
        }
        if (nombre.empty()) {
            throw ErrorValidacion("El nombre del lector no puede estar vacio");
        }
        static const vector<string> validos = {"estudiante", "docente", "comunidad"};
        if (find(validos.begin(), validos.end(), tipo) == validos.end()) {
            throw ErrorValidacion("Tipo de lector invalido: " + tipo);
        }
    }

public:
    Lector(string codigo, string nombre, string tipo)
        : codigo_(move(codigo)), nombre_(move(nombre)), tipo_(move(tipo)) {
        validar(codigo_, nombre_, tipo_);
    }

    const string& getCodigo() const { return codigo_; }
    const string& getNombre() const { return nombre_; }
    const string& getTipo() const { return tipo_; }
};

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
    INotificable* notificador_; // puede ser nullptr: notificador es opcional
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

inline Result<MaterialBibliografico*, string> buscarEnCatalogo(
    const vector<unique_ptr<MaterialBibliografico>>& catalogo,
    const string& codigo) {
    for (const auto& m : catalogo) {
        if (m->getCodigo() == codigo) {
            return Result<MaterialBibliografico*, string>::Ok(m.get());
        }
    }
    return Result<MaterialBibliografico*, string>::Err(
        "No se encontro material con codigo '" + codigo + "' en el catalogo");
}