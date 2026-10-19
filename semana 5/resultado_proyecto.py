"""result.py — Tipo Result[T, E] con .map() encadenable.
Equivalente en Python de result.hpp en C++.
"""
from __future__ import annotations
from typing import Callable, Generic, Optional, TypeVar

T = TypeVar("T")
E = TypeVar("E")
U = TypeVar("U")


class Result(Generic[T, E]):
    def __init__(self, es_ok: bool, valor: Optional[T] = None, error: Optional[E] = None):
        self._es_ok = es_ok
        self._valor = valor
        self._error = error

    @staticmethod
    def Ok(valor: T) -> "Result[T, E]":
        return Result(True, valor=valor)

    @staticmethod
    def Err(error: E) -> "Result[T, E]":
        return Result(False, error=error)

    def es_exito(self) -> bool:
        return self._es_ok

    def es_error(self) -> bool:
        return not self._es_ok

    def valor(self) -> T:
        return self._valor

    def error(self) -> E:
        return self._error

    def map(self, f: Callable[[T], U]) -> "Result[U, E]":
        """Aplica f si es exito; propaga el error sin tocarlo si no."""
        if self._es_ok:
            return Result.Ok(f(self._valor))
        return Result.Err(self._error)