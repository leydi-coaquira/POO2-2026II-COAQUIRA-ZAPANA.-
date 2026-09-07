from __future__ import annotations
from dataclasses import dataclass
import csv, io

NOTA_MINIMA = 0.0
NOTA_MAXIMA = 20.0
PROMEDIO_APROBACION = 10.5


@dataclass(frozen=True)
class Nota:
    valor: float

    def __post_init__(self):
        if not (NOTA_MINIMA <= self.valor <= NOTA_MAXIMA):
            raise ValueError(f"La nota {self.valor} esta fuera de rango [0, 20]")


class Estudiante:
    def __init__(self, codigo: str, nombre: str):
        self._codigo = codigo
        self._nombre = nombre
        self._notas: list[Nota] = []

    @property
    def codigo(self):
        return self._codigo

    @property
    def nombre(self):
        return self._nombre

    @property
    def notas(self):
        return tuple(self._notas)

    def registrar_nota(self, valor: float):
        self._notas.append(Nota(valor))

    @property
    def promedio(self):
        if not self._notas:
            return 0.0
        return sum(n.valor for n in self._notas) / len(self._notas)

    def esta_aprobado(self):
        return self.promedio >= PROMEDIO_APROBACION


class RepositorioEstudiantes:
    def __init__(self):
        self._estudiantes: dict[str, Estudiante] = {}

    def registrar(self, estudiante: Estudiante):
        if estudiante.codigo in self._estudiantes:
            raise ValueError(f"Ya existe un estudiante con codigo {estudiante.codigo}")
        self._estudiantes[estudiante.codigo] = estudiante

    def obtener(self, codigo: str):
        return self._estudiantes[codigo]

    def listar(self):
        return list(self._estudiantes.values())


class GeneradorReporte:
    @staticmethod
    def _estado(e: Estudiante):
        return "APROBADO" if e.esta_aprobado() else "DESAPROBADO"

    @classmethod
    def generar_texto(cls, estudiantes):
        return "\n".join(
            f"{e.codigo} - {e.nombre}: promedio={e.promedio:.2f} ({cls._estado(e)})"
            for e in estudiantes
        )

    @classmethod
    def generar_csv(cls, estudiantes):
        buf = io.StringIO()
        w = csv.writer(buf)
        w.writerow(["codigo", "nombre", "promedio", "estado"])
        for e in estudiantes:
            w.writerow([e.codigo, e.nombre, f"{e.promedio:.2f}", cls._estado(e)])
        return buf.getvalue()


if __name__ == "__main__":
    repo = RepositorioEstudiantes()
    repo.registrar(Estudiante("E001", "Ana Quispe"))
    repo.obtener("E001").registrar_nota(12)
    print(GeneradorReporte.generar_texto(repo.listar()))