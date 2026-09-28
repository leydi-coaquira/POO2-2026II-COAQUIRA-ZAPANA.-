// main_biblioteca.cpp — Practica S3, Actividad 5
#include "MaterialBibliografico.hpp"

int main() {
    vector<unique_ptr<MaterialBibliografico>> catalogo;
    catalogo.push_back(make_unique<LibroFisico>(
        "BIB001", "Los rios profundos", "Jose Maria Arguedas", "Estante A-3"));
    catalogo.push_back(make_unique<LibroDigital>(
        "BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2));

    mostrarCatalogo(catalogo);

    cout << "\nVerificacion LSP:\n";
    for (const auto& m : catalogo) {
        if (!verificarContratoMaterial(*m)) {
            cout << "LSP VIOLADO en " << m->getCodigo() << "\n";
            return 1;
        }
    }
    cout << "OK - LibroFisico y LibroDigital sustituyen completamente a "
                 "MaterialBibliografico\n";

    return 0;
}