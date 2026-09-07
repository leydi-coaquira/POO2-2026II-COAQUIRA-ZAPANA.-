# SU SOLUCIÓN REFACTORIZADA — Práctica S1, Actividad 2
from dataclasses import dataclass

@dataclass
class Nota:
    curso: str
    calificacion: float

    def __post_init__(self):
        if not (0 <= self.calificacion <= 20):
            raise ValueError(
                f"La calificación debe estar entre 0 y 20 (recibido: {self.calificacion})"
            )

class Estudiante:
    def __init__(self, nombre: str, codigo: int, escuela: str):
        self._nombre = nombre
        self._codigo = codigo
        self._escuela = escuela
        self._notas: list[Nota] = []

    @property
    def nombre(self) -> str:
        return self._nombre

    def agregar_nota(self, curso: str, calificacion: float) -> None:
        self._notas.append(Nota(curso, calificacion))

    def calcular_ppa(self) -> float:
        if not self._notas:
            return 0.0
        return sum(n.calificacion for n in self._notas) / len(self._notas)

    def __str__(self) -> str:
        return f"{self._nombre} ({self._codigo}, {self._escuela}) — PPA: {self.calcular_ppa():.2f}"

# Prueba — debe ejecutar sin errores:
e = Estudiante("Juan Carlos Mamani Quispe", 20210500, "Ing. Sistemas")
e.agregar_nota("POO II", 16)
e.agregar_nota("Base de Datos I", 14)
print(e)
print(f"PPA: {e.calcular_ppa():.2f}")

try:
    e.agregar_nota("Redes", 25)  # debe lanzar ValueError
except ValueError as ex:
    print(f"Error capturado correctamente: {ex}")