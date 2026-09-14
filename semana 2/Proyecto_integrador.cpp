#include "Proyecto_integrador.hpp"
#include <ctime>

int main() {
    EstacionMonitoreo est("EST01", "Bahia de Huata");
    Sensor s1("SEN001", "temperatura", "Orilla - Huata");
    Sensor s2("SEN002", "turbidez", "Zona de totorales");
    est.agregarSensor(s1);
    est.agregarSensor(s2);
    est.imprimir();
    s1.imprimir();

    Lectura lect(std::time(nullptr), 14.5, "SEN001");
    std::cout << "Lectura valida: " << lect.verificarConSensor(s1) << "\n";
    std::cout << "Antiguedad (h): " << lect.antiguedadHoras() << "\n";

    // Invariantes -- deben lanzar error
    
    try {
        Sensor bad("BAD", "temperatura", "x");
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    try {
        est.agregarSensor(s1); // duplicado
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    try {
        Lectura lectMala(std::time(nullptr), 999, "SEN001");
        lectMala.verificarConSensor(s1); // fuera de rango
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error esperado: " << ex.what() << "\n";
    }
    return 0;
}