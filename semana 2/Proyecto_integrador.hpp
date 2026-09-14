// Practica S2 - Actividad 5 
// Encapsulamiento estricto de Sensor, EstacionMonitoreo y Lectura
#pragma once
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <ctime>
#include <map>
#include <iostream>
#include <algorithm>

class Sensor {
public:
    Sensor(std::string codigo, std::string tipo, std::string ubicacion) {
        setCodigo(codigo);
        setTipo(tipo);
        setUbicacion(ubicacion);
    }

    void setCodigo(const std::string& cod) {
        static const std::regex patron(R"(^SEN[0-9]{3}$)");
        if (!std::regex_match(cod, patron)) {
            throw std::invalid_argument("Codigo de sensor invalido: " + cod);
        }
        codigo_ = cod;
    }

    void setTipo(const std::string& t) {
        static const std::vector<std::string> validos = {"temperatura", "turbidez", "nivel"};
        if (std::find(validos.begin(), validos.end(), t) == validos.end()) {
            throw std::invalid_argument("Tipo de sensor invalido: " + t);
        }
        tipo_ = t;
    }

    void setUbicacion(const std::string& u) {
        if (u.size() < 3) {
            throw std::invalid_argument("La ubicacion debe tener al menos 3 caracteres");
        }
        ubicacion_ = u;
    }

    const std::string& getCodigo() const { return codigo_; }
    const std::string& getTipo() const { return tipo_; }
    const std::string& getUbicacion() const { return ubicacion_; }

    // Propiedad calculada (const, sin atributo de respaldo)
    std::string unidadMedida() const {
        if (tipo_ == "temperatura") return "C";
        if (tipo_ == "turbidez") return "NTU";
        return "m"; // nivel
    }

    // Metodo de negocio: verifica precondicion de rango fisico
    bool validarRango(double valor) const {
        double minimo, maximo;
        if (tipo_ == "temperatura") { minimo = -5.0; maximo = 40.0; }
        else if (tipo_ == "turbidez") { minimo = 0.0; maximo = 1000.0; }
        else { minimo = 0.0; maximo = 50.0; }
        if (valor < minimo || valor > maximo) {
            throw std::invalid_argument(
                "Valor fuera del rango fisico esperado para " + tipo_);
        }
        return true;
    }

    void imprimir() const {
        std::cout << codigo_ << " (" << tipo_ << ", " << unidadMedida()
                  << ") @ " << ubicacion_ << "\n";
    }

private:
    std::string codigo_;
    std::string tipo_;
    std::string ubicacion_;
};

class EstacionMonitoreo {
public:
    static constexpr int MAX_SENSORES = 10;

    EstacionMonitoreo(std::string idEstacion, std::string nombre) {
        setIdEstacion(idEstacion);
        setNombre(nombre);
    }

    void setIdEstacion(const std::string& id) {
        static const std::regex patron(R"(^EST[0-9]{2}$)");
        if (!std::regex_match(id, patron)) {
            throw std::invalid_argument("Id de estacion invalido: " + id);
        }
        idEstacion_ = id;
    }

    void setNombre(const std::string& n) {
        if (n.empty()) {
            throw std::invalid_argument("El nombre de la estacion no puede estar vacio");
        }
        nombre_ = n;
    }

    const std::string& getIdEstacion() const { return idEstacion_; }
    const std::string& getNombre() const { return nombre_; }

    // Propiedades calculadas (const)
    int totalSensores() const { return static_cast<int>(sensores_.size()); }
    bool tieneCapacidad() const { return totalSensores() < MAX_SENSORES; }

    // Metodo de negocio: verifica precondiciones antes de modificar el estado
    void agregarSensor(const Sensor& sensor) {
        if (!tieneCapacidad()) {
            throw std::runtime_error("La estacion " + idEstacion_ + " ya alcanzo el maximo de sensores");
        }
        for (const auto& s : sensores_) {
            if (s.getCodigo() == sensor.getCodigo()) {
                throw std::invalid_argument("El sensor " + sensor.getCodigo() + " ya esta registrado");
            }
        }
        sensores_.push_back(sensor);
    }

    void imprimir() const {
        std::cout << idEstacion_ << " - " << nombre_
                  << " (" << totalSensores() << "/" << MAX_SENSORES << " sensores)\n";
    }

private:
    std::string idEstacion_;
    std::string nombre_;
    std::vector<Sensor> sensores_;
};

class Lectura {
public:
    Lectura(std::time_t fechaHora, double valor, std::string sensorId)
        : valor_(valor) {
        setFechaHora(fechaHora);
        setSensorId(sensorId);
    }

    void setFechaHora(std::time_t f) {
        if (f > std::time(nullptr)) {
            throw std::invalid_argument("La fecha_hora de una lectura no puede estar en el futuro");
        }
        fechaHora_ = f;
    }

    void setSensorId(const std::string& id) {
        if (id.empty()) {
            throw std::invalid_argument("sensorId no puede estar vacio");
        }
        sensorId_ = id;
    }

    std::time_t getFechaHora() const { return fechaHora_; }
    double getValor() const { return valor_; }
    const std::string& getSensorId() const { return sensorId_; }

    // Propiedad calculada (const): antiguedad en horas
    double antiguedadHoras() const {
        return std::difftime(std::time(nullptr), fechaHora_) / 3600.0;
    }

    // Metodo de negocio: exige la precondicion de pertenencia al sensor
    bool verificarConSensor(const Sensor& sensor) const {
        if (sensor.getCodigo() != sensorId_) {
            throw std::invalid_argument(
                "La lectura pertenece a " + sensorId_ + ", no a " + sensor.getCodigo());
        }
        return sensor.validarRango(valor_);
    }

private:
    std::time_t fechaHora_;
    double valor_;
    std::string sensorId_;
};