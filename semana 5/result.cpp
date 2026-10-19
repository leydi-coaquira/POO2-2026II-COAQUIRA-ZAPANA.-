#include "result.hpp"
#include <iostream>

int main() {
    auto res = buscarEstudiante(1001);
    if (res.esExito())
        std::cout << "Encontrado: " << res.valor() << "\n";
    else
        std::cerr << "Error: " << res.error() << "\n";

    auto res2 = buscarEstudiante(9999);
    if (res2.esExito())
        std::cout << "Encontrado: " << res2.valor() << "\n";
    else
        std::cerr << "Error: " << res2.error() << "\n";

    return 0;
}