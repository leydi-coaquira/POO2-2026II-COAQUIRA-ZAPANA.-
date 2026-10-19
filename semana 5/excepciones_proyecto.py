"""excepciones.py — Practica S5, Actividad 5
Jerarquia de excepciones para el Proyecto Integrador (Biblioteca Digital
Huata).
"""
import sys
try:
    sys.stdout.reconfigure(encoding="utf-8")
except AttributeError:
    pass


class ErrorBiblioteca(Exception):
    """Raiz de todas las excepciones del dominio de la biblioteca."""

    def __init__(self, mensaje: str, contexto: dict | None = None):
        super().__init__(mensaje)
        self.contexto: dict = contexto or {}

    def __str__(self) -> str:
        base = super().__str__()
        if self.contexto:
            ctx = ", ".join(f"{k}={v!r}" for k, v in self.contexto.items())
            return f"{base}  <- contexto: {{{ctx}}}"
        return base


class ErrorValidacion(ErrorBiblioteca):
    """Datos de entrada que violan las reglas del dominio."""


class ErrorEstado(ErrorBiblioteca):
    """El objeto esta en un estado que no permite la operacion pedida
    (por ejemplo, prestar algo ya prestado, o devolver algo no prestado)."""


class ErrorCapacidad(ErrorBiblioteca):
    """Operacion que excede un limite permitido (reservado, por ejemplo,
    para un futuro limite de prestamos simultaneos por lector)."""

    def __init__(self, actual: int, maximo: int, entidad: str):
        super().__init__(
            f"{entidad}: capacidad agotada ({actual}/{maximo})",
            contexto={"actual": actual, "maximo": maximo, "entidad": entidad},
        )


class ErrorNoEncontrado(ErrorBiblioteca):
    """No se encontro el elemento buscado, se deja disponible para los
    casos que NO se hayan refactorizado a Result[T]."""