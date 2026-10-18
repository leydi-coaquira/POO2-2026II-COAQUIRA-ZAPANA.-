// notificador.hpp Práctica S4, Actividad 3 
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

//Interfaz pura: todos los métodos son virtuales puros, sin estado
class INotificador {
public:
    virtual ~INotificador() = default;
    virtual bool        enviar(const std::string& dest,
                                const std::string& msg) = 0;
    virtual bool        esDisponible() const = 0;
    virtual std::string nombre()       const = 0;
};

// Implementaciones concretas 
class NotificadorEmail : public INotificador {
    std::string servidor_;
    bool        activo_ = true;

public:
    explicit NotificadorEmail(std::string servidor)
        : servidor_(std::move(servidor)) {}

    bool esDisponible() const override { return activo_; }

    std::string nombre() const override {
        return "Email (" + servidor_ + ")";
    }

    bool enviar(const std::string& dest,
                const std::string& msg) override {
        std::cout << "[EMAIL -> " << dest << "]: "
                  << msg.substr(0, 50) << "...\n";
        return true;
    }
};

class NotificadorSMS : public INotificador {
    std::string proveedor_;
    bool        activo_ = true;
    static constexpr int MAX_CHARS = 160;

public:
    explicit NotificadorSMS(std::string proveedor)
        : proveedor_(std::move(proveedor)) {}

    bool esDisponible() const override { return activo_; }
    std::string nombre() const override { return "SMS (" + proveedor_ + ")"; }

    bool enviar(const std::string& dest,
                const std::string& msg) override {
        if (static_cast<int>(msg.size()) > MAX_CHARS)
            throw std::invalid_argument("SMS excede 160 caracteres");
        std::cout << "[SMS -> " << dest << "]: " << msg << "\n";
        return true;
    }
};

class SistemaAlertas {
    std::vector<std::unique_ptr<INotificador>> notificadores_;

public:
    void agregar(std::unique_ptr<INotificador> n) {
        notificadores_.push_back(std::move(n));
    }

    int alertar(const std::string& dest, const std::string& msg) {
        int enviados = 0;
        for (auto& n : notificadores_) {
            if (!n->esDisponible()) continue;
            try {
                if (n->enviar(dest, msg)) ++enviados;
            } catch (const std::exception& e) {
                std::cerr << "Error en " << n->nombre()
                          << ": " << e.what() << "\n";
            }
        }
        return enviados;
    }
};