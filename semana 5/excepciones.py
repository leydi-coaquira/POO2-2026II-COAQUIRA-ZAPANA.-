"""excepciones.py — Práctica S5, Actividad 1 
Jerarquía de excepciones del dominio académico + ErrorMatriculaDuplicada.
"""
import sys
try:
    sys.stdout.reconfigure(encoding="utf-8")
except AttributeError:
    pass


class ErrorAcademico(Exception):
    """Raíz de todas las excepciones del dominio académico."""

    def __init__(self, mensaje: str, contexto: dict | None = None):
        super().__init__(mensaje)
        self.contexto: dict = contexto or {}

    def __str__(self) -> str:
        base = super().__str__()
        if self.contexto:
            ctx = ", ".join(f"{k}={v!r}" for k, v in self.contexto.items())
            return f"{base}  ← contexto: {{{ctx}}}"
        return base


class ErrorValidacion(ErrorAcademico):
    """Datos de entrada que violan las reglas del dominio."""


class ErrorCalificacion(ErrorValidacion):
    """Nota fuera del rango [0, 20] del sistema peruano vigesimal."""

    def __init__(self, valor: float, campo: str = "nota"):
        super().__init__(
            f"Calificación {valor} fuera de [0, 20]",
            contexto={"campo": campo, "valor": valor, "rango": "[0, 20]"},
        )
        self.valor_invalido = valor


# (Actividad 1): matrícula duplicada
class ErrorMatriculaDuplicada(ErrorValidacion):
    """El código de estudiante ya figura en la lista de matriculados."""

    def __init__(self, codigo_estudiante: str):
        super().__init__(
            f"El estudiante {codigo_estudiante} ya está matriculado",
            contexto={"codigo_estudiante": codigo_estudiante},
        )
        self.codigo_estudiante = codigo_estudiante


def validar_matricula(codigo: int, matriculados: list[int]) -> None:
    """Lanza ErrorMatriculaDuplicada si codigo ya está en matriculados."""
    if codigo in matriculados:
        raise ErrorMatriculaDuplicada(str(codigo))
    matriculados.append(codigo)


def matricular_desde_archivo(ruta: str, codigo: int,
                              matriculados: list[int]) -> None:
    
    try:
        with open(ruta) as f:
            f.read()
    except (OSError, ValueError) as causa:
        raise ErrorAcademico(
            f"No se pudo leer la configuración de matrícula ({ruta})"
        ) from causa
    validar_matricula(codigo, matriculados)


if __name__ == "__main__":
    matriculados = [1001, 1002, 1003]

    print("=== Caso 1: matrícula nueva (no lanza) ===")
    validar_matricula(1004, matriculados)
    print(f"[OK] Código 1004 matriculado. Total: {len(matriculados)}")

    print("\n=== Caso 2: matrícula duplicada ===")
    try:
        validar_matricula(1001, matriculados)
    except ErrorMatriculaDuplicada as e:
        print(f"Error específico: {e}")
    except ErrorValidacion as e:
        print(f"Error de validación: {e}")
    except ErrorAcademico as e:
        print(f"Error de dominio: {e}")

    print("\n=== Caso 3: la misma captura también atrapa "
          "ErrorCalificacion (es subclase de ErrorValidacion) ===")
    try:
        raise ErrorCalificacion(25.0)
    except ErrorMatriculaDuplicada as e:
        print(f"Duplicado: {e}")
    except ErrorValidacion as e:
        print(f"Validación (genérica): {e}")
    except ErrorAcademico as e:
        print(f"Dominio: {e}")

    print("\n=== Caso 4: encadenamiento con 'raise ... from ...' ===")
    try:
        matricular_desde_archivo("config_matricula_inexistente.ini",
                                  1005, matriculados)
    except ErrorAcademico as e:
        print(f"Error: {e}")
        print(f"Causa original (__cause__): {e.__cause__!r}")

    print(f"\nTotal final de matriculados: {len(matriculados)}")