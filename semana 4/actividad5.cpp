// main_integrador.cpp — Practica S4, Actividad 5
#include "MaterialBibliografico.hpp"

int main() {
    vector<unique_ptr<MaterialBibliografico>> catalogo;
    catalogo.push_back(make_unique<LibroFisico>(
        "BIB001", "Los rios profundos", "Jose Maria Arguedas", "Estante A-3"));
    catalogo.push_back(make_unique<LibroDigital>(
        "BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2));

    mostrarCatalogo(catalogo);

    Lector lector("LEC001", "Rosa Mamani", "comunidad");

    cout << "\n--- Prestamo CON notificador inyectado ---\n";
    NotificadorConsola notificador;
    Prestamo p1(*catalogo[0], lector, &notificador);
    cout << p1.describir() << "\n";

    cout << "\n--- Prestamo SIN notificador (sigue funcionando) ---\n";
    Prestamo p2(*catalogo[1], lector);
    cout << p2.describir() << "\n";

    return 0;
}