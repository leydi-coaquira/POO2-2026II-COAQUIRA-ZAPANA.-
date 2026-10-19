// result.hpp
// Tipo Result<T, E> con soporte para encadenar transformaciones
// mediante .map().
#ifndef RESULT_HPP
#define RESULT_HPP

#include <utility>
#include <type_traits>

template <typename T, typename E>
class Result {
public:
    static Result<T, E> Ok(const T& valor) {
        Result<T, E> r;
        r.esOk_ = true;
        r.valor_ = valor;
        return r;
    }

    static Result<T, E> Err(const E& error) {
        Result<T, E> r;
        r.esOk_ = false;
        r.error_ = error;
        return r;
    }

    bool esExito() const { return esOk_; }
    bool esError() const { return !esOk_; }

    const T& valor() const { return valor_; }
    const E& error() const { return error_; }

    template <typename Func>
    auto map(Func f) const -> Result<decltype(f(std::declval<T>())), E> {
        using U = decltype(f(std::declval<T>()));
        if (esOk_) {
            return Result<U, E>::Ok(f(valor_));
        }
        return Result<U, E>::Err(error_);
    }

private:
    Result() : esOk_(false) {}

    bool esOk_;
    T valor_;
    E error_;
};

#endif // RESULT_HPP