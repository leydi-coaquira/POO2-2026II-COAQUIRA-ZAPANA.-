// ── result.hpp — patrón Result con std::variant (C++17) ────────────────
#pragma once
#include <variant>
#include <string>
#include <stdexcept>
#include <functional>
#include <unordered_map>

template<typename T>
class Result {
    std::variant<T, std::string> dato_;

public:
    static Result ok(T valor) {
        return Result(std::variant<T, std::string>(std::in_place_index<0>, std::move(valor)));
    }
    static Result fallo(std::string e) {
        return Result(std::variant<T, std::string>(std::in_place_index<1>, std::move(e)));
    }

    bool esExito() const noexcept { return dato_.index() == 0; }

    const T& valor() const {
        if (!esExito())
            throw std::logic_error("Result fallido: " + error());
        return std::get<0>(dato_);
    }

    const std::string& error() const { return std::get<1>(dato_); }

    template<typename F>
    auto map(F f) const -> Result<decltype(f(std::declval<T>()))> {
        using U = decltype(f(std::declval<T>()));
        if (esExito()) return Result<U>::ok(f(valor()));
        else           return Result<U>::fallo(error());
    }

private:
    explicit Result(std::variant<T, std::string> d) : dato_(std::move(d)) {}
};

// 'inline' permite definir esto en el header sin violar la ODR
inline std::unordered_map<int, std::string> db = {{1001, "Juan Mamani"}};

inline Result<std::string> buscarEstudiante(int codigo) {
    auto it = db.find(codigo);
    if (it == db.end())
        return Result<std::string>::fallo("Código " + std::to_string(codigo) + " no encontrado");
    return Result<std::string>::ok(it->second);
}