// ── main.cpp — Práctica S4, Actividad 3 ──────────────────────────────────
// Ejecutar: ./notificaciones
#include "notificador.hpp"

int main() {
    SistemaAlertas sistema;
    sistema.agregar(std::make_unique<NotificadorEmail>("smtp.unap.edu.pe"));
    sistema.agregar(std::make_unique<NotificadorSMS>("Movistar"));

    int n = sistema.alertar(
        "jmamani@unap.edu.pe",
        "ALERTA: Temperatura Titicaca supera 18 grados"
    );
    std::cout << "Alerta enviada por " << n << " canal(es).\n";
    return 0;
}